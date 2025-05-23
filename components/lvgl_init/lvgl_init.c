#include <stdio.h>
#include <unistd.h>
#include <sys/lock.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "driver/i2c_master.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"
#include "lvgl_private.h"

#include "esp_lcd_panel_st7789.h"
#include "esp_lcd_touch_cst816s.h"

#include "lvgl_init.h"

#pragma region LVGL
static const char *LVGL_TASK_TAG = "LVGL Task";
static const char *LVGL_LOG_TAG = "LVGL Log";
extern void (*lvgl_tick_callback)() = NULL;

static bool notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
    lv_display_t *disp = (lv_display_t *)user_ctx;
    lv_display_flush_ready(disp);
    return false;
}

/* Rotate display and touch, when rotated screen in LVGL. Called when driver parameters are updated. */
static void lvgl_port_update_callback(lv_display_t *disp)
{
    esp_lcd_panel_handle_t panel_handle = lv_display_get_user_data(disp);
    lv_display_rotation_t rotation = lv_display_get_rotation(disp);

    bool swapXY = false;
    bool mirrorX = false;
    bool mirrorY = false;

    switch (rotation)
    {
    case LV_DISPLAY_ROTATION_0:
        // Rotate LCD display
        swapXY = false;
        mirrorX = true;
        mirrorY = false;
        break;
    case LV_DISPLAY_ROTATION_90:
        // Rotate LCD display
        swapXY = true;
        mirrorX = true;
        mirrorY = true;
        break;
    case LV_DISPLAY_ROTATION_180:
        // Rotate LCD display
        swapXY = false;
        mirrorX = false;
        mirrorY = true;
        break;
    case LV_DISPLAY_ROTATION_270:
        // Rotate LCD display
        swapXY = true;
        mirrorX = false;
        mirrorY = false;
        break;
    }
    esp_lcd_panel_swap_xy(panel_handle, swapXY);
    esp_lcd_panel_mirror(panel_handle, !mirrorX, mirrorY);
}

static void lvgl_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    lvgl_port_update_callback(disp);
    esp_lcd_panel_handle_t panel_handle = lv_display_get_user_data(disp);
    int offsetx1 = area->x1;
    int offsetx2 = area->x2;
    int offsety1 = area->y1;
    int offsety2 = area->y2;
    // because SPI LCD is big-endian, we need to swap the RGB bytes order
    lv_draw_sw_rgb565_swap(px_map, (offsetx2 + 1 - offsetx1) * (offsety2 + 1 - offsety1));
    // copy a buffer's content to a specific area of the display
    esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, px_map);
}

static void lvgl_touch_cb(lv_indev_t *indev, lv_indev_data_t *data)
{
    uint16_t touchpad_x[1] = {0};
    uint16_t touchpad_y[1] = {0};
    uint8_t touchpad_cnt = 0;

    esp_lcd_touch_handle_t touch_pad = lv_indev_get_user_data(indev);
    esp_lcd_touch_read_data(touch_pad);
    /* Get coordinates */
    bool touchpad_pressed = esp_lcd_touch_get_coordinates(touch_pad, touchpad_x, touchpad_y, NULL, &touchpad_cnt, 1);

    if (touchpad_pressed && touchpad_cnt > 0)
    {
        data->point.x = touchpad_x[0];
        data->point.y = touchpad_y[0];
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

void IRAM_ATTR increase_lvgl_tick(void *arg)
{
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

void lvgl_port_task(void *arg)
{
    uint32_t next_milis;

    while (1)
    {
        lv_lock();
        next_milis = lv_task_handler();
        (*lvgl_tick_callback)();
        lv_unlock();
        vTaskDelay(pdMS_TO_TICKS(next_milis));
    }
}

void esp_log_cb(lv_log_level_t level, const char *buf)
{
    switch (level)
    {
    case LV_LOG_LEVEL_TRACE:
        ESP_LOGV(LVGL_LOG_TAG, "%s", buf);
        break;
    case LV_LOG_LEVEL_INFO:
        ESP_LOGI(LVGL_LOG_TAG, "%s", buf);
        break;
    case LV_LOG_LEVEL_WARN:
        ESP_LOGW(LVGL_LOG_TAG, "%s", buf);
        break;
    case LV_LOG_LEVEL_ERROR:
        ESP_LOGE(LVGL_LOG_TAG, "%s", buf);
        break;
    case LV_LOG_LEVEL_USER:
    case LV_LOG_LEVEL_NONE:
        ESP_LOGI(LVGL_LOG_TAG, "%s", buf);
        break;
    default:
        break;
    }
}

lv_display_t *lvgl_init()
{

#pragma region LVGL_INIT

    ESP_LOGI(LVGL_TASK_TAG, "Initialize LVGL library");
    lv_init();
    // alloc draw buffers used by LVGL
    // it's recommended to choose the size of the draw buffer(s) to be at least 1/10 screen sized
    size_t draw_buffer_sz = LCD_H_RES * LVGL_DRAW_BUF_LINES * sizeof(lv_color16_t);
    void *buf1 = heap_caps_calloc(1, draw_buffer_sz, MALLOC_CAP_DMA);
    assert(buf1);
    void *buf2 = heap_caps_calloc(1, draw_buffer_sz, MALLOC_CAP_DMA);
    assert(buf2);
    lv_display_t *display = lv_display_create(LCD_H_RES, LCD_V_RES);
    // initialize LVGL draw buffers
    lv_display_set_buffers(display, buf1, buf2, draw_buffer_sz, LV_DISPLAY_RENDER_MODE_PARTIAL);
    // set color depth
    lv_display_set_color_format(display, LV_COLOR_FORMAT_RGB565);
    // set the callback which can copy the rendered image to an area of the display
    lv_display_set_flush_cb(display, lvgl_flush_cb);

    ESP_LOGI(LVGL_TASK_TAG, "Install LVGL tick timer");
    // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &increase_lvgl_tick,
        .name = "lvgl_tick",
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, LVGL_TICK_PERIOD_MS * 1000));

    ESP_LOGI(LVGL_TASK_TAG, "Register io panel event callback for LVGL flush ready notification");
    const esp_lcd_panel_io_callbacks_t cbs = {
        .on_color_trans_done = notify_lvgl_flush_ready,
    };

#pragma endregion

#pragma region LCD_INIT

    ESP_LOGI(LVGL_TASK_TAG, "Turn off LCD backlight");
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << PIN_NUM_LCD_BK_LIGHT};
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
    gpio_set_level(PIN_NUM_LCD_BK_LIGHT, LCD_BK_LIGHT_OFF_LEVEL);

    ESP_LOGI(LVGL_TASK_TAG, "Initialize SPI bus");
    spi_bus_config_t spi_bus_cfg = {
        .sclk_io_num = PIN_NUM_SCLK,
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_H_RES * 80 * sizeof(uint16_t),
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &spi_bus_cfg, SPI_DMA_CH_AUTO));

    ESP_LOGI(LVGL_TASK_TAG, "Install panel IO");
    esp_lcd_panel_io_handle_t lcd_io_handle = NULL;
    esp_lcd_panel_io_spi_config_t lcd_io_config = {
        .dc_gpio_num = PIN_NUM_LCD_DC,
        .cs_gpio_num = PIN_NUM_LCD_CS,
        .pclk_hz = LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &lcd_io_config, &lcd_io_handle));

    ESP_LOGI(LVGL_TASK_TAG, "Install ST7789 panel driver");
    esp_lcd_panel_handle_t lcd_handle = NULL;
    esp_lcd_panel_dev_config_t lcd_config = {
        .reset_gpio_num = PIN_NUM_LCD_RST,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
        .bits_per_pixel = 16,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(lcd_io_handle, &lcd_config, &lcd_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(lcd_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(lcd_handle));
    // ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, false));
    // user can flush pre-defined pattern to the screen before we turn on the screen or backlight
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(lcd_handle, true));
    gpio_set_level(PIN_NUM_LCD_BK_LIGHT, LCD_BK_LIGHT_ON_LEVEL);

    /* Register done callback */
    ESP_ERROR_CHECK(esp_lcd_panel_io_register_event_callbacks(lcd_io_handle, &cbs, display));

    lv_display_set_user_data(display, lcd_handle);

    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LCD_LEDC_MODE,
        .duty_resolution = LEDC_TIMER_13_BIT,
        .timer_num = LCD_LEDC_TIMER,
        .freq_hz = 4000, // Set output frequency at 4 kHz
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t lcd_ledc = {
        .gpio_num = PIN_NUM_LCD_BK_LIGHT,
        .channel = LCD_LEDC_CHANNEL,
        .speed_mode = LCD_LEDC_MODE,
        .timer_sel = LCD_LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .duty = 0,
        .hpoint = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&lcd_ledc));

#pragma endregion

#pragma region TOUCH_INIT

    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_CST816S_CONFIG();

    ESP_LOGI(LVGL_TASK_TAG, "Init i2c bus");
    i2c_master_bus_handle_t i2c_bus_handle;
    i2c_master_bus_config_t i2c_conf = {
        .i2c_port = TOUCH_I2C_NUM,
        .sda_io_num = PIN_NUM_TOUCH_SDA,
        .scl_io_num = PIN_NUM_TOUCH_SCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_conf, &i2c_bus_handle));

    ESP_LOGI(LVGL_TASK_TAG, "Init i2c touch panel device");
    i2c_master_dev_handle_t touch_dev_handle;
    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_10,
        .device_address = ESP_LCD_TOUCH_IO_I2C_CST816S_ADDRESS,
        .scl_speed_hz = tp_io_config.scl_speed_hz,
    };
    ESP_ERROR_CHECK(i2c_master_bus_add_device(i2c_bus_handle, &dev_config, &touch_dev_handle));

    ESP_LOGI(LVGL_TASK_TAG, "Install i2c touch panel driver");
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus_handle, &tp_io_config, &tp_io_handle));
    esp_lcd_touch_config_t tp_cfg = {
        .x_max = LCD_H_RES,
        .y_max = LCD_V_RES,
        .rst_gpio_num = PIN_NUM_TOUCH_RST,
        .int_gpio_num = PIN_NUM_TOUCH_INT,
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 0,
        },
    };
    esp_lcd_touch_handle_t tp_handle = NULL;
    ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_cst816s(tp_io_handle, &tp_cfg, &tp_handle));

    ESP_LOGI(LVGL_TASK_TAG, "Setup touch panel");
    static lv_indev_t *input_device;
    input_device = lv_indev_create(); // Input device driver (Touch)
    lv_indev_set_type(input_device, LV_INDEV_TYPE_POINTER);
    lv_indev_set_display(input_device, display);
    lv_indev_set_user_data(input_device, tp_handle);
    lv_indev_set_read_cb(input_device, lvgl_touch_cb);

#pragma endregion

    lv_log_register_print_cb(esp_log_cb);
    return display;
}

void set_brightness(uint8_t brightness)
{
    brightness = MAX(brightness, 10);     // minimum brightness is 10%
    float fbrightness = brightness / 255; // convert to percentage
    uint32_t duty = MIN((2 << 13) * fbrightness, (2 << 13) - 1);
    ESP_ERROR_CHECK(ledc_set_duty(LCD_LEDC_MODE, LCD_LEDC_CHANNEL, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LCD_LEDC_MODE, LCD_LEDC_CHANNEL));
}
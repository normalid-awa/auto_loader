#include "config.h"
#include "lvgl_init.h"

static void app_lcd_init(esp_lcd_panel_io_handle_t *lcd_io, esp_lcd_panel_handle_t *lcd_panel)
{
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << LCD_BL_GPIO};
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
    ESP_ERROR_CHECK(gpio_set_level(LCD_BL_GPIO, 0));

    const spi_bus_config_t buscfg = {
        .sclk_io_num = LCD_SCLK_GPIO,
        .mosi_io_num = LCD_MOSI_GPIO,
        .miso_io_num = GPIO_NUM_NC,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .max_transfer_sz = LCD_H_RES * LVGL_BUF_LINES * sizeof(uint16_t),
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO));

    const esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = LCD_DC_GPIO,
        .cs_gpio_num = LCD_CS_GPIO,
        .pclk_hz = LCD_SPI_FREQ,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_SPI_HOST, &io_config, lcd_io));

    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_RST_GPIO,
        .color_space = ESP_LCD_COLOR_SPACE_BGR,
        .bits_per_pixel = 16,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(*lcd_io, &panel_config, lcd_panel));

    esp_lcd_panel_reset(*lcd_panel);
    esp_lcd_panel_init(*lcd_panel);
    esp_lcd_panel_mirror(*lcd_panel, true, true);
    esp_lcd_panel_disp_on_off(*lcd_panel, true);

    // ESP_ERROR_CHECK(gpio_set_level(LCD_BL_GPIO, 1));

    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LCD_LEDC_RES,
        .timer_num = LCD_LEDC_TIMER,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t lcd_ch = {
        .gpio_num = LCD_BL_GPIO,
        .channel = LCD_LEDC_CH,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_sel = LCD_LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .duty = 4096,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&lcd_ch));
}

static void app_touch_init(esp_lcd_touch_handle_t *touch_handle)
{
    /* Initilize I2C */
    i2c_master_bus_handle_t i2c_handle = NULL;
    const i2c_master_bus_config_t i2c_config = {
        .i2c_port = TOUCH_I2C_CH,
        .sda_io_num = TOUCH_SDA_GPIO,
        .scl_io_num = TOUCH_SCL_GPIO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_config, &i2c_handle));

    /* Initialize touch HW */
    const esp_lcd_touch_config_t tp_cfg = {
        .x_max = LCD_H_RES,
        .y_max = LCD_V_RES,
        .rst_gpio_num = GPIO_NUM_NC,
        .int_gpio_num = TOUCH_INTR_GPIO,
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = false,
            .mirror_x = true,
            .mirror_y = true,
        },
    };
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    const esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_CST816S_CONFIG();
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_handle, &tp_io_config, &tp_io_handle));
    ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_cst816s(tp_io_handle, &tp_cfg, touch_handle));
}

static void app_lvgl_init(esp_lcd_panel_io_handle_t lcd_io, esp_lcd_panel_handle_t lcd_panel, esp_lcd_touch_handle_t touch_handle)
{
    static lv_display_t *lvgl_disp = NULL;
    static lv_indev_t *lvgl_touch_indev = NULL;

    /* Initialize LVGL */
    const lvgl_port_cfg_t lvgl_cfg = {
        .task_priority = 4,                     /* LVGL task priority */
        .task_stack = LVGL_TASK_STACK_SIZE,     /* LVGL task stack size */
        .task_affinity = -1,                    /* LVGL task pinned to core (-1 is no affinity) */
        .task_max_sleep_ms = 500,               /* Maximum sleep in LVGL task */
        .timer_period_ms = LVGL_TICK_PERIOD_MS, /* LVGL timer tick period in ms */
    };
    ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

    /* Add LCD screen */
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = lcd_io,
        .panel_handle = lcd_panel,
        .buffer_size = LCD_H_RES * LVGL_BUF_LINES,
        .double_buffer = true,
        .hres = LCD_H_RES,
        .vres = LCD_V_RES,
        .monochrome = false,
        .color_format = LV_COLOR_FORMAT_RGB565,
        .rotation = {
            .swap_xy = false,
            .mirror_x = true,
            .mirror_y = true,
        },
        .flags = {
            .buff_dma = true,
            .swap_bytes = true,
        },
    };
    lvgl_disp = lvgl_port_add_disp(&disp_cfg);

    /* Add touch input (for selected screen) */
    const lvgl_port_touch_cfg_t touch_cfg = {
        .disp = lvgl_disp,
        .handle = touch_handle,
    };
    lvgl_touch_indev = lvgl_port_add_touch(&touch_cfg);

    lv_display_set_default(lvgl_disp);
    lv_display_set_rotation(lvgl_disp, LV_DISPLAY_ROTATION_90);
}

void lvgl_init(void)
{
    static esp_lcd_panel_io_handle_t lcd_io = NULL;
    static esp_lcd_panel_handle_t lcd_panel = NULL;
    static esp_lcd_touch_handle_t touch_handle = NULL;

    app_lcd_init(&lcd_io, &lcd_panel);
    app_touch_init(&touch_handle);
    app_lvgl_init(lcd_io, lcd_panel, touch_handle);
}

/// @brief
/// @param brightness 0-1 (percentage)
void set_lcd_brightness(float brightness)
{
    brightness = MIN(MAX(brightness, 0.05f), 0.99f);
    int duty = (2 << (LCD_LEDC_RES - 1)) * brightness;
    uint32_t uduty = (uint32_t)duty;
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LCD_LEDC_CH, uduty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LCD_LEDC_CH));
}
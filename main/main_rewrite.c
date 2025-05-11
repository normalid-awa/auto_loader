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
#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"
#include "lvgl_private.h"

#include "esp_lcd_panel_st7789.h"
#include "esp_lcd_touch_cst816s.h"

#include "lvgl_init.c"
#include "lvgl_ui.h"

void app_main()
{
    lv_display_t *display = lvgl_init(&touch_handle);
    ESP_LOGI(LVGL_TASK_TAG, "Create LVGL task");
    xTaskCreate(lvgl_port_task, "LVGL", LVGL_TASK_STACK_SIZE, NULL, LVGL_TASK_PRIORITY, NULL);

    ESP_LOGI(LVGL_TASK_TAG, "Display LVGL Meter Widget");
    // Lock the mutex due to the LVGL APIs are not thread-safe
    _lock_acquire(&lvgl_api_lock);
    lv_disp_set_rotation(display, LV_DISPLAY_ROTATION_90);
    example_lvgl_demo_ui(display);
    _lock_release(&lvgl_api_lock);
}
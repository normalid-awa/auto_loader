#include <sys/lock.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_io.h"
#include "esp_log.h"

#include "lvgl.h"
#include "lvgl_private.h"
#include "lvgl_init.h"

#include "screens.h"
#include "ui.h"

void app_main()
{
    lv_display_t *display = lvgl_init();
    lv_disp_set_default(display);
    ESP_LOGI("LVGL_TASK_TAG", "Create LVGL task");
    // xTaskCreate(lvgl_port_task, "LVGL", LVGL_TASK_STACK_SIZE, NULL, LVGL_TASK_PRIORITY, NULL);
    xTaskCreatePinnedToCore(lvgl_port_task, "UI", LVGL_TASK_STACK_SIZE, NULL, LVGL_TASK_PRIORITY, NULL, 0);
    // Lock the mutex due to the LVGL APIs are not thread-safe
    lv_lock();
    lv_disp_set_rotation(display, LV_DISPLAY_ROTATION_90);
    ui_init();
    lv_unlock();
}
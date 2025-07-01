#include <stdio.h>
#include <sys/time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_attr.h"

#include "esp_task_wdt.h"

#include "iot_button.h"
#include "button_gpio.h"
#include "iot_servo.h"

#include "config.h"
#include "lvgl_init.h"

#include "actions.h"
#include "app_actions.hpp"
#include "ui.h"
#include "vars.h"
#include "screens.h"
#include "nvs-preferences.h"

static const char *ACTION_TAG = "Main";
static const char *AMMO_COUNTER_TAG = "Ammo Counter";
static const char *MAG_DETECTOR_TAG = "Mag Detector";

#pragma region "App Functions"

static int AMMO_COUNT = 0;

mcpwm_cmpr_handle_t motor_comparator = NULL;

void set_angle(int angle)
{
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(motor_comparator, (uint32_t)((angle - 0) * (2000 - 1000) / (180 - 0) + 1000)));
}

void setup_motor()
{
    mcpwm_timer_handle_t timer = NULL;
    mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = 1000000,
        .period_ticks = 20000,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &timer));

    mcpwm_oper_handle_t oper = NULL;
    mcpwm_operator_config_t operator_config = {
        .group_id = 0, // operator must be in the same group to the timer
    };
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &oper));

    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper, timer));

    mcpwm_comparator_config_t comparator_config = {
        .flags.update_cmp_on_tez = true,
    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comparator_config, &motor_comparator));

    mcpwm_gen_handle_t generator = NULL;
    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = SERVO_GPIO,
    };
    ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config, &generator));

    set_angle(90);

    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator,
                                                              MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator,
                                                                MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, motor_comparator, MCPWM_GEN_ACTION_LOW)));

    ESP_ERROR_CHECK(mcpwm_timer_enable(timer));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP));
}

static void on_mag_inserted(void *arg, void *usr_data)
{
    if (get_max_ammo() == -1 || AMMO_COUNT < get_max_ammo())
        set_angle((int)(90 - (90 * ((float)get_motor_force() / 100))));
    else
        set_angle(90);
}

static void on_mag_removed(void *arg, void *usr_data)
{
    set_angle(90);
    AMMO_COUNT = 0;
    set_current_ammo(AMMO_COUNT);
}

void setup_mag_detector()
{
    esp_rom_gpio_pad_select_gpio(MAG_DETECTOR_GPIO);
    gpio_set_direction(MAG_DETECTOR_GPIO, GPIO_MODE_INPUT);

    const button_config_t btn_cfg = {0};
    const button_gpio_config_t btn_gpio_cfg = {
        .gpio_num = MAG_DETECTOR_GPIO,
        .active_level = 1,
    };
    button_handle_t gpio_btn = NULL;
    esp_err_t ret = iot_button_new_gpio_device(&btn_cfg, &btn_gpio_cfg, &gpio_btn);
    if (NULL == gpio_btn)
    {
        ESP_LOGE(AMMO_COUNTER_TAG, "Button create failed");
    }
    iot_button_set_param(gpio_btn, BUTTON_LONG_PRESS_TIME_MS, (void *)20);
    iot_button_register_cb(gpio_btn, BUTTON_LONG_PRESS_HOLD, NULL, on_mag_inserted, NULL);
    iot_button_register_cb(gpio_btn, BUTTON_PRESS_UP, NULL, on_mag_removed, NULL);
}

static void inc_ammo_count(void *arg, void *usr_data)
{
    AMMO_COUNT++;
    set_current_ammo(AMMO_COUNT);
}

void setup_ammo_counter()
{
    esp_rom_gpio_pad_select_gpio(AMMO_COUNTER_GPIO);
    gpio_set_direction(AMMO_COUNTER_GPIO, GPIO_MODE_INPUT);

    const button_config_t btn_cfg = {0};
    const button_gpio_config_t btn_gpio_cfg = {
        .gpio_num = AMMO_COUNTER_GPIO,
        .active_level = 1,
    };
    button_handle_t gpio_btn = NULL;
    esp_err_t ret = iot_button_new_gpio_device(&btn_cfg, &btn_gpio_cfg, &gpio_btn);
    if (NULL == gpio_btn)
    {
        ESP_LOGE(AMMO_COUNTER_TAG, "Button create failed");
    }
    iot_button_set_param(gpio_btn, BUTTON_SHORT_PRESS_TIME_MS, (void *)50);
    iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, NULL, inc_ammo_count, NULL);
}

#pragma endregion

void app_tick()
{
    const esp_task_wdt_config_t wdt_config = {
        .timeout_ms = LVGL_TICK_PERIOD_MS,
    };
    esp_task_wdt_reconfigure(&wdt_config);
    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(LVGL_TICK_PERIOD_MS));
        lvgl_port_lock(0);
        ui_tick();
        lvgl_port_unlock();
    }
}

void action_update_max_ammo(lv_event_t *e)
{
    lvgl_port_lock(0);
    lv_obj_t *obj = lv_event_get_target_obj(e);
    int amount = (int)lv_event_get_user_data(e);
    int action = (int)lv_buttonmatrix_get_selected_button(obj);

    int value = get_max_ammo();

    switch (action)
    {
    case Increase:
        value += amount;
        break;
    case Decrease:
        if (value - amount >= 1)
            value -= amount;
        break;
    case Infinate:
        value = -1;
    default:
        break;
    }
    set_max_ammo(value);
    lvgl_port_unlock();
}

void action_update_dark_mode(lv_event_t *e)
{
    lvgl_port_lock(0);
    lv_obj_t *obj = lv_event_get_target_obj(e);
    bool dark_mode = lv_obj_has_state(obj, LV_STATE_CHECKED);
    set_dark_mode(dark_mode);
    lvgl_port_unlock();
}

void action_update_brightness(lv_event_t *e)
{
    lvgl_port_lock(0);
    lv_obj_t *obj = lv_event_get_target_obj(e);
    int value = lv_slider_get_value(obj);
    set_brightness(value);
    lvgl_port_unlock();
}

void action_update_motor_force(lv_event_t *e)
{
    lvgl_port_lock(0);
    lv_obj_t *obj = lv_event_get_target_obj(e);
    int value = lv_slider_get_value(obj);
    set_motor_force(value);
    lvgl_port_unlock();
}

void action_move_motor(lv_event_t *e)
{
    lvgl_port_lock(0);
    lv_obj_t *obj = lv_event_get_target_obj(e);
    int direction = (int)lv_event_get_user_data(e);
    lvgl_port_unlock();

    set_angle((int)(90 + direction * (90 * ((float)get_motor_force() / 100))));
    vTaskDelay(pdMS_TO_TICKS(100));
    set_angle(90);
}

void load_preferences()
{
    uint32_t brightness = 255;
    load_u32("brightness", &brightness);
    set_brightness(brightness);

    bool dark_mode = true;
    load_bool("dark_mode", &dark_mode);
    set_dark_mode(dark_mode);

    uint32_t motor_force = 80;
    load_u32("motor_force", &motor_force);
    set_motor_force(motor_force);
}

void app_main()
{
    setup_motor();
    setup_mag_detector();
    setup_ammo_counter();
    ESP_ERROR_CHECK(setup_nvs_preferences());

    lvgl_init();

    lvgl_port_lock(0);
    ui_init();
    lvgl_port_unlock();

    load_preferences();
    xTaskCreate(app_tick, "app_tick", 4096, NULL, 1, NULL);
}

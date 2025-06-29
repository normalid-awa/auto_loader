#include "app_actions.hpp"

#include "lvgl.h"
#include "lvgl_init.h"

#include "esp_log.h"

#include "eez-flow.h"
#include "structs.h"
#include "vars.h"
#include "ui.h"
#include "vars.h"
#include "screens.h"

#include "config.h"
#include "nvs-preferences.h"

static const char *ACTION_TAG = "App action";

int get_max_ammo()
{
    return flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_MAX_AMMO).getInt();
}
void set_max_ammo(int ammo)
{
    flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_MAX_AMMO, IntegerValue((int32_t)ammo));
}

int get_current_ammo()
{
    return flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_CURRENT_AMMO).getInt();
}
void set_current_ammo(int ammo)
{
    flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_CURRENT_AMMO, IntegerValue((int32_t)ammo));
}

bool get_dark_mode()
{
    return flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_DARK_MODE).getBoolean();
}
void set_dark_mode(bool dark_mode)
{
    flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_DARK_MODE, BooleanValue(dark_mode));
    lvgl_port_lock(0);
    lv_disp_t *disp = lv_display_get_default();
    lv_theme_t *theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), !dark_mode, LV_FONT_DEFAULT);
    lv_display_set_theme(disp, theme);
    lvgl_port_unlock();
    ESP_LOGD(ACTION_TAG, "Set dark_mode: %d", dark_mode);
    save_bool("dark_mode", dark_mode);
}

int get_brightness()
{
    return flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_BRIGHTNESS).getInt();
}
void set_brightness(int brightness)
{
    flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_BRIGHTNESS, IntegerValue((int32_t)brightness));
    lvgl_port_lock(0);
    ESP_LOGD(ACTION_TAG, "Set brightness: %d", brightness);
    set_lcd_brightness(brightness / 255.0f);
    save_u32("brightness", brightness);
    lvgl_port_unlock();
}

int get_motor_force()
{
    return flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_MOTOR_FORCE).getInt();
}
void set_motor_force(int force)
{
    flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_MOTOR_FORCE, IntegerValue((int32_t)force));
    ESP_LOGD(ACTION_TAG, "Set motor force: %d", force);
    save_u32("motor_force", force);
}
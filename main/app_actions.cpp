#include "app_actions.hpp"
#include "esp_log.h"
#include "lvgl_init.h"
#include "eez-flow.h"
#include "structs.h"
#include "vars.h"
#include "ui.h"
#include "vars.h"
#include "screens.h"

int get_max_ammo()
{
    return flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_MAX_AMMO).getInt();
}
void set_max_ammo(int ammo)
{
    flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_MAX_AMMO, IntegerValue(ammo));
}

int get_current_ammo()
{
    return flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_CURRENT_AMMO).getInt();
}
void set_current_ammo(int ammo)
{
    flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_CURRENT_AMMO, IntegerValue(ammo));
}

bool get_dark_mode()
{
    return flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_DARK_MODE).getBoolean();
}
void set_dark_mode(bool dark_mode)
{
    flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_DARK_MODE, BooleanValue(dark_mode));
}

int get_brightness()
{
    return flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_BRIGHTNESS).getInt();
}
void set_brightness(int brightness)
{
    flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_BRIGHTNESS, IntegerValue(brightness));
}

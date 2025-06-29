#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations



// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_CURRENT_AMMO = 0,
    FLOW_GLOBAL_VARIABLE_MAX_AMMO = 1,
    FLOW_GLOBAL_VARIABLE_DARK_MODE = 2,
    FLOW_GLOBAL_VARIABLE_BRIGHTNESS = 3,
    FLOW_GLOBAL_VARIABLE_MOTOR_FORCE = 4
};

// Native global variables



#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/
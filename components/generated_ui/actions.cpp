#include "vars.h"
#include "screens.h"
#include "structs.h"
#include "ui.h"
#include "eez-flow.h"
#include <lvgl.h>
#include <esp_log.h>
#include <lvgl_init.h>

enum ButtonMatrixIds
{
    INCREAMENT = 0,
    DECREAMENT = 1,
    INFINITE = 2,
};

extern "C" void action_button_matrix_clicked(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target_obj(e);
    uint32_t id = lv_buttonmatrix_get_selected_button(obj);
    int value = flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_MAX).getInt();

    switch (id)
    {
    case INCREAMENT:
        value += 1;
        break;
    case DECREAMENT:
        if (value > 0)
            value -= 1;
        break;
    case INFINITE:
        value = -1;
        break;
    };

    flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_MAX, IntegerValue(value));
}

extern "C" void action_button_matrix_long_pressed(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target_obj(e);
    uint32_t id = lv_buttonmatrix_get_selected_button(obj);
    int value = flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_MAX).getInt();

    switch (id)
    {
    case INCREAMENT:
        value += 1;
        break;
    case DECREAMENT:
        if (value > 0)
            value -= 1;
        break;
    case INFINITE:
        value = -1;
        break;
    };

    flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_MAX, IntegerValue(value));
}

extern "C" void action_toggle_dark_mode(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target_obj(e);
    bool dark_mode = lv_obj_has_state(obj, LV_STATE_CHECKED);
    flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_DARK_MODE, BooleanValue(dark_mode));
    lv_disp_t *disp = lv_display_get_default();
    lv_theme_t *theme = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), dark_mode, LV_FONT_DEFAULT);
    lv_display_set_theme(disp, theme);
}

extern "C" void action_brightness_changed(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target_obj(e);
    int32_t value = lv_slider_get_value(slider);
    flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_BRIGHTNESS, IntegerValue(value));
    // set_brightness(value);
}
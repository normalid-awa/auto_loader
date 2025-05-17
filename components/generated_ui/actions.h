#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_button_matrix_clicked(lv_event_t * e);
extern void action_button_matrix_long_pressed(lv_event_t * e);
extern void action_toggle_dark_mode(lv_event_t * e);
extern void action_brightness_changed(lv_event_t * e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/
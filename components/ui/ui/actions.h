#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_update_max_ammo(lv_event_t * e);
extern void action_update_dark_mode(lv_event_t * e);
extern void action_update_brightness(lv_event_t * e);
extern void action_update_motor_force(lv_event_t * e);
extern void action_move_motor(lv_event_t * e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/
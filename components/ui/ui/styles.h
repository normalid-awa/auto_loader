#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: setting_item
lv_style_t *get_style_setting_item_MAIN_DEFAULT();
void add_style_setting_item(lv_obj_t *obj);
void remove_style_setting_item(lv_obj_t *obj);

// Style: tab_style
lv_style_t *get_style_tab_style_MAIN_DEFAULT();
void add_style_tab_style(lv_obj_t *obj);
void remove_style_tab_style(lv_obj_t *obj);

// Style: setting_slider_style
lv_style_t *get_style_setting_slider_style_MAIN_DEFAULT();
lv_style_t *get_style_setting_slider_style_INDICATOR_DEFAULT();
lv_style_t *get_style_setting_slider_style_KNOB_DEFAULT();
void add_style_setting_slider_style(lv_obj_t *obj);
void remove_style_setting_slider_style(lv_obj_t *obj);



#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/
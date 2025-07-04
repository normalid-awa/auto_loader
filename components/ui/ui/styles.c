#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: setting_item
//

void init_style_setting_item_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_layout(style, LV_LAYOUT_FLEX);
    lv_style_set_flex_flow(style, LV_FLEX_FLOW_COLUMN);
    lv_style_set_flex_main_place(style, LV_FLEX_ALIGN_SPACE_BETWEEN);
    lv_style_set_flex_cross_place(style, LV_FLEX_ALIGN_CENTER);
    lv_style_set_flex_track_place(style, LV_FLEX_ALIGN_CENTER);
    lv_style_set_pad_left(style, 5);
    lv_style_set_pad_right(style, 5);
    lv_style_set_radius(style, 20);
    lv_style_set_margin_bottom(style, 5);
};

lv_style_t *get_style_setting_item_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_setting_item_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_setting_item(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_setting_item_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_setting_item(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_setting_item_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: tab_style
//

void init_style_tab_style_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_pad_left(style, 0);
    lv_style_set_pad_bottom(style, 0);
    lv_style_set_pad_top(style, 0);
    lv_style_set_pad_right(style, 0);
    lv_style_set_margin_left(style, 30);
    lv_style_set_margin_right(style, 30);
};

lv_style_t *get_style_tab_style_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_tab_style_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_tab_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_tab_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_tab_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_tab_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: setting_slider_style
//

void init_style_setting_slider_style_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_flex_grow(style, 1);
};

lv_style_t *get_style_setting_slider_style_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_setting_slider_style_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_setting_slider_style_INDICATOR_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_color(style, lv_color_hex(0xff2196f3));
    lv_style_set_bg_grad_color(style, lv_color_hex(theme_colors[eez_flow_get_selected_theme_index()][0]));
    lv_style_set_bg_grad_dir(style, LV_GRAD_DIR_HOR);
};

lv_style_t *get_style_setting_slider_style_INDICATOR_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_setting_slider_style_INDICATOR_DEFAULT(style);
    }
    return style;
};

void init_style_setting_slider_style_KNOB_DEFAULT(lv_style_t *style) {
    lv_style_set_opa(style, 0);
};

lv_style_t *get_style_setting_slider_style_KNOB_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = lv_malloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_setting_slider_style_KNOB_DEFAULT(style);
    }
    return style;
};

void add_style_setting_slider_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_setting_slider_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_setting_slider_style_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_setting_slider_style_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
};

void remove_style_setting_slider_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_setting_slider_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_setting_slider_style_INDICATOR_DEFAULT(), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_setting_slider_style_KNOB_DEFAULT(), LV_PART_KNOB | LV_STATE_DEFAULT);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_setting_item,
        add_style_tab_style,
        add_style_setting_slider_style,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_setting_item,
        remove_style_tab_style,
        remove_style_setting_slider_style,
    };
    remove_style_funcs[styleIndex](obj);
}


#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;

static void event_handler_cb_main_obj0(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    void *flowState = lv_event_get_user_data(e);
    (void)flowState;
    
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target(e);
        if (tick_value_change_obj != ta) {
            int32_t value = lv_arc_get_value(ta);
            assignIntegerProperty(flowState, 5, 4, value, "Failed to assign Value in Arc widget");
        }
    }
}

void create_screen_main() {
    void *flowState = getFlowState(0, 0);
    (void)flowState;
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_tabview_create(parent_obj);
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_PCT(100), 240);
            lv_tabview_set_tab_bar_position(obj, LV_DIR_TOP);
            lv_tabview_set_tab_bar_size(obj, 32);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Home tab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Home");
                    objects.home_tab = obj;
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    lv_obj_set_pos(obj, 0, 16);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_PCT(100));
                                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_right(obj, 10, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_left(obj, 10, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_bottom(obj, 10, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_top(obj, 10, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                    lv_obj_set_style_margin_bottom(obj, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                    lv_obj_set_style_margin_left(obj, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                    lv_obj_set_style_margin_right(obj, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                    lv_obj_set_style_margin_top(obj, 0, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            lv_obj_t *obj = lv_arc_create(parent_obj);
                                            objects.obj0 = obj;
                                            lv_obj_set_pos(obj, 0, 0);
                                            lv_obj_set_size(obj, 150, 150);
                                            lv_arc_set_range(obj, 0, 100);
                                            lv_arc_set_bg_start_angle(obj, 0);
                                            lv_arc_set_bg_end_angle(obj, 359);
                                            lv_arc_set_rotation(obj, -90);
                                            lv_obj_add_event_cb(obj, event_handler_cb_main_obj0, LV_EVENT_ALL, flowState);
                                            lv_obj_set_style_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
                                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                        }
                                        {
                                            // current text
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.current_text = obj;
                                            lv_obj_set_pos(obj, LV_PCT(-45), LV_PCT(35));
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_38, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text(obj, "");
                                        }
                                        {
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            lv_obj_set_pos(obj, LV_PCT(52), LV_PCT(0));
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text(obj, "/");
                                        }
                                        {
                                            // max text
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.max_text = obj;
                                            lv_obj_set_pos(obj, LV_PCT(58), LV_PCT(52));
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_obj_set_style_align(obj, LV_ALIGN_TOP_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
                                            lv_label_set_text(obj, "");
                                        }
                                    }
                                }
                                {
                                    lv_obj_t *obj = lv_buttonmatrix_create(parent_obj);
                                    lv_obj_set_pos(obj, 207, 0);
                                    lv_obj_set_size(obj, 90, LV_PCT(100));
                                    static const char *map[6] = {
                                        "+",
                                        "\n",
                                        "-",
                                        "\n",
                                        "INF",
                                        NULL,
                                    };
                                    lv_buttonmatrix_set_map(obj, map);
                                    lv_obj_set_style_layout(obj, LV_LAYOUT_NONE, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_ITEMS | LV_STATE_DEFAULT);
                                }
                            }
                        }
                    }
                }
                {
                    // Setting tab
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Setting");
                    objects.setting_tab = obj;
                }
            }
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
    void *flowState = getFlowState(0, 0);
    (void)flowState;
    {
        int32_t new_val = evalIntegerProperty(flowState, 5, 3, "Failed to evaluate Range max in Arc widget");
        int32_t cur_val = lv_arc_get_max_value(objects.obj0);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj0;
            int16_t min = lv_arc_get_min_value(objects.obj0);
            int16_t max = new_val;
            if (min < max) {
                lv_arc_set_range(objects.obj0, min, max);
            }
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = evalIntegerProperty(flowState, 5, 4, "Failed to evaluate Value in Arc widget");
        int32_t cur_val = lv_arc_get_value(objects.obj0);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.obj0;
            lv_arc_set_value(objects.obj0, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 6, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.current_text);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.current_text;
            lv_label_set_text(objects.current_text, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = evalTextProperty(flowState, 8, 3, "Failed to evaluate Text in Label widget");
        const char *cur_val = lv_label_get_text(objects.max_text);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.max_text;
            lv_label_set_text(objects.max_text, new_val);
            tick_value_change_obj = NULL;
        }
    }
}


static const char *screen_names[] = { "Main" };
static const char *object_names[] = { "main", "home_tab", "obj0", "current_text", "max_text", "setting_tab" };


typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    eez_flow_init_screen_names(screen_names, sizeof(screen_names) / sizeof(const char *));
    eez_flow_init_object_names(object_names, sizeof(object_names) / sizeof(const char *));
    
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
}

/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

// This demo UI is adapted from LVGL official example: https://docs.lvgl.io/master/examples.html#loader-with-arc

#include "lvgl_ui.h"

void example_lvgl_demo_ui(lv_display_t *disp)
{
    lv_obj_t *scr = lv_display_get_screen_active(disp);

    lv_obj_t *tabview = lv_tabview_create(scr);

    lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "Tab 1");

    static lv_style_t style_hidden;
    lv_style_init(&style_hidden);
    lv_style_set_opa(&style_hidden, 0);

    lv_obj_t *arc1 = lv_arc_create(tab1);
    lv_obj_set_size(arc1, 150, 150);
    lv_obj_align(arc1, LV_ALIGN_LEFT_MID, 0, 0);
    lv_arc_set_angles(arc1, 0, 360 - 1);
    lv_obj_add_style(arc1, &style_hidden, LV_PART_KNOB);
    lv_obj_center(arc1);

    lv_obj_t *current_text = lv_label_create(tab1);
    lv_label_set_text(current_text, "110");
    lv_obj_align(current_text, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_set_pos(current_text, -236, 58);
    lv_obj_set_style_text_font(current_text, &lv_font_montserrat_32, LV_PART_MAIN);

    lv_obj_t *divider = lv_label_create(tab1);
    lv_label_set_text(divider, "/");
    lv_obj_align(divider, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_pos(divider, 75, 0);
    lv_obj_set_style_text_font(divider, &lv_font_montserrat_48, LV_PART_MAIN);

    lv_obj_t *max_text = lv_label_create(tab1);
    lv_label_set_text(max_text, "100");
    lv_obj_align(max_text, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_pos(max_text, 84, 96);
    lv_obj_set_style_text_font(max_text, &lv_font_montserrat_18, LV_PART_MAIN);
}

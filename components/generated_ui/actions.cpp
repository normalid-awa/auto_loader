#include "vars.h"
#include "screens.h"
#include "structs.h"
#include "ui.h"
#include "eez-flow.h"
#include <lvgl.h>
#include <esp_log.h>

const static char *TAG = "ACTION";

extern "C" void action_button_matrix_clicked(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target_obj(e);
    uint32_t id = lv_buttonmatrix_get_selected_button(obj);
    int value = flow::getGlobalVariable(FLOW_GLOBAL_VARIABLE_MAX).getInt();

    switch (id)
    {
    case 0:
        value += 1;
        break;
    case 1:
        if (value > 0)
            value -= 1;
        break;
    case 2:
        value = -1;
        break;
    };

    flow::setGlobalVariable(FLOW_GLOBAL_VARIABLE_MAX, IntegerValue(value));
}
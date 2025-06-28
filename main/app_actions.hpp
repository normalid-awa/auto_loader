#include "lvgl.h"

#ifndef APP_ACTIONS_H
#define APP_ACTIONS_H

#ifdef __cplusplus
extern "C"
{
#endif

    enum ButtonMatrixEvent
    {
        Increase = 0,
        Decrease = 1,
        Infinate = 2,
    };

    int get_max_ammo();
    void set_max_ammo(int ammo);

    int get_current_ammo();
    void set_current_ammo(int ammo);

    bool get_dark_mode();
    void set_dark_mode(bool dark_mode);

    int get_brightness();
    void set_brightness(int brightness);

#ifdef __cplusplus
}
#endif

#endif // APP_ACTIONS_H
#ifndef LVGL_INIT_H
#define LVGL_INIT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void lvgl_init(void);
    void set_lcd_brightness(float brightness);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LVGL_INIT_H */
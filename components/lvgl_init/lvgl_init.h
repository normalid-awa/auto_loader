#ifndef LVGL_INIT_H_
#define LVGL_INIT_H_
#include "lvgl.h"
#include "lvgl_private.h"

#define LCD_HOST SPI2_HOST
#define LCD_PIXEL_CLOCK_HZ (20 * 1000 * 1000) // 20MHz
#define LCD_BK_LIGHT_ON_LEVEL 1
#define LCD_BK_LIGHT_OFF_LEVEL 0

#define PIN_NUM_MISO -1

#define PIN_NUM_LCD_RST GPIO_NUM_15
#define PIN_NUM_MOSI GPIO_NUM_16
#define PIN_NUM_SCLK GPIO_NUM_17
#define PIN_NUM_LCD_CS GPIO_NUM_18
#define PIN_NUM_LCD_DC GPIO_NUM_8
#define PIN_NUM_LCD_BK_LIGHT GPIO_NUM_3
#define LCD_LEDC_MODE LEDC_LOW_SPEED_MODE
#define LCD_LEDC_CHANNEL LEDC_CHANNEL_0
#define LCD_LEDC_TIMER LEDC_TIMER_0
#define LCD_BL_PWM_FREQ 5000

#define PIN_NUM_TOUCH_INT GPIO_NUM_4
#define PIN_NUM_TOUCH_RST GPIO_NUM_5
#define PIN_NUM_TOUCH_SDA GPIO_NUM_6
#define PIN_NUM_TOUCH_SCL GPIO_NUM_7
#define TOUCH_I2C_NUM I2C_NUM_0
#define TOUCH_I2C_HZ 100000

// The pixel number in horizontal and vertical
#define LCD_H_RES 240
#define LCD_V_RES 320

// Bit number used to represent command and parameter
#define LCD_CMD_BITS 8
#define LCD_PARAM_BITS 8

#define LVGL_DRAW_BUF_LINES 140 // number of display lines in each draw buffer
#define LVGL_TICK_PERIOD_MS 5
#define LVGL_TASK_MAX_DELAY_MS 500
#define LVGL_TASK_MIN_DELAY_MS 1
#define LVGL_TASK_STACK_SIZE (16 * 1024)
#define LVGL_TASK_PRIORITY 2

void lvgl_port_task(void *arg);
lv_display_t *lvgl_init();

extern void (*lvgl_tick_callback)();

/**
 * @brief Set the brightness level of the backlight.
 * @param level The brightness level, range from 0 to 255.
 */
void set_brightness(uint8_t brightness);

#endif // LVGL_INIT_H_
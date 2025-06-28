#include "lvgl.h"
#include "esp_lvgl_port.h"

#include "esp_timer.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_check.h"

#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "driver/i2c.h"
#include "driver/spi_master.h"
#include "driver/ledc.h"
#include "driver/mcpwm_prelude.h"

#include "esp_lcd_io_spi.h"
#include "esp_lcd_io_i2c.h"
#include "esp_lcd_panel_dev.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_st7789.h"
#include "esp_lcd_touch_cst816s.h"

#include <sys/param.h>

#ifndef GPIO_H
#define GPIO_H

#define MAG_DETECTOR_GPIO GPIO_NUM_41
#define AMMO_COUNTER_GPIO GPIO_NUM_40
#define SERVO_GPIO GPIO_NUM_2

#define TOUCH_INTR_GPIO GPIO_NUM_4
#define TOUCH_RST_GPIO GPIO_NUM_5
#define TOUCH_SDA_GPIO GPIO_NUM_6
#define TOUCH_SCL_GPIO GPIO_NUM_7
#define TOUCH_I2C_FREQ 400000
#define TOUCH_I2C_CH I2C_NUM_0

#define LCD_RST_GPIO GPIO_NUM_15
#define LCD_MOSI_GPIO GPIO_NUM_16
#define LCD_SCLK_GPIO GPIO_NUM_17
#define LCD_CS_GPIO GPIO_NUM_18
#define LCD_DC_GPIO GPIO_NUM_8
#define LCD_BL_GPIO GPIO_NUM_3
#define LCD_SPI_HOST SPI2_HOST
#define LCD_SPI_FREQ 40 * 1000 * 1000

#define LCD_LEDC_CH LEDC_CHANNEL_0
#define LCD_LEDC_TIMER LEDC_TIMER_0
#define LCD_LEDC_RES LEDC_TIMER_10_BIT

#define LCD_H_RES 240
#define LCD_V_RES 320

#define LVGL_BUF_LINES 40
#define LVGL_TICK_PERIOD_MS 5
#define LVGL_TASK_STACK_SIZE 8 * 1024

#endif // GPIO_H
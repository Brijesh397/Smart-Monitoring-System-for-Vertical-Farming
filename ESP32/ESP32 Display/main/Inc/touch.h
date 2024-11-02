#ifndef TOUCH_H_
#define TOUCH_H_

/***************************INCLUDES******************************/
#include <stdio.h>
#include "sdkconfig.h"
#include "esp_timer.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "lvgl.h"
#include "lvglInit.h"
#include "lcd.h"
#include "main.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_touch_gt911.h"

extern esp_lcd_touch_handle_t touch_handle;

/************************FUNCTION PROTOTYPE************************/
void touchInit(void);
void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);

#endif /* MAIN_INC_TOUCH_H_ */

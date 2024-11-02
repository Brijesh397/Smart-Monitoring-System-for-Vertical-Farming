#ifndef LVGLINIT_H_
#define LVGLINIT_H_

/***************************INCLUDES******************************/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"
#include "lcd.h"
#include "main.h"
#include "touch.h"

/******************************MACROS***********************************/
#define EXAMPLE_LVGL_TICK_PERIOD_MS    	2
#define EXAMPLE_LVGL_TASK_MAX_DELAY_MS 	500
#define EXAMPLE_LVGL_TASK_MIN_DELAY_MS 	1
#define LVGL_TASK_STACK_SIZE   			(4 * 1024)
#define LVGL_TASK_PRIORITY     			2

/*****************************EXTERN VARIABLES******************************/
extern lv_disp_t *disp;
extern lv_disp_draw_buf_t disp_buf; // contains internal graphic buffer(s) called draw buffer(s)
extern lv_disp_drv_t disp_drv;      // contains callback functions

/*************FUNCTION PROTOTYPE*******************************/
void lvglInit(void);


#endif






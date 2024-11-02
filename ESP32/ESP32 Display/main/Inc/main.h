#ifndef MAIN_H_
#define MAIN_H_

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
#include "lvglInit.h"
#include "lcd.h"
#include "touch.h"
#include "lv_demos.h"
#include "lv_examples.h"
#include "gui.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_http_client.h"
#include "esp_netif.h"
#include "esp_tls.h"
#include "esp_system.h"
#include <string.h>
#include <sys/param.h>
#include <stdlib.h>
#include <ctype.h>
#include "jpeg_decoder.h"

/**********************MACROS*******************************/
#define WIFI_AUTHMODE 		WIFI_AUTH_WPA2_PSK
#define WIFI_CONNECTED_BIT 	BIT0
#define WIFI_FAIL_BIT 		BIT1
#define WIFI_SSID			"Embedded Wizard"
#define WIFI_PASSWORD		"Embeddedwizard@1416c"
#define MAX_HTTP_RECV_BUFFER 	10000
#define MAX_HTTP_OUTPUT_BUFFER 	10000

extern esp_http_client_handle_t client;
/*****************************EXTERN VARIABLES******************************/
extern const char *TAG;
extern bool httpFinish;

/************************FUNCTION PROTOTYPE************************/
bool example_lvgl_lock(int timeout_ms);
void example_lvgl_unlock(void);
void lvglTask(void *arg);
void wifiTask(void *arg);

esp_err_t wifiInit(void);
esp_err_t wifiConnect(char* wifi_ssid, char* wifi_password);
esp_err_t wifiDisconnect(void);
esp_err_t wifiDeinit(void);

#endif /* MAIN_MAIN_H_ */

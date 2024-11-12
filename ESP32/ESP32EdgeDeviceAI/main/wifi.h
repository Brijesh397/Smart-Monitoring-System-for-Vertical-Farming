#ifndef WIFI_H
#define WIFI_H

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/param.h>
#include <stdlib.h>
#include <ctype.h>
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_http_client.h"
#include "esp_netif.h"
#include "esp_tls.h"
#include "esp_system.h"
#include "esp_err.h"
#include "esp_log.h"
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

extern const char *TAG;

extern bool wifiConnected;
extern bool httpFinish;

esp_err_t _http_event_handler(esp_http_client_event_t *evt);
esp_err_t wifiInit(void);
esp_err_t wifiConnect();
esp_err_t wifiDisconnect(void);
esp_err_t wifiDeinit(void);

#endif /* MAIN_WIFI_H_ */

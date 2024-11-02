/***************************INCLUDES******************************/
#include "main.h"
#include "draw/lv_img_buf.h"
#include "freertos/projdefs.h"
#include "gui.h"

/*****************************EXTERN VARIABLES******************************/
const char *TAG = "DISPLAY";

/*************************GLOBAL VARIABLES************************/
static SemaphoreHandle_t lvgl_mux = NULL;
TaskHandle_t wifiTaskHandle = NULL;
bool wifiConnected = false;
bool httpFinish = false;

//Wifi
static const int WIFI_RETRY_ATTEMPT = 3;
static int wifi_retry_count = 0;

static esp_netif_t *tutorial_netif = NULL;
static esp_event_handler_instance_t ip_event_handler;
static esp_event_handler_instance_t wifi_event_handler;

static EventGroupHandle_t s_wifi_event_group = NULL;
static void capture_image_from_esp32_cam(void);
esp_err_t _http_event_handler(esp_http_client_event_t *evt);

esp_http_client_handle_t client;

//Buffer to store Camera Image
char cameraBuffer[10000] = {0};
uint8_t *out_img_buf = NULL;

/************************MAIN***************************************/
void app_main(void)
{
	ESP_LOGI(TAG, "Initialising Wifi...");
    ESP_ERROR_CHECK(wifiInit());
	
    //Create Mutex
    lvgl_mux = xSemaphoreCreateRecursiveMutex();
    assert(lvgl_mux);
    
    //Create Task
    ESP_LOGI(TAG, "Create LVGL task");
    xTaskCreate(lvglTask, "LVGL", LVGL_TASK_STACK_SIZE, NULL, LVGL_TASK_PRIORITY, NULL);
	
	ESP_LOGI(TAG, "Create Wifi task");
	xTaskCreate(wifiTask, "wifiTask", 14096, NULL, 2, &wifiTaskHandle);
	

}

/***********************LVGL TASK*******************************/
void lvglTask(void *arg)
{
    ESP_LOGI(TAG, "Starting LVGL task");
    uint32_t task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
    
   	lcdInit();
    touchInit();
    lvglInit();
    
    // Lock the mutex due to the LVGL APIs are not thread-safe
    if (example_lvgl_lock(-1)) {
        createDashboardScreen();
        // Release the mutex
        example_lvgl_unlock();
    }
    
    while (1) {
        // Lock the mutex due to the LVGL APIs are not thread-safe
        if (example_lvgl_lock(-1)) {
            task_delay_ms = lv_timer_handler();
            // Release the mutex
            example_lvgl_unlock();
        }
        if (task_delay_ms > EXAMPLE_LVGL_TASK_MAX_DELAY_MS) {
            task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
        } else if (task_delay_ms < EXAMPLE_LVGL_TASK_MIN_DELAY_MS) {
            task_delay_ms = EXAMPLE_LVGL_TASK_MIN_DELAY_MS;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/***********************WIFI TASK*******************************/
void wifiTask(void *arg)
{ 
    esp_err_t ret = wifiConnect(WIFI_SSID, WIFI_PASSWORD);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to connect to Wi-Fi network");
    }

    while(1)
    {
		if(wifiConnected)
		{
			if((activeScreen == ANALYTICSSCREEN) && (activeScreen != DASHBOARDSCREEN))
			{
				capture_image_from_esp32_cam();
			}
			
		}
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}


/***********************FUNCTION DEFINITION*******************************/
bool example_lvgl_lock(int timeout_ms)
{
    // Convert timeout in milliseconds to FreeRTOS ticks
    // If `timeout_ms` is set to -1, the program will block until the condition is met
    const TickType_t timeout_ticks = (timeout_ms == -1) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xSemaphoreTakeRecursive(lvgl_mux, timeout_ticks) == pdTRUE;
}

void example_lvgl_unlock(void)
{
    xSemaphoreGiveRecursive(lvgl_mux);
}

/*************************WIFI**************************88*/
esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    static char *output_buffer;  // Buffer to store response of http request from event handler
    static int output_len;       // Stores number of bytes read
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            // Clean the buffer in case of a new request
            if (output_len == 0 && evt->user_data) {
                // we are just starting to copy the output data into the use
                memset(evt->user_data, 0, MAX_HTTP_OUTPUT_BUFFER);
            }
            /*
             *  Check for chunked encoding is added as the URL for chunked encoding used in this example returns binary data.
             *  However, event handler can also be used in case chunked encoding is used.
             */
            if (!esp_http_client_is_chunked_response(evt->client)) {
                // If user_data buffer is configured, copy the response into the buffer
                int copy_len = 0;
                if (evt->user_data) {
                    // The last byte in evt->user_data is kept for the NULL character in case of out-of-bound access.
                    copy_len = MIN(evt->data_len, (MAX_HTTP_OUTPUT_BUFFER - output_len));
                    if (copy_len) {
                        memcpy(evt->user_data + output_len, evt->data, copy_len);
                    }
                } else {
                    int content_len = esp_http_client_get_content_length(evt->client);
                    if (output_buffer == NULL) {
                        // We initialize output_buffer with 0 because it is used by strlen() and similar functions therefore should be null terminated.
                        output_buffer = (char *) calloc(content_len + 1, sizeof(char));
                        output_len = 0;
                        if (output_buffer == NULL) {
                            ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                            return ESP_FAIL;
                        }
                    }
                    copy_len = MIN(evt->data_len, (content_len - output_len));
                    if (copy_len) {
                        memcpy(output_buffer + output_len, evt->data, copy_len);
                    }
                }
                output_len += copy_len;
            }

            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            if (output_buffer != NULL) {
                // Response is accumulated in output_buffer. Uncomment the below line to print the accumulated response
                // ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            int mbedtls_err = 0;
            esp_err_t err = esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
            if (err != 0) {
                ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
                ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
            }
            if (output_buffer != NULL) {
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
            esp_http_client_set_header(evt->client, "From", "user@example.com");
            esp_http_client_set_header(evt->client, "Accept", "text/html");
            esp_http_client_set_redirection(evt->client);
            break;
    }
    return ESP_OK;
}


static void capture_image_from_esp32_cam(void)
{
    httpFinish = false;
    
    memset(cameraBuffer, 0, 10000);
    
    // HTTP client configuration
    esp_http_client_config_t config = {
        .host = "10.0.0.18",
        .path = "/get",
        .event_handler = _http_event_handler,
        .user_data = cameraBuffer,
    };
    client = esp_http_client_init(&config);

    // Open HTTP connection
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) 
    {
		//Get Content Length
		int content_length = esp_http_client_get_content_length(client);
		
		ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %d",
		         esp_http_client_get_status_code(client),
		         content_length);


        // Prepare output buffer for the decoded image
        size_t out_img_buf_size = (content_length * 100); // Assuming maximum size for RGB565
        uint8_t *out_img_buf = malloc(out_img_buf_size);
        if (out_img_buf == NULL) 
        {
            ESP_LOGE(TAG, "Failed to allocate memory for output image");
            //free(buffer);
            esp_http_client_cleanup(client);
            return;
        }
        
        // JPEG decoding configuration
        esp_jpeg_image_cfg_t jpeg_cfg = {
            .indata = (uint8_t*)cameraBuffer,
            .indata_size = sizeof(cameraBuffer),
            .outbuf = out_img_buf,
            .outbuf_size = out_img_buf_size,
            .out_format = JPEG_IMAGE_FORMAT_RGB565,
            .out_scale = JPEG_IMAGE_SCALE_0, // No scaling
            .flags = {
                .swap_color_bytes = 0, // Set to 1 if needed for your display format
            }
        };
        esp_jpeg_image_output_t outimg;

        // Decode the JPEG image
        esp_err_t decode_err = esp_jpeg_decode(&jpeg_cfg, &outimg);
        if (decode_err == ESP_OK) 
        {
            ESP_LOGI(TAG, "JPEG decoded successfully");

            // Create an LVGL image descriptor
            lv_img_dsc_t img_dsc;
            img_dsc.header.always_zero = 0;  // Always set to 0
            img_dsc.header.w = outimg.width; // Set the width from the output image
            img_dsc.header.h = outimg.height; // Set the height from the output image
            img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR;
            img_dsc.data = (const void *)out_img_buf;
			
			printf("width:%d  height:%d  size:%d\r\n",img_dsc.header.w,img_dsc.header.h,out_img_buf_size);
            // Create an LVGL image object
            if (example_lvgl_lock(-1)) 
            {
	           	ESP_LOGI(TAG, "Creating Image");
	           	lv_img_set_src(Image1, &img_dsc);
            	//lv_obj_center(Image1);  // Center the image on the screen
            	example_lvgl_unlock();
        	}
            
            // Free the output buffer after use
            free(out_img_buf);
    	}
        else 
        {
            ESP_LOGE(TAG, "JPEG decoding failed: %s", esp_err_to_name(decode_err));
        }
    } 
    else 
    {
        ESP_LOGE(TAG, "HTTP GET failed: %s", esp_err_to_name(err));
    }

    // Cleanup
    esp_http_client_cleanup(client);
    
    httpFinish = true;
}

static void ip_event_cb(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    ESP_LOGI(TAG, "Handling IP event, event code 0x%" PRIx32, event_id);
    switch (event_id)
    {
    case (IP_EVENT_STA_GOT_IP):
        ip_event_got_ip_t *event_ip = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event_ip->ip_info.ip));
        wifi_retry_count = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    case (IP_EVENT_STA_LOST_IP):
        ESP_LOGI(TAG, "Lost IP");
        break;
    case (IP_EVENT_GOT_IP6):
        ip_event_got_ip6_t *event_ip6 = (ip_event_got_ip6_t *)event_data;
        ESP_LOGI(TAG, "Got IPv6: " IPV6STR, IPV62STR(event_ip6->ip6_info.ip));
        wifi_retry_count = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        break;
    default:
        ESP_LOGI(TAG, "IP event not handled");
        break;
    }
}

static void wifi_event_cb(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    ESP_LOGI(TAG, "Handling Wi-Fi event, event code 0x%" PRIx32, event_id);

    switch (event_id)
    {
    case (WIFI_EVENT_WIFI_READY):
        ESP_LOGI(TAG, "Wi-Fi ready");
        break;
    case (WIFI_EVENT_SCAN_DONE):
        ESP_LOGI(TAG, "Wi-Fi scan done");
        break;
    case (WIFI_EVENT_STA_START):
        ESP_LOGI(TAG, "Wi-Fi started, connecting to AP...");
        esp_wifi_connect();
        break;
    case (WIFI_EVENT_STA_STOP):
        ESP_LOGI(TAG, "Wi-Fi stopped");
        break;
    case (WIFI_EVENT_STA_CONNECTED):
        ESP_LOGI(TAG, "Wi-Fi connected");
        wifiConnected = true;
        break;
    case (WIFI_EVENT_STA_DISCONNECTED):
        ESP_LOGI(TAG, "Wi-Fi disconnected");
        if (wifi_retry_count < WIFI_RETRY_ATTEMPT) {
            ESP_LOGI(TAG, "Retrying to connect to Wi-Fi network...");
            wifiConnected = false;
            esp_wifi_connect();
            wifi_retry_count++;
        } else {
            ESP_LOGI(TAG, "Failed to connect to Wi-Fi network");
            wifiConnected = false;
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        break;
    case (WIFI_EVENT_STA_AUTHMODE_CHANGE):
        ESP_LOGI(TAG, "Wi-Fi authmode changed");
        break;
    default:
        ESP_LOGI(TAG, "Wi-Fi event not handled");
        break;
    }
}

esp_err_t wifiInit(void)
{
    // Initialize Non-Volatile Storage (NVS)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    s_wifi_event_group = xEventGroupCreate();

    ret = esp_netif_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize TCP/IP network stack");
        return ret;
    }

    ret = esp_event_loop_create_default();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create default event loop");
        return ret;
    }

    ret = esp_wifi_set_default_wifi_sta_handlers();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set default handlers");
        return ret;
    }

    tutorial_netif = esp_netif_create_default_wifi_sta();
    if (tutorial_netif == NULL) {
        ESP_LOGE(TAG, "Failed to create default WiFi STA interface");
        return ESP_FAIL;
    }

    // Wi-Fi stack configuration parameters
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_cb,
                                                        NULL,
                                                        &wifi_event_handler));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &ip_event_cb,
                                                        NULL,
                                                        &ip_event_handler));
    return ret;
}

esp_err_t wifiConnect(char* wifi_ssid, char* wifi_password)
{
    wifi_config_t wifi_config = {
        .sta = {
            // this sets the weakest authmode accepted in fast scan mode (default)
            .threshold.authmode = WIFI_AUTHMODE,
        },
    };

    strncpy((char*)wifi_config.sta.ssid, wifi_ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char*)wifi_config.sta.password, wifi_password, sizeof(wifi_config.sta.password));

    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE)); // default is WIFI_PS_MIN_MODEM
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM)); // default is WIFI_STORAGE_FLASH

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

    ESP_LOGI(TAG, "Connecting to Wi-Fi network: %s", wifi_config.sta.ssid);
    ESP_ERROR_CHECK(esp_wifi_start());

    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdFALSE, pdFALSE, portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "Connected to Wi-Fi network: %s", wifi_config.sta.ssid);
        return ESP_OK;
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGE(TAG, "Failed to connect to Wi-Fi network: %s", wifi_config.sta.ssid);
        return ESP_FAIL;
    }

    ESP_LOGE(TAG, "Unexpected Wi-Fi error");
    return ESP_FAIL;
}

esp_err_t wifiDisconnect(void)
{
    if (s_wifi_event_group) {
        vEventGroupDelete(s_wifi_event_group);
    }

    return esp_wifi_disconnect();
}

esp_err_t wifiDeinit(void)
{
    esp_err_t ret = esp_wifi_stop();
    if (ret == ESP_ERR_WIFI_NOT_INIT) {
        ESP_LOGE(TAG, "Wi-Fi stack not initialized");
        return ret;
    }

    ESP_ERROR_CHECK(esp_wifi_deinit());
    ESP_ERROR_CHECK(esp_wifi_clear_default_wifi_driver_and_handlers(tutorial_netif));
    esp_netif_destroy(tutorial_netif);

    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, ESP_EVENT_ANY_ID, ip_event_handler));
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler));

    return ESP_OK;
}


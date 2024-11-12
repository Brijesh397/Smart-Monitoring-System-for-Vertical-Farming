#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/param.h>
#include <stdlib.h>
#include <ctype.h>
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"
#include "sdkconfig.h"

extern "C" 
{
	#include "esp_event.h"
	#include "esp_wifi.h"
	#include "esp_http_client.h"
	#include "esp_netif.h"
	#include "esp_tls.h"
	#include "esp_system.h"
	#include "esp_err.h"
	#include "esp_log.h"
	#include "jpeg_decoder.h"
	#include "wifi.h"
}
const char *TAG = "EDGE_DEVICE";

//Buffer to store Camera Image
char cameraBuffer[12000] = {};

uint8_t *out_img_buf = NULL;

// Define model input size for resizing
#define MODEL_INPUT_WIDTH 	160
#define MODEL_INPUT_HEIGHT 	160
#define CLASSIFIER_THRESHOLD	0.7

// Buffer for resized image
uint8_t resized_buf[MODEL_INPUT_WIDTH * MODEL_INPUT_HEIGHT * 3];

ei_impulse_result_t result = {};

TaskHandle_t wifiTaskHandle = NULL;
void wifiTask(void *arg);

typedef struct
{
	char diseaseDetected[190];
	uint16_t soilMoistureValue;
	int16_t tempValue;
    int16_t humidityValue;
}parameters_t;

parameters_t plantPrediction = {};

bool plantHealth = true;
bool plantHealthImage = true;
bool plantHealthTemperature = true;
bool plantHealthSoilMoisture = true;
bool plantHealthHumidity = true;

char plantHealthReason[1000];

// Function to perform bilinear interpolation and resize an RGB888 image
void resize_rgb888(const uint8_t* input, int in_width, int in_height, uint8_t* output, int out_width, int out_height) {
    float x_ratio = in_width / (float)out_width;
    float y_ratio = in_height / (float)out_height;
    int channels = 3; // RGB888 has 3 channels

    for (int i = 0; i < out_height; i++) {
        for (int j = 0; j < out_width; j++) {
            // Calculate coordinates in the input image
            float x = j * x_ratio;
            float y = i * y_ratio;
            int x_low = (int)x;
            int y_low = (int)y;
            int x_high = x_low + 1 < in_width ? x_low + 1 : x_low;
            int y_high = y_low + 1 < in_height ? y_low + 1 : y_low;

            // Calculate interpolation weights
            float x_weight = x - x_low;
            float y_weight = y - y_low;

            for (int c = 0; c < channels; c++) {
                // Get the values of the four surrounding pixels
                uint8_t top_left = input[(y_low * in_width + x_low) * channels + c];
                uint8_t top_right = input[(y_low * in_width + x_high) * channels + c];
                uint8_t bottom_left = input[(y_high * in_width + x_low) * channels + c];
                uint8_t bottom_right = input[(y_high * in_width + x_high) * channels + c];

                // Interpolate the pixel value
                float top = top_left + x_weight * (top_right - top_left);
                float bottom = bottom_left + x_weight * (bottom_right - bottom_left);
                float value = top + y_weight * (bottom - top);

                // Store the result in the output image
                output[(i * out_width + j) * channels + c] = (uint8_t)value;
            }
        }
    }
}

int raw_feature_get_data(size_t offset, size_t out_len, float *signal_ptr) {
    size_t pixel_ix = offset * 3;
    size_t bytes_left = out_len;
    size_t out_ptr_ix = 0;

    while (bytes_left != 0) {
        uint8_t r, g, b;
        r = resized_buf[pixel_ix];
        g = resized_buf[pixel_ix + 1];
        b = resized_buf[pixel_ix + 2];

        float pixel_f = (r << 16) + (g << 8) + b;
        signal_ptr[out_ptr_ix] = pixel_f;

        out_ptr_ix++;
        pixel_ix += 3;
        bytes_left--;
    }
    return 0;
}

void classify() {
    ESP_LOGI(TAG, "Getting signal...");
    signal_t signal;
    signal.total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_WIDTH;
    signal.get_data = &raw_feature_get_data;

    ESP_LOGI(TAG, "Run classifier...");
    
    EI_IMPULSE_ERROR res = run_classifier(&signal, &result, false /* debug */);
    if (res != EI_IMPULSE_OK) {
        ei_printf("ERR: Failed to run classifier (%d)\n", res);
        return;
    }

    ei_printf("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.): \n",
              result.timing.dsp, result.timing.classification, result.timing.anomaly);
#if EI_CLASSIFIER_OBJECT_DETECTION == 1
    bool bb_found = result.bounding_boxes[0].value > 0;
    for (size_t ix = 0; ix < result.bounding_boxes_count; ix++) {
        auto bb = result.bounding_boxes[ix];
        if (bb.value == 0) {
            continue;
        }

        ei_printf("    %s (", bb.label);
        ei_printf_float(bb.value);
        ei_printf(") [ x: %lu, y: %lu, width: %lu, height: %lu ]\n", bb.x, bb.y, bb.width, bb.height);
    }

    if (!bb_found) {
        ei_printf("    No objects found\n");
    }
#else
	for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) 
	{
        ei_printf("    %s: ", result.classification[ix].label);
        ei_printf_float(result.classification[ix].value);
        ei_printf("\n");
        if (result.classification[ix].value >= CLASSIFIER_THRESHOLD) 
   	 	{
			memset(plantPrediction.diseaseDetected, '\0', 190);
			snprintf(plantPrediction.diseaseDetected, 190, "%s", result.classification[ix].label);	
			printf("Disease Detected : %s \r\n",plantPrediction.diseaseDetected);	
		}
    }
    
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("    anomaly score: ");
    ei_printf_float(result.anomaly);
    ei_printf("\n");
#endif
#endif
}

/************************* capture_image_from_esp32_cam ***************************/
static void capture_image_from_esp32_cam(void)
{
    httpFinish = false;
    
    memset(cameraBuffer, 0, 10000);
    
    // HTTP client configuration
    esp_http_client_config_t config = {
        .host = "10.0.0.18",
        .path = "/capture",
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
        ESP_LOGI(TAG, "Free heap size: %" PRIu32, esp_get_free_heap_size());
        ESP_LOGI(TAG, "Free heap size: %zu", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
        
		ESP_LOGI(TAG, "Free heap size psram: %zu", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
        
        size_t out_img_buf_size = (content_length * 80); // Assuming maximum size for RGB565
        uint8_t* out_img_buf = (uint8_t*) heap_caps_malloc(out_img_buf_size, MALLOC_CAP_SPIRAM);
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
            .out_format = JPEG_IMAGE_FORMAT_RGB888,
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
            
			// Resize the decoded image to the model input size
            resize_rgb888(out_img_buf, outimg.width, outimg.height, resized_buf, MODEL_INPUT_WIDTH, MODEL_INPUT_HEIGHT);
            	
			classify();
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

bool plantHealthPredict(void)
{
	memset(plantHealthReason, '\0', 1000);
	
	//Check Image prediction
	if(strcmp("tomato-healthy",plantPrediction.diseaseDetected) == 0)
	{
		plantHealthImage = true;	
	}
	else
	{
		plantHealthImage = false;
		strcat(plantHealthReason,plantPrediction.diseaseDetected);
		strcat(plantHealthReason,"\n");
	}
	
	//Check Soil Moisture Prediction
	if((plantPrediction.soilMoistureValue >= 60) && (plantPrediction.soilMoistureValue <= 80))
	{
		plantHealthSoilMoisture = true;
	}
	else
	{
		plantHealthSoilMoisture = false;
		char soilMoistureString[20];
		snprintf(soilMoistureString, 20,"Soil Moisture %d", plantPrediction.soilMoistureValue);
		strcat(plantHealthReason,soilMoistureString);
		strcat(plantHealthReason,"\n");
	}
	
	//Check Temperature Prediction
	if((plantPrediction.tempValue >= 24) && (plantPrediction.tempValue <= 27))
	{
		plantHealthTemperature = true;
	}
	else
	{
		plantHealthTemperature = false;
		char temperaturString[20];
		snprintf(temperaturString, 20,"Temperature %d", plantPrediction.tempValue);
		strcat(plantHealthReason,temperaturString);
		strcat(plantHealthReason,"\n");
	}
	
	//Check Humidity Prediction
	if((plantPrediction.humidityValue >= 60) && (plantPrediction.humidityValue <= 85))
	{
		plantHealthHumidity = true;
	}
	else
	{
		plantHealthHumidity = false;
		char HumidityString[20];
		snprintf(HumidityString, 20,"Humidity %d", plantPrediction.humidityValue);
		strcat(plantHealthReason,HumidityString);
		strcat(plantHealthReason,"\n");
	}
	
	//Decide
	if((plantHealthImage == false) || (plantHealthSoilMoisture == false) || (plantHealthTemperature == false) || (plantHealthHumidity == false))
	{
		plantHealth = false;
		printf("%s",plantHealthReason);
	}
	else
	{
		plantHealth = true;
		printf("Plant Health is Good \r\n");		
	}
	
	return plantHealth;
	
}

extern "C" void app_main()
{   
    ESP_LOGI(TAG, "Initialising Wifi...");
    ESP_ERROR_CHECK(wifiInit());
    
    ESP_LOGI(TAG, "Create Wifi task");
	xTaskCreate(wifiTask, "wifiTask", 14096, NULL, 2, &wifiTaskHandle);
	
}

void wifiTask(void *arg)
{ 
    esp_err_t ret = wifiConnect();
    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to connect to Wi-Fi network");
    }
    
    plantPrediction.soilMoistureValue = 50;
    plantPrediction.tempValue = 20;
    plantPrediction.humidityValue = 50;

    while(1)
    {
		if(wifiConnected)
		{
			capture_image_from_esp32_cam();
			plantHealthPredict();
		}
	}
}

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gap.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/addr.h>
#include <zephyr/bluetooth/conn.h>
#include "mySensorService.h"

/************LOGGER MODULE********************/
LOG_MODULE_REGISTER(TemperatureSensor, LOG_LEVEL_INF);

/*************MACROS************************/
#define LOW     false
#define HIGH    true

#define DHT22_GPIO_PORT         DT_NODELABEL(gpio0)
#define DHT22_PIN               2
static const struct device *gpio_dev = DEVICE_DT_GET(DHT22_GPIO_PORT);

/***********Function Prototype**********************/
//Temperature Sensor
int dht22Begin (void);
uint8_t dht22ReadByte(void);
void dht22ReadData(uint8_t *humidityInt, uint8_t *humidityDec, uint8_t *tempInt, uint8_t *tempDec, uint8_t *checksum);
int expectPulse(bool level, uint32_t timeout);

//BMS
void BMSThreadFunc(void);
enum adc_action BMSCallback(const struct device *dev, const struct adc_sequence *sequence, uint16_t n);

//BLE
void on_connected(struct bt_conn *conn, uint8_t err);
void on_disconnected(struct bt_conn *conn, uint8_t reason);
void on_le_param_updated(struct bt_conn *conn, uint16_t interval, uint16_t latency, uint16_t timeout);

//Temperature Sensor Task
void tempSensorThreadFunc(void);

//BLE Task
void BLEThreadFunc(void);

void RGBThreadFunc(void);
int rgb_led_init(void);
void rgb_led_set_color(bool red, bool green, bool blue);
void rgb_led_toggle_blue();

/*************************************************
*********** RGB CONFIGURATION ********************
**************************************************/
#define RGB_GPIO_PORT         	DT_NODELABEL(gpio0)
#define R_PIN               	28
#define G_PIN              		29
#define B_PIN               	4
static const struct device *rgb_dev = DEVICE_DT_GET(RGB_GPIO_PORT);


/*************************************************
*********** ADC CONFIGURATION ********************
**************************************************/
/*******GATHERING ADC INFO FROM DEVICE TREE********/
#define ADC_NODE        DT_NODELABEL(adc)
static const struct device *adc_dev = DEVICE_DT_GET(ADC_NODE);

/***********ADC PARAMETERS************************/
#define ADC_RESOLUTION          12
#define ADC_REFERENCE           ADC_REF_INTERNAL
#define ADC_GAIN                ADC_GAIN_1_5
#define ADC_ACQUISITION_TIME    ADC_ACQ_TIME_DEFAULT
#define ADC_BMS_ID              1       // ADC Channel 1

/**********BMS ADC CONFIG STRUCT*****************/
struct adc_channel_cfg adcBMSChannel1Cfg = {
    .gain = ADC_GAIN,
    .reference = ADC_REFERENCE,
    .acquisition_time = ADC_ACQUISITION_TIME,
    .channel_id = ADC_BMS_ID,
#ifdef CONFIG_ADC_NRFX_SAADC
	.input_positive = SAADC_CH_PSELP_PSELP_AnalogInput1
#endif
};

/*********BMS SAMPLE BUFFER STRUCT************/
uint16_t BMSSampleBuffer[1];

// struct adc_sequence_options BMSSequenceOptions = {
//     .callback = BMSCallback,  /* Link callback here */
// };

struct adc_sequence BMSSequence = {
    .channels = BIT(ADC_BMS_ID),
    .buffer = BMSSampleBuffer,
    .buffer_size = sizeof(BMSSampleBuffer),
    .resolution = ADC_RESOLUTION,
//     .options = &BMSSequenceOptions
};

/*******************************************+******
*********** BLE CONFIGURATION ********************
**************************************************/
#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

/**********ADVERTISING PARAMETERS*****************/
static struct bt_le_adv_param *adv_param =
BT_LE_ADV_PARAM(
    BT_LE_ADV_OPT_CONNECTABLE, //Conectable
	800, //Min Advertising Interval 500ms (800*0.625ms)
	801, //Max Advertising Interval 500.625ms (801*0.625ms)
	NULL //Set to NULL for undirected advertising
);

/*************ADVERTISING MANUFACTURING DATA***********/
typedef struct adv_mfg_data {
    char company_name[10]; //Company Name
    char device_type[2];  //Device Type
} adv_mfg_data_type;

static adv_mfg_data_type adv_mfg_data = {
    .company_name = "SkyHarvest",
    .device_type = "TS",
};

/**************ADVERTISING DATA***************/
static const struct bt_data ad[] =
{
    //Set the advertising flags 
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    
    //Set the advertising packet data
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),

};

/************SCAN RESPONSE DATA***************/
static const struct bt_data sd[] = {
    //Set advertising manufacturing data
    BT_DATA(BT_DATA_MANUFACTURER_DATA,(unsigned char *)&adv_mfg_data, sizeof(adv_mfg_data))
};

//Structure to hold connection Info
struct bt_conn *my_conn = NULL;

/***********CONNECTION CALLBACK STRUCTURE***********/
struct bt_conn_cb connection_callbacks = {
	.connected = on_connected,
	.disconnected = on_disconnected,
    .le_param_updated = on_le_param_updated,
};

/***********Global Variables************************/
uint8_t humidityInt, humidityDec, tempInt, tempDec, checksum;
int16_t temperature;
int16_t humidity;
uint16_t batteryVoltageInt = 0;
uint16_t batteryVoltageBLE = 0;
bool BLECONNECTING = false;
bool BLECONNECTED = false;
bool BLEDISCONNECTED = false;

/**********SEMAPHORES*******************/
/* Semaphores to signal threads */
K_SEM_DEFINE(BMSSem, 0, 1);
K_SEM_DEFINE(tempSem, 0, 1);

/***********THREAD CREATION**********************/
K_THREAD_DEFINE(tempSensorThreadId, 1024, tempSensorThreadFunc, NULL, NULL, NULL, 0, 0, 0);
K_THREAD_DEFINE(BMSThreadId, 1024, BMSThreadFunc, NULL, NULL, NULL, 1 , 0, 0);
K_THREAD_DEFINE(BLEThreadId, 4096, BLEThreadFunc, NULL, NULL, NULL, 2, 0, 0);
K_THREAD_DEFINE(RGBThreadId, 512, RGBThreadFunc, NULL, NULL, NULL, 3, 0, 0);

int main(void)
{     
        int err;

        /***************RGB SETUP********************/
        err = rgb_led_init();
        if (err < 0) {
                LOG_ERR("Failed to initialize RGB LED: %d", err);
                return -1;
        }

        /***************GPIO SETUP**************************/
        // Check Device is ready
        if (!device_is_ready(gpio_dev))
        {
                LOG_INF("GPIO not ready");
                return -1;
        }

        /***************ADC SETUP**************************/
        // Check Device is ready
        if (!device_is_ready(adc_dev))
        {
                LOG_INF("ADC not ready\r\n");
                return 0;
        }

        // Setting up BMS ADC Channel
        err = adc_channel_setup(adc_dev, &adcBMSChannel1Cfg);
        if (err != 0)
        {
                LOG_ERR("ADC setup failed with error %d \r\n", err);
                return 0;
        }

	LOG_INF("Initialisation Successfull\r\n");

         /**************BLE SETUP****************************/
        //Is done inside the BLE Task
        rgb_led_set_color(true, true, true); // White

        return 0;
}

/***************************BLE THREAD***************************************/
void BLEThreadFunc(void)
{
        /**************BLE SETUP****************************/      
        int err;
        
        LOG_INF("BLE initialization started\r\n");

        bt_conn_cb_register(&connection_callbacks);
        
        //Enable Bluetooth module
        err = bt_enable(NULL);
        if (err) 
        {
                LOG_ERR("Bluetooth init failed (err %d)\n", err);
                return ;
        }   

        LOG_INF("Bluetooth initialized\n");

        // Start advertising
        err = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
        if (err) 
        {
                LOG_ERR("Advertising failed to start (err %d)\n", err);
                return ;
        }

        LOG_INF("Advertising successfully started\n");

        BLECONNECTING = true;

        while(1)
        {
                k_sem_take(&BMSSem, K_FOREVER);

                mySensorServiceSendTemperatureNotification(temperature,humidity,batteryVoltageInt);
        }
}

/****************************Temperature Sensor Thread************************/
void tempSensorThreadFunc(void)
{
        while(1)
        {
                dht22Begin();
                dht22ReadData(&humidityInt, &humidityDec, &tempInt, &tempDec, &checksum);
                
                if(checksum == ((humidityInt + humidityDec + tempInt + tempDec) & 0xFF))
                {
                        // Handle temperature sign
                        if (tempInt & 0x80) // Check if the MSB of temp_int is set (negative temperature)
                        {  
                                temperature = -(((tempInt & 0x7F) << 8) | tempDec);  // Convert to negative temperature
                        } 
                        else 
                        {
                                temperature = ((tempInt << 8) | tempDec);  // Positive temperature
                        }

                        humidity = (humidityInt << 8) | humidityDec;

                        LOG_INF("Humidity: %d.%d%% , Temperature: %d.%d°C", humidity /10 , humidity % 10 , temperature / 10, temperature % 10); 

                        k_sem_give(&tempSem);
                } 
                else 
                {
                        LOG_INF("Checksum error!");
                }      

                k_sleep(K_MSEC(3000));                 
        }     
}

/***********************************BMS THREAD***********************************************/
void BMSThreadFunc(void)
{
        int err;
        
        while (1)
        {   
                k_sem_take(&tempSem, K_FOREVER);
                
                LOG_INF("Starting BMS sampling");

                // Start ADC read using Interrupts and store value in sequence
                err = adc_read(adc_dev, &BMSSequence);

                if(err != 0)
                {
                        LOG_ERR("ADC Reading failed %d", err);
                }

                batteryVoltageInt = BMSSampleBuffer[0];

                //Process the ADC sample
                LOG_INF("Processing BMS sample: %d",batteryVoltageInt);

                k_sem_give(&BMSSem);

        }
}

/***********************************RGB THREAD***********************************************/
void RGBThreadFunc(void)
{
    while(1)
    {
        if(BLECONNECTING)
        {
            rgb_led_toggle_blue();  //blue
        }
        else if(BLECONNECTED)
        {
            rgb_led_set_color(false,true,false);    //green
        }
        else if(BLEDISCONNECTED)
        {
            rgb_led_set_color(true,false,false);    //red
            k_sleep(K_MSEC(3000));
            BLEDISCONNECTED = false;
            BLECONNECTING = true;
        }
    }
}


/********************************DHT22 Begin Function***********************************/
int dht22Begin (void)
{
        // Go into high impedence state to let pull-up raise data line level and start the reading process.
        gpio_pin_configure(gpio_dev, DHT22_PIN, GPIO_INPUT | GPIO_PULL_UP );

        k_busy_wait(2);
        
        //Setting the Data Pin to Output High
        gpio_pin_configure(gpio_dev, DHT22_PIN, GPIO_OUTPUT | GPIO_PULL_UP);

        //Setting the Data Pin to Low
        gpio_pin_set(gpio_dev, DHT22_PIN, 0);

        k_busy_wait(1100);

        //Set the GPIO to input
        gpio_pin_configure(gpio_dev, DHT22_PIN, GPIO_INPUT | GPIO_PULL_UP);

        if (expectPulse(HIGH,40) == -1) 
        {
                LOG_INF("DHT timeout waiting for start signal 1 high pulse");
                return -1;
        }

        // First expect a low signal for ~80 microseconds followed by a high signal for ~80 microseconds again.
        if (expectPulse(LOW,80) == -1) 
        {
                LOG_INF("DHT timeout waiting for start signal low pulse");
                return -1;
        }

        if (expectPulse(HIGH,80) == -1) 
        {
                LOG_INF("DHT timeout waiting for start signal 2 high pulse");
                return -1;
        }

        return 0;
}

/********************************DHT22 Read Data Function***********************************/
void dht22ReadData(uint8_t *humidityInt, uint8_t *humidityDec, uint8_t *tempInt, uint8_t *tempDec, uint8_t *checksum) 
{
    *humidityInt = dht22ReadByte();
    *humidityDec = dht22ReadByte();
    *tempInt = dht22ReadByte();
    *tempDec = dht22ReadByte();
    *checksum = dht22ReadByte();
}

/********************************DHT22 Read Byte Function***********************************/
uint8_t dht22ReadByte(void) 
{
    uint8_t receivedByte = 0;
    for (int i = 0; i < 8; i++) 
    {
        if (expectPulse(LOW,50) == -1) 
        {
                LOG_INF("DHT timeout waiting for read signal low pulse.");
        }

        k_busy_wait(40);  // Wait 30us to sample the bit (high for '1', low for '0')
        if (gpio_pin_get(gpio_dev, DHT22_PIN)) 
        {
                receivedByte |= (1 << (7 - i));  // If line is high, it's a '1'
                if (expectPulse(HIGH,50) == -1) 
                {
                        LOG_INF("DHT timeout waiting for read signal High pulse.");
                }
        }
    }
    return receivedByte;        
}

/********************************DHT22 Expect pulse Function***********************************/
int expectPulse(bool level, uint32_t timeout)
{
        uint32_t count = 0;
        
        while((gpio_pin_get(gpio_dev, DHT22_PIN)) == level)
        {
                if(count++ >= timeout)
                {
                        return -1;
                }

                k_busy_wait(1);
        }

        return 0;
}

/*************BLE ON CONNECTED CALLBACK**************/
void on_connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		LOG_ERR("Connection error %d", err);
		return;
	}
	LOG_INF("Connected");
    BLECONNECTED = true;
    BLECONNECTING = false;

	my_conn = bt_conn_ref(conn);

    //structure to store the connection parameters
    struct bt_conn_info info;
	err = bt_conn_get_info(conn, &info);
	if (err) {
		LOG_ERR("bt_conn_get_info() returned %d", err);
		return;
	}

    //Updating and Logging the connection parameters
	double connection_interval = info.le.interval * 1.25; // in ms
	uint16_t supervision_timeout = info.le.timeout * 10; // in ms
	LOG_INF("Connection parameters: interval %.2f ms, latency %d intervals, timeout %d ms",
		connection_interval, info.le.latency, supervision_timeout);
}

/*************BLE ON DISCONNECTED CALLBACK**************/
void on_disconnected(struct bt_conn *conn, uint8_t reason)
{
        BLECONNECTED = false;
        BLEDISCONNECTED = true;
        LOG_INF("Disconnected. Reason %d", reason);
        bt_conn_unref(my_conn);
}

/**************BLE CONNECTION PARAMETER UPDATED CALLBACK******************/
void on_le_param_updated(struct bt_conn *conn, uint16_t interval, uint16_t latency, uint16_t timeout)
{
    double connection_interval = interval*1.25;         // in ms
    uint16_t supervision_timeout = timeout*10;          // in ms
    LOG_INF("Connection parameters updated: interval %.2f ms, latency %d intervals, timeout %d ms", connection_interval, latency, supervision_timeout);
}

/**************Intialise RGB Led******************/
int rgb_led_init(void) {
	// Check Device is ready
    if (!device_is_ready(rgb_dev))
    {
        LOG_INF("GPIO not ready");
        return -1;
    }

	// Go into high impedence state to let pull-up raise data line level and start the reading process.
    gpio_pin_configure(rgb_dev, R_PIN, GPIO_OUTPUT_LOW);

	// Go into high impedence state to let pull-up raise data line level and start the reading process.
    gpio_pin_configure(rgb_dev, G_PIN, GPIO_OUTPUT_LOW);

	// Go into high impedence state to let pull-up raise data line level and start the reading process.
    gpio_pin_configure(rgb_dev, B_PIN, GPIO_OUTPUT_LOW);

    return 0;
}

/**************Set RGB Color******************/
void rgb_led_set_color(bool red, bool green, bool blue) {
    gpio_pin_set(rgb_dev, R_PIN, red);
    gpio_pin_set(rgb_dev, G_PIN, green);
    gpio_pin_set(rgb_dev, B_PIN, blue);
}

void rgb_led_toggle_blue() {
    rgb_led_set_color(false,false,true);
    k_sleep(K_MSEC(500));
    rgb_led_set_color(false,false,false);
    k_sleep(K_MSEC(500));
}

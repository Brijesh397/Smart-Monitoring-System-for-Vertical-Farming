#ifndef MYSENSORSERVICE_H_
#define MYSENSORSERVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

typedef struct sensor_data {
    int16_t tempValue;
    int16_t humidityValue;
    uint16_t bmsValue;
}sensorData_t;

int mySensorServiceSendTemperatureNotification(int16_t tempValue, int16_t humidityValue, uint16_t bmsValue);

/*****************SENSOR SERVICE UUID***********************/
//Create a UUID Array
#define BT_UUID_TS_VAL  BT_UUID_128_ENCODE(0x863a2001, 0x1d45, 0x47a6, 0xac67, 0x415cfb250c68)
//Convert the array to Generic UUID
#define BT_UUID_TS      BT_UUID_DECLARE_128(BT_UUID_TS_VAL)

/******************SOIL MOISTURE SENSOR CHARACTERISITIC UUID*****************/
//Create a UUID Array
#define BT_UUID_TS_TEMPERATURE_VAL  BT_UUID_128_ENCODE(0x863a2002, 0x1d45, 0x47a6, 0xac67, 0x415cfb250c68)
//Convert the array to Generic UUID
#define BT_UUID_SS_TEMPERATURE      BT_UUID_DECLARE_128(BT_UUID_TS_TEMPERATURE_VAL)


#ifdef __cplusplus
}
#endif

#endif
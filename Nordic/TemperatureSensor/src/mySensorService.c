#include "mySensorService.h"

static bool notify_Temperature_enabled;

//Define the configuration change callback function for the Soilmoisture characteristic
static void mySensorServiceTemperature_cfg_changed(const struct bt_gatt_attr *attr,uint16_t value)
{
	notify_Temperature_enabled = (value == BT_GATT_CCC_NOTIFY);
}

// Sensor Service Declaration
BT_GATT_SERVICE_DEFINE(
    mySensorSvc, BT_GATT_PRIMARY_SERVICE(BT_UUID_TS),

    //Create and add the SoilMoisture Sensor characteristic and its CCCD
    BT_GATT_CHARACTERISTIC(BT_UUID_SS_TEMPERATURE, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_NONE, NULL, NULL, NULL),
    BT_GATT_CCC(mySensorServiceTemperature_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CUD("TemperatureSensor", BT_GATT_PERM_READ),

);

// Define the function to send notifications for the SOILMOISTURE characteristic */
int mySensorServiceSendTemperatureNotification(int16_t tempValue, int16_t humidityValue, uint16_t bmsValue)
{
	if (!notify_Temperature_enabled) {
		return -EACCES;
	}

	sensorData_t combinedSensorValue;

	combinedSensorValue.tempValue = tempValue; 
	combinedSensorValue.humidityValue = humidityValue;
	combinedSensorValue.bmsValue = bmsValue;

	return bt_gatt_notify(NULL, &mySensorSvc.attrs[2], &combinedSensorValue, sizeof(combinedSensorValue));
}

#include "mySensorService.h"

static bool notify_Soilmoisture_enabled;

//Define the configuration change callback function for the Soilmoisture characteristic
static void mySensorServiceSoilmoisture_cfg_changed(const struct bt_gatt_attr *attr,uint16_t value)
{
	notify_Soilmoisture_enabled = (value == BT_GATT_CCC_NOTIFY);
}

// Sensor Service Declaration
BT_GATT_SERVICE_DEFINE(
    mySensorSvc, BT_GATT_PRIMARY_SERVICE(BT_UUID_SS),

    //Create and add the SoilMoisture Sensor characteristic and its CCCD
    BT_GATT_CHARACTERISTIC(BT_UUID_SS_SOILMOISTURE, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_NONE, NULL, NULL, NULL),
    BT_GATT_CCC(mySensorServiceSoilmoisture_cfg_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CUD("SoilMoisture", BT_GATT_PERM_READ),

);

// Define the function to send notifications for the SOILMOISTURE characteristic */
int mySensorServiceSendSoilMoistureNotification(uint16_t soilMoistureValue, uint16_t bmsValue)
{
	if (!notify_Soilmoisture_enabled) {
		return -EACCES;
	}

	sensorData_t combinedSensorValue;

	combinedSensorValue.soilMoistureValue = soilMoistureValue; 
	combinedSensorValue.bmsValue = bmsValue;

	return bt_gatt_notify(NULL, &mySensorSvc.attrs[2], &combinedSensorValue, sizeof(combinedSensorValue));
}

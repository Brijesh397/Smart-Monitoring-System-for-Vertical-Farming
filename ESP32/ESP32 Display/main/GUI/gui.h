/*
 * gui.h
 *
 *  Created on: Oct. 13, 2024
 *      Author: Brijesh
 */

#ifndef GUI_H_
#define GUI_H_

#include "main.h"

/*************ENUMS*********************/
typedef enum
{
	DASHBOARDSCREEN = 0,
	ANALYTICSSCREEN
}currentScreen_t;

void createDashboardScreen(void);
void createAnalyticsScreen(void);

extern currentScreen_t	activeScreen;

/***************LVGL********************************/
//Screen Objects
extern lv_obj_t * dashboardScreen;
extern lv_obj_t * analyticsScreen;

//Dashboard Screen Objects
extern lv_obj_t * logoImage;
extern lv_obj_t * verticalFarmingLabel;
extern lv_obj_t * sectorLabel;
extern lv_obj_t * sectorDropdown;
extern lv_obj_t * dashboardPanel;
extern lv_obj_t * locationImage;
extern lv_obj_t * locationLabel;
extern lv_obj_t * dashboardTemperatureLabel;
extern lv_obj_t * dashboardDegreeCelciusLabel;
extern lv_obj_t * weatherLabel;
extern lv_obj_t * weatherImage;
extern lv_obj_t * dateLabel;
extern lv_obj_t * timeLabel;
extern lv_obj_t * sectorPanel;
extern lv_obj_t * sectorLabel;
extern lv_obj_t * plantHealthPanel;
extern lv_obj_t * plantHealthImage;
extern lv_obj_t * plantHealthLabel;
extern lv_obj_t * plantHealthPercentage;
extern lv_obj_t * plantHealthPercentageLabel;
extern lv_obj_t * temperaturePanel;
extern lv_obj_t * temperatureImage;
extern lv_obj_t * temperatureLabel;
extern lv_obj_t * temperatureNumberLabel;
extern lv_obj_t * degreeCelciusLabel;
extern lv_obj_t * humidityPanel;
extern lv_obj_t * humidityImage;
extern lv_obj_t * humidityLabel;
extern lv_obj_t * humidityNumberLabel;
extern lv_obj_t * humidityPercentageLabel;
extern lv_obj_t * soilMoisturePanel;
extern lv_obj_t * soilMoistureImage;
extern lv_obj_t * soilMoistureLabel;
extern lv_obj_t * soilMoistureNumberLabel;
extern lv_obj_t * soilMoisturePercentageLabel;
extern lv_obj_t * analyticsButton;
extern lv_obj_t * analyticsLabel;
extern lv_obj_t * analyticsImage;
extern lv_obj_t * settingsButton;
extern lv_obj_t * settingsLabel;
extern lv_obj_t * settingsImage;

//Analytics Screen
extern lv_obj_t * logo2;
extern lv_obj_t * verticalFarmingLabel2;
extern lv_obj_t * sectorLabel2;
extern lv_obj_t * Dropdown3;
extern lv_obj_t * cameraLabel;
extern lv_obj_t * Image1;
extern lv_obj_t * Panel1;
extern lv_obj_t * plantHealthLabel2;
extern lv_obj_t * plantHealthPercentage2;
extern lv_obj_t * plantHealthPercentageLabel2;
extern lv_obj_t * homeButton;
extern lv_obj_t * Panel2;
extern lv_obj_t * analyticsLabel2;


//Images
LV_IMG_DECLARE(img_logo_png);    // assets/Logo.png
LV_IMG_DECLARE(img_location_png);    // assets/location.png
LV_IMG_DECLARE(img_sunnyimage_png);    // assets/sunnyImage.png
LV_IMG_DECLARE(img_planthealthimage_png);    // assets/plantHealthImage.png
LV_IMG_DECLARE(img_tempimage_png);    // assets/tempImage.png
LV_IMG_DECLARE(img_humidityimage_png);    // assets/humidityImage.png
LV_IMG_DECLARE(img_soilmoistureimage_png);    // assets/soilMoistureImage.png
LV_IMG_DECLARE(img_analyticsimage_png);    // assets/analyticsImage.png
LV_IMG_DECLARE(img_settingsimage_png);    // assets/settingsImage.png

//FOnt
LV_FONT_DECLARE(font_MontserratBold32);
LV_FONT_DECLARE(font_MontserratBold28);
LV_FONT_DECLARE(font_MontserratMedium14);
LV_FONT_DECLARE(font_MontserratBold50);
LV_FONT_DECLARE(font_MontserratBold16);


#endif /* MAIN_GUI_GUI_H_ */

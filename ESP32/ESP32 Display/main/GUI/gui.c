#include "gui.h"


currentScreen_t	activeScreen;

//Screen Objects
lv_obj_t * dashboardScreen = NULL;
lv_obj_t * analyticsScreen = NULL;

//Dashboard Screen Objects
lv_obj_t * logoImage = NULL;
lv_obj_t * verticalFarmingLabel = NULL;
lv_obj_t * sectorDropdown = NULL;
lv_obj_t * dashboardPanel = NULL;
lv_obj_t * locationImage = NULL;
lv_obj_t * locationLabel = NULL;
lv_obj_t * dashboardTemperatureLabel = NULL;
lv_obj_t * dashboarddegreeCelciusLabel = NULL;
lv_obj_t * weatherLabel = NULL;
lv_obj_t * weatherImage = NULL;
lv_obj_t * dateLabel = NULL;
lv_obj_t * timeLabel = NULL;
lv_obj_t * sectorPanel = NULL;
lv_obj_t * sectorLabel = NULL;
lv_obj_t * plantHealthPanel = NULL;
lv_obj_t * plantHealthImage = NULL;
lv_obj_t * plantHealthLabel = NULL;
lv_obj_t * plantHealthPercentage = NULL;
lv_obj_t * plantHealthPercentageLabel = NULL;
lv_obj_t * temperaturePanel = NULL;
lv_obj_t * temperatureImage = NULL;
lv_obj_t * temperatureLabel = NULL;
lv_obj_t * temperatureNumberLabel = NULL;
lv_obj_t * degreeCelciusLabel = NULL;
lv_obj_t * humidityPanel = NULL;
lv_obj_t * humidityImage = NULL;
lv_obj_t * humidityLabel = NULL;
lv_obj_t * humidityNumberLabel = NULL;
lv_obj_t * humidityPercentageLabel = NULL;
lv_obj_t * soilMoisturePanel = NULL;
lv_obj_t * soilMoistureImage = NULL;
lv_obj_t * soilMoistureLabel = NULL;
lv_obj_t * soilMoistureNumberLabel = NULL;
lv_obj_t * soilMoisturePercentageLabel = NULL;
lv_obj_t * analyticsButton = NULL;
lv_obj_t * analyticsLabel = NULL;
lv_obj_t * analyticsImage = NULL;
lv_obj_t * settingsButton = NULL;
lv_obj_t * settingsLabel = NULL;
lv_obj_t * settingsImage = NULL;

//Analytics Screen
lv_obj_t * logo2 = NULL;
lv_obj_t * verticalFarmingLabel2 = NULL;
lv_obj_t * sectorLabel2 = NULL;
lv_obj_t * Dropdown3 = NULL;
lv_obj_t * cameraLabel = NULL;
lv_obj_t * Image1 = NULL;
lv_obj_t * Panel1 = NULL;
lv_obj_t * plantHealthLabel2 = NULL;
lv_obj_t * plantHealthPercentage2 = NULL;
lv_obj_t * plantHealthPercentageLabel2 = NULL;
lv_obj_t * homeButton = NULL;
lv_obj_t * Panel2 = NULL;
lv_obj_t * analyticsLabel2 = NULL;

void homeButtonCallback(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		activeScreen = DASHBOARDSCREEN;
		if (example_lvgl_lock(-1)) 
		{
			while(httpFinish)
			{
				createDashboardScreen();
				lv_obj_del(analyticsScreen);
				example_lvgl_unlock();
				break;
			}
    	}
	}
}

void analyticsButtonCallback(lv_event_t* e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if (code == LV_EVENT_CLICKED)
	{
		if (example_lvgl_lock(-1)) 
		{
			createAnalyticsScreen();
			lv_obj_del(dashboardScreen);
			example_lvgl_unlock();
    	}
	}
}


void createDashboardScreen(void)
{
    activeScreen = DASHBOARDSCREEN;
    
    dashboardScreen = lv_obj_create(NULL);
	lv_scr_load(dashboardScreen);
    
    lv_obj_clear_flag(dashboardScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(dashboardScreen, lv_color_hex(0xEEEEEE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(dashboardScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    logoImage = lv_img_create(dashboardScreen);
    lv_img_set_src(logoImage, &img_logo_png);
    lv_obj_set_width(logoImage, 100);
    lv_obj_set_height(logoImage, 100);
    lv_obj_set_x(logoImage, 30);
    lv_obj_set_y(logoImage, 10);
    lv_obj_add_flag(logoImage, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(logoImage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    verticalFarmingLabel = lv_label_create(dashboardScreen);
    lv_obj_set_width(verticalFarmingLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(verticalFarmingLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(verticalFarmingLabel, 150);
    lv_obj_set_y(verticalFarmingLabel, 25);
    lv_label_set_text(verticalFarmingLabel, "Vertical Farm Monitoring");
    lv_obj_set_style_text_color(verticalFarmingLabel, lv_color_hex(0x393E46), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(verticalFarmingLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(verticalFarmingLabel, &font_MontserratBold28, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    sectorLabel = lv_label_create(dashboardScreen);
    lv_obj_set_width(sectorLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(sectorLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(sectorLabel, -200);
    lv_obj_set_y(sectorLabel, 32);
    lv_obj_set_align(sectorLabel, LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(sectorLabel, "Sector:");
    lv_obj_set_style_text_font(sectorLabel, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    sectorDropdown = lv_dropdown_create(dashboardScreen);
    lv_dropdown_set_options(sectorDropdown, "Spinach Garden 01\nSpinach Garden 02");
    lv_dropdown_set_selected_highlight(sectorDropdown, false);
    lv_obj_set_width(sectorDropdown, 180);
    lv_obj_set_height(sectorDropdown, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(sectorDropdown, -10);
    lv_obj_set_y(sectorDropdown, 20);
    lv_obj_set_align(sectorDropdown, LV_ALIGN_TOP_RIGHT);
    lv_obj_add_flag(sectorDropdown, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_set_style_border_color(sectorDropdown, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(sectorDropdown, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    dashboardPanel = lv_obj_create(dashboardScreen);
    lv_obj_set_width(dashboardPanel, 600);
    lv_obj_set_height(dashboardPanel, 180);
    lv_obj_set_x(dashboardPanel, -32);
    lv_obj_set_y(dashboardPanel, 80);
    lv_obj_set_align(dashboardPanel, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(dashboardPanel,
                      LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                      LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                      LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(dashboardPanel, lv_color_hex(0xF7F7F7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(dashboardPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(dashboardPanel, lv_color_hex(0xF7F7F7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(dashboardPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    locationImage = lv_img_create(dashboardPanel);
    lv_img_set_src(locationImage, &img_location_png);
    lv_obj_set_width(locationImage, 20);
    lv_obj_set_height(locationImage, 30);
    lv_obj_add_flag(locationImage, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(locationImage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    locationLabel = lv_label_create(dashboardPanel);
    lv_obj_set_width(locationLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(locationLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(locationLabel, 25);
    lv_obj_set_y(locationLabel, 5);
    lv_label_set_text(locationLabel, "Cambridge, Canada");
    lv_obj_set_style_text_color(locationLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(locationLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(locationLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    dashboardTemperatureLabel = lv_label_create(dashboardPanel);
    lv_obj_set_width(dashboardTemperatureLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(dashboardTemperatureLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(dashboardTemperatureLabel, 0);
    lv_obj_set_y(dashboardTemperatureLabel, 50);
    lv_label_set_text(dashboardTemperatureLabel, "22");
    lv_obj_set_style_text_color(dashboardTemperatureLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(dashboardTemperatureLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(dashboardTemperatureLabel, &font_MontserratBold50, LV_PART_MAIN | LV_STATE_DEFAULT);

    dashboarddegreeCelciusLabel = lv_label_create(dashboardPanel);
    lv_obj_set_width(dashboarddegreeCelciusLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(dashboarddegreeCelciusLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(dashboarddegreeCelciusLabel, 60);
    lv_obj_set_y(dashboarddegreeCelciusLabel, 55);
    lv_label_set_text(dashboarddegreeCelciusLabel, "°C");
    lv_obj_set_style_text_color(dashboarddegreeCelciusLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(dashboarddegreeCelciusLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(dashboarddegreeCelciusLabel, &lv_font_montserrat_36, LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherLabel = lv_label_create(dashboardPanel);
    lv_obj_set_width(weatherLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(weatherLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(weatherLabel, 0);
    lv_obj_set_y(weatherLabel, 100);
    lv_label_set_text(weatherLabel, "Sunny");
    lv_obj_set_style_text_color(weatherLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(weatherLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(weatherLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    weatherImage = lv_img_create(dashboardPanel);
    lv_img_set_src(weatherImage, &img_sunnyimage_png);
    lv_obj_set_width(weatherImage, 100);
    lv_obj_set_height(weatherImage, 100);
    lv_obj_set_x(weatherImage, 120);
    lv_obj_set_y(weatherImage, 40);
    lv_obj_add_flag(weatherImage, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(weatherImage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    dateLabel = lv_label_create(dashboardPanel);
    lv_obj_set_width(dateLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(dateLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(dateLabel, -15);
    lv_obj_set_y(dateLabel, 55);
    lv_obj_set_align(dateLabel, LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(dateLabel, "Mon, 14 September 2024");
    lv_obj_set_style_text_color(dateLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(dateLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(dateLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    timeLabel = lv_label_create(dashboardPanel);
    lv_obj_set_width(timeLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(timeLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(timeLabel, -10);
    lv_obj_set_y(timeLabel, 85);
    lv_obj_set_align(timeLabel, LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(timeLabel, "09:40 AM");
    lv_obj_set_style_text_color(timeLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(timeLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(timeLabel, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);

    sectorPanel = lv_obj_create(dashboardPanel);
    lv_obj_set_width(sectorPanel, 200);
    lv_obj_set_height(sectorPanel, 50);
    lv_obj_set_x(sectorPanel, 22);
    lv_obj_set_y(sectorPanel, -22);
    lv_obj_set_align(sectorPanel, LV_ALIGN_TOP_RIGHT);
    lv_obj_clear_flag(sectorPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(sectorPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(sectorPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(sectorPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(sectorPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    sectorLabel = lv_label_create(sectorPanel);
    lv_obj_set_width(sectorLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(sectorLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(sectorLabel, LV_ALIGN_LEFT_MID);
    lv_label_set_text(sectorLabel, "Spinach Garden 01");
    lv_obj_set_style_text_font(sectorLabel, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    plantHealthPanel = lv_obj_create(dashboardScreen);
    lv_obj_set_width(plantHealthPanel, 180);
    lv_obj_set_height(plantHealthPanel, 180);
    lv_obj_set_x(plantHealthPanel, 22);
    lv_obj_set_y(plantHealthPanel, -20);
    lv_obj_set_align(plantHealthPanel, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_clear_flag(plantHealthPanel,
                      LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                      LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                      LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(plantHealthPanel, lv_color_hex(0xF7F7F7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(plantHealthPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(plantHealthPanel, lv_color_hex(0xF7F7F7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(plantHealthPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    plantHealthImage = lv_img_create(plantHealthPanel);
    lv_img_set_src(plantHealthImage, &img_planthealthimage_png);
    lv_obj_set_width(plantHealthImage, 30);
    lv_obj_set_height(plantHealthImage, 30);
    lv_obj_add_flag(plantHealthImage, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(plantHealthImage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    plantHealthLabel = lv_label_create(plantHealthPanel);
    lv_obj_set_width(plantHealthLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(plantHealthLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(plantHealthLabel, 0);
    lv_obj_set_y(plantHealthLabel, 45);
    lv_label_set_text(plantHealthLabel, "Plant Health");
    lv_obj_set_style_text_color(plantHealthLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(plantHealthLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(plantHealthLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    plantHealthPercentage = lv_label_create(plantHealthPanel);
    lv_obj_set_width(plantHealthPercentage, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(plantHealthPercentage, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(plantHealthPercentage, 0);
    lv_obj_set_y(plantHealthPercentage, 80);
    lv_label_set_text(plantHealthPercentage, "90");
    lv_obj_set_style_text_color(plantHealthPercentage, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(plantHealthPercentage, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(plantHealthPercentage, &font_MontserratBold50, LV_PART_MAIN | LV_STATE_DEFAULT);

    plantHealthPercentageLabel = lv_label_create(plantHealthPanel);
    lv_obj_set_width(plantHealthPercentageLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(plantHealthPercentageLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(plantHealthPercentageLabel, 70);
    lv_obj_set_y(plantHealthPercentageLabel, 85);
    lv_label_set_text(plantHealthPercentageLabel, "%");
    lv_obj_set_style_text_color(plantHealthPercentageLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(plantHealthPercentageLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(plantHealthPercentageLabel, &lv_font_montserrat_36, LV_PART_MAIN | LV_STATE_DEFAULT);

    temperaturePanel = lv_obj_create(dashboardScreen);
    lv_obj_set_width(temperaturePanel, 180);
    lv_obj_set_height(temperaturePanel, 180);
    lv_obj_set_x(temperaturePanel, 212);
    lv_obj_set_y(temperaturePanel, -20);
    lv_obj_set_align(temperaturePanel, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_clear_flag(temperaturePanel,
                      LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                      LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                      LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(temperaturePanel, lv_color_hex(0xF7F7F7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(temperaturePanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(temperaturePanel, lv_color_hex(0xF7F7F7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(temperaturePanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    temperatureImage = lv_img_create(temperaturePanel);
    lv_img_set_src(temperatureImage, &img_tempimage_png);
    lv_obj_set_width(temperatureImage, 20);
    lv_obj_set_height(temperatureImage, 40);
    lv_obj_set_x(temperatureImage, 0);
    lv_obj_set_y(temperatureImage, -5);
    lv_obj_add_flag(temperatureImage, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(temperatureImage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    temperatureLabel = lv_label_create(temperaturePanel);
    lv_obj_set_width(temperatureLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(temperatureLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(temperatureLabel, 0);
    lv_obj_set_y(temperatureLabel, 45);
    lv_label_set_text(temperatureLabel, "Temperature");
    lv_obj_set_style_text_color(temperatureLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(temperatureLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(temperatureLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    temperatureNumberLabel = lv_label_create(temperaturePanel);
    lv_obj_set_width(temperatureNumberLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(temperatureNumberLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(temperatureNumberLabel, 0);
    lv_obj_set_y(temperatureNumberLabel, 80);
    lv_label_set_text(temperatureNumberLabel, "20");
    lv_obj_set_style_text_color(temperatureNumberLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(temperatureNumberLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(temperatureNumberLabel, &font_MontserratBold50, LV_PART_MAIN | LV_STATE_DEFAULT);

    degreeCelciusLabel = lv_label_create(temperaturePanel);
    lv_obj_set_width(degreeCelciusLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(degreeCelciusLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(degreeCelciusLabel, 65);
    lv_obj_set_y(degreeCelciusLabel, 85);
    lv_label_set_text(degreeCelciusLabel, "°C");
    lv_obj_set_style_text_color(degreeCelciusLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(degreeCelciusLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(degreeCelciusLabel, &lv_font_montserrat_36, LV_PART_MAIN | LV_STATE_DEFAULT);

    humidityPanel = lv_obj_create(dashboardScreen);
    lv_obj_set_width(humidityPanel, 180);
    lv_obj_set_height(humidityPanel, 180);
    lv_obj_set_x(humidityPanel, 402);
    lv_obj_set_y(humidityPanel, -20);
    lv_obj_set_align(humidityPanel, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_clear_flag(humidityPanel, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                      LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                      LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(humidityPanel, lv_color_hex(0xF7F7F7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(humidityPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(humidityPanel, lv_color_hex(0xF7F7F7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(humidityPanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    humidityImage = lv_img_create(humidityPanel);
    lv_img_set_src(humidityImage, &img_humidityimage_png);
    lv_obj_set_width(humidityImage, 30);
    lv_obj_set_height(humidityImage, 30);
    lv_obj_add_flag(humidityImage, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(humidityImage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    humidityLabel = lv_label_create(humidityPanel);
    lv_obj_set_width(humidityLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(humidityLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(humidityLabel, 0);
    lv_obj_set_y(humidityLabel, 45);
    lv_label_set_text(humidityLabel, "Humidity");
    lv_obj_set_style_text_color(humidityLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(humidityLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(humidityLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    humidityNumberLabel = lv_label_create(humidityPanel);
    lv_obj_set_width(humidityNumberLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(humidityNumberLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(humidityNumberLabel, 0);
    lv_obj_set_y(humidityNumberLabel, 80);
    lv_label_set_text(humidityNumberLabel, "80");
    lv_obj_set_style_text_color(humidityNumberLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(humidityNumberLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(humidityNumberLabel, &font_MontserratBold50, LV_PART_MAIN | LV_STATE_DEFAULT);

    humidityPercentageLabel = lv_label_create(humidityPanel);
    lv_obj_set_width(humidityPercentageLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(humidityPercentageLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(humidityPercentageLabel, 70);
    lv_obj_set_y(humidityPercentageLabel, 85);
    lv_label_set_text(humidityPercentageLabel, "%");
    lv_obj_set_style_text_color(humidityPercentageLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(humidityPercentageLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(humidityPercentageLabel, &lv_font_montserrat_36, LV_PART_MAIN | LV_STATE_DEFAULT);

    soilMoisturePanel = lv_obj_create(dashboardScreen);
    lv_obj_set_width(soilMoisturePanel, 180);
    lv_obj_set_height(soilMoisturePanel, 180);
    lv_obj_set_x(soilMoisturePanel, 592);
    lv_obj_set_y(soilMoisturePanel, -20);
    lv_obj_set_align(soilMoisturePanel, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_clear_flag(soilMoisturePanel,
                      LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                      LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                      LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(soilMoisturePanel, lv_color_hex(0xF7F7F7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(soilMoisturePanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(soilMoisturePanel, lv_color_hex(0xF7F7F7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(soilMoisturePanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    soilMoistureImage = lv_img_create(soilMoisturePanel);
    lv_img_set_src(soilMoistureImage, &img_soilmoistureimage_png);
    lv_obj_set_width(soilMoistureImage, 30);
    lv_obj_set_height(soilMoistureImage, 30);
    lv_obj_add_flag(soilMoistureImage, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(soilMoistureImage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    soilMoistureLabel = lv_label_create(soilMoisturePanel);
    lv_obj_set_width(soilMoistureLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(soilMoistureLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(soilMoistureLabel, 0);
    lv_obj_set_y(soilMoistureLabel, 45);
    lv_label_set_text(soilMoistureLabel, "Soil Moisture");
    lv_obj_set_style_text_color(soilMoistureLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(soilMoistureLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(soilMoistureLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    soilMoistureNumberLabel = lv_label_create(soilMoisturePanel);
    lv_obj_set_width(soilMoistureNumberLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(soilMoistureNumberLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(soilMoistureNumberLabel, 0);
    lv_obj_set_y(soilMoistureNumberLabel, 80);
    lv_label_set_text(soilMoistureNumberLabel, "65");
    lv_obj_set_style_text_color(soilMoistureNumberLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(soilMoistureNumberLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(soilMoistureNumberLabel, &font_MontserratBold50, LV_PART_MAIN | LV_STATE_DEFAULT);

    soilMoisturePercentageLabel = lv_label_create(soilMoisturePanel);
    lv_obj_set_width(soilMoisturePercentageLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(soilMoisturePercentageLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(soilMoisturePercentageLabel, 70);
    lv_obj_set_y(soilMoisturePercentageLabel, 85);
    lv_label_set_text(soilMoisturePercentageLabel, "%");
    lv_obj_set_style_text_color(soilMoisturePercentageLabel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(soilMoisturePercentageLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(soilMoisturePercentageLabel, &lv_font_montserrat_36, LV_PART_MAIN | LV_STATE_DEFAULT);

    analyticsButton = lv_obj_create(dashboardScreen);
    lv_obj_set_width(analyticsButton, 130);
    lv_obj_set_height(analyticsButton, 50);
    lv_obj_set_x(analyticsButton, 20);
    lv_obj_set_y(analyticsButton, 130);
    lv_obj_clear_flag(analyticsButton,
                      LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                      LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                      LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(analyticsButton, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(analyticsButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(analyticsButton, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(analyticsButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    analyticsLabel = lv_label_create(analyticsButton);
    lv_obj_set_width(analyticsLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(analyticsLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(analyticsLabel, 15);
    lv_obj_set_y(analyticsLabel, 0);
    lv_obj_set_align(analyticsLabel, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(analyticsLabel, "Analytics");
    lv_obj_set_style_text_color(analyticsLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(analyticsLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(analyticsLabel, &font_MontserratBold16, LV_PART_MAIN | LV_STATE_DEFAULT);

    analyticsImage = lv_img_create(analyticsButton);
    lv_img_set_src(analyticsImage, &img_analyticsimage_png);
    lv_obj_set_width(analyticsImage, 30);
    lv_obj_set_height(analyticsImage, 30);
    lv_obj_set_x(analyticsImage, -15);
    lv_obj_set_y(analyticsImage, 0);
    lv_obj_set_align(analyticsImage, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(analyticsImage, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(analyticsImage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    settingsButton = lv_obj_create(dashboardScreen);
    lv_obj_set_width(settingsButton, 130);
    lv_obj_set_height(settingsButton, 50);
    lv_obj_set_x(settingsButton, 20);
    lv_obj_set_y(settingsButton, 200);
    lv_obj_clear_flag(settingsButton,
                      LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE |
                      LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                      LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(settingsButton, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(settingsButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(settingsButton, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(settingsButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    settingsLabel = lv_label_create(settingsButton);
    lv_obj_set_width(settingsLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(settingsLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(settingsLabel, 10);
    lv_obj_set_y(settingsLabel, 0);
    lv_obj_set_align(settingsLabel, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(settingsLabel, "Settings");
    lv_obj_set_style_text_color(settingsLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(settingsLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(settingsLabel, &font_MontserratBold16, LV_PART_MAIN | LV_STATE_DEFAULT);

    settingsImage = lv_img_create(settingsButton);
    lv_img_set_src(settingsImage, &img_settingsimage_png);
    lv_obj_set_width(settingsImage, 30);
    lv_obj_set_height(settingsImage, 30);
    lv_obj_set_x(settingsImage, -15);
    lv_obj_set_y(settingsImage, 0);
    lv_obj_set_align(settingsImage, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(settingsImage, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(settingsImage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    
    //Analytics Button Event
	lv_obj_add_event_cb(analyticsButton, analyticsButtonCallback, LV_EVENT_ALL, NULL);

}

void createAnalyticsScreen(void)
{
    activeScreen = ANALYTICSSCREEN;
    
    analyticsScreen = lv_obj_create(NULL);
	lv_scr_load(analyticsScreen);
    
    lv_obj_clear_flag(analyticsScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(analyticsScreen, lv_color_hex(0xEEEEEE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(analyticsScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    logo2 = lv_img_create(analyticsScreen);
    lv_img_set_src(logo2, &img_logo_png);
    lv_obj_set_width(logo2, 100);
    lv_obj_set_height(logo2, 100);
    lv_obj_set_x(logo2, 20);
    lv_obj_set_y(logo2, 10);
    lv_obj_add_flag(logo2, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(logo2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    verticalFarmingLabel2 = lv_label_create(analyticsScreen);
    lv_obj_set_width(verticalFarmingLabel2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(verticalFarmingLabel2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(verticalFarmingLabel2, 150);
    lv_obj_set_y(verticalFarmingLabel2, 25);
    lv_label_set_text(verticalFarmingLabel2, "Vertical Farm Monitoring");
    lv_obj_set_style_text_color(verticalFarmingLabel2, lv_color_hex(0x393E46), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(verticalFarmingLabel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(verticalFarmingLabel2, &font_MontserratBold28, LV_PART_MAIN | LV_STATE_DEFAULT);

    sectorLabel2 = lv_label_create(analyticsScreen);
    lv_obj_set_width(sectorLabel2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(sectorLabel2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(sectorLabel2, -200);
    lv_obj_set_y(sectorLabel2, 32);
    lv_obj_set_align(sectorLabel2, LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(sectorLabel2, "Sector:");
    lv_obj_set_style_text_font(sectorLabel2, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    Dropdown3 = lv_dropdown_create(analyticsScreen);
    lv_dropdown_set_options(Dropdown3, "Spinach Garden 01\nSpinach Garden 02");
    lv_dropdown_set_selected_highlight(Dropdown3, false);
    lv_obj_set_width(Dropdown3, 180);
    lv_obj_set_height(Dropdown3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(Dropdown3, -10);
    lv_obj_set_y(Dropdown3, 20);
    lv_obj_set_align(Dropdown3, LV_ALIGN_TOP_RIGHT);
    lv_obj_add_flag(Dropdown3, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_set_style_border_color(Dropdown3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(Dropdown3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    cameraLabel = lv_label_create(analyticsScreen);
    lv_obj_set_width(cameraLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(cameraLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(cameraLabel, -220);
    lv_obj_set_y(cameraLabel, 80);
    lv_obj_set_align(cameraLabel, LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(cameraLabel, "Camera Feed:");
    lv_obj_set_style_text_font(cameraLabel, &font_MontserratBold16, LV_PART_MAIN | LV_STATE_DEFAULT);

    Panel2 = lv_obj_create(analyticsScreen);
    lv_obj_set_width(Panel2, 320);
    lv_obj_set_height(Panel2, 240);
    lv_obj_set_x(Panel2, -20);
    lv_obj_set_y(Panel2, -10);
    lv_obj_set_align(Panel2, LV_ALIGN_RIGHT_MID);
    lv_obj_clear_flag(Panel2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(Panel2, lv_color_hex(0xF7F7F7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(Panel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(Panel2, lv_color_hex(0xF7F7F7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(Panel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    Image1 = lv_img_create(Panel2);
    lv_obj_set_width(Image1, 320);
    lv_obj_set_height(Image1, 240);
    lv_obj_set_align(Image1, LV_ALIGN_CENTER);
    lv_obj_add_flag(Image1, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(Image1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(Image1, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(Image1, lv_color_hex(0xF7F7F7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(Image1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(Image1, 5, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(Image1, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);

    Panel1 = lv_obj_create(analyticsScreen);
    lv_obj_set_width(Panel1, 270);
    lv_obj_set_height(Panel1, 380);
    lv_obj_set_x(Panel1, 170);
    lv_obj_set_y(Panel1, 25);
    lv_obj_set_align(Panel1, LV_ALIGN_LEFT_MID);
    lv_obj_clear_flag(Panel1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(Panel1, lv_color_hex(0xF7F7F7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(Panel1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(Panel1, lv_color_hex(0xF7F7F7), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(Panel1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    plantHealthLabel2 = lv_label_create(Panel1);
    lv_obj_set_width(plantHealthLabel2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(plantHealthLabel2, LV_SIZE_CONTENT);    /// 1
    lv_label_set_text(plantHealthLabel2, "Overal Health :");
    lv_obj_set_style_text_color(plantHealthLabel2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(plantHealthLabel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(plantHealthLabel2, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

    plantHealthPercentage2 = lv_label_create(Panel1);
    lv_obj_set_width(plantHealthPercentage2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(plantHealthPercentage2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(plantHealthPercentage2, 0);
    lv_obj_set_y(plantHealthPercentage2, 30);
    lv_label_set_text(plantHealthPercentage2, "90");
    lv_obj_set_style_text_color(plantHealthPercentage2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(plantHealthPercentage2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(plantHealthPercentage2, &font_MontserratBold50, LV_PART_MAIN | LV_STATE_DEFAULT);

    plantHealthPercentageLabel2 = lv_label_create(Panel1);
    lv_obj_set_width(plantHealthPercentageLabel2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(plantHealthPercentageLabel2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(plantHealthPercentageLabel2, 70);
    lv_obj_set_y(plantHealthPercentageLabel2, 35);
    lv_label_set_text(plantHealthPercentageLabel2, "%");
    lv_obj_set_style_text_color(plantHealthPercentageLabel2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(plantHealthPercentageLabel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(plantHealthPercentageLabel2, &lv_font_montserrat_36, LV_PART_MAIN | LV_STATE_DEFAULT);

    homeButton = lv_obj_create(analyticsScreen);
    lv_obj_set_width(homeButton, 130);
    lv_obj_set_height(homeButton, 50);
    lv_obj_set_x(homeButton, 20);
    lv_obj_set_y(homeButton, 130);
    lv_obj_clear_flag(homeButton, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_GESTURE_BUBBLE |
                      LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                      LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(homeButton, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(homeButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(homeButton, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(homeButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    analyticsLabel2 = lv_label_create(homeButton);
    lv_obj_set_width(analyticsLabel2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(analyticsLabel2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(analyticsLabel2, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(analyticsLabel2, "Home");
    lv_obj_set_style_text_color(analyticsLabel2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(analyticsLabel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(analyticsLabel2, &font_MontserratBold16, LV_PART_MAIN | LV_STATE_DEFAULT);	
    
    //Home Button Event
	lv_obj_add_event_cb(homeButton, homeButtonCallback, LV_EVENT_ALL, NULL);
}
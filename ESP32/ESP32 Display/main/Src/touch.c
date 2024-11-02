/***************************INCLUDES******************************/
#include "touch.h"

esp_lcd_touch_handle_t touch_handle = NULL;

void touchInit(void)
{
	ESP_LOGI(TAG, "Install Touch driver");
    const i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 19,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_io_num = 20,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .master.clk_speed = 400000
    };
    ESP_LOGI(TAG, "i2c_param_config");
    i2c_param_config(I2C_NUM_1, &i2c_conf);
    ESP_LOGI(TAG, "i2c_driver_install");
    i2c_driver_install(I2C_NUM_1, i2c_conf.mode, 0, 0, 0);

    /* Initialize touch */
    const esp_lcd_touch_config_t tp_cfg = {
        .x_max = EXAMPLE_LCD_H_RES,
        .y_max = EXAMPLE_LCD_V_RES,
        .rst_gpio_num = -1, 
        .int_gpio_num = -1,
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 0,
        },
    };
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    

    const esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_GT911_CONFIG();

    ESP_LOGI(TAG, "Create LCD panel IO handle");
    esp_lcd_new_panel_io_i2c((esp_lcd_i2c_bus_handle_t)I2C_NUM_1, &tp_io_config, &tp_io_handle);
    ESP_LOGI(TAG, "Create a new GT911 touch driver");
    esp_lcd_touch_new_i2c_gt911(tp_io_handle, &tp_cfg, &touch_handle);	
}

void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{

    esp_lcd_touch_handle_t touch_handle = (esp_lcd_touch_handle_t)indev_driver->user_data;

    uint16_t touchpad_x;
    uint16_t touchpad_y;
    uint16_t touch_strength;
    uint8_t touch_cnt = 0;

    data->state = LV_INDEV_STATE_REL;

    esp_lcd_touch_read_data(touch_handle);
    bool touchpad_pressed = esp_lcd_touch_get_coordinates(touch_handle, &touchpad_x, &touchpad_y, &touch_strength, &touch_cnt, 1);
    if (touchpad_pressed) {
        //ESP_LOGI(TAG, "Touchpad_read %d %d", touchpad_x, touchpad_y);
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touchpad_x;
        data->point.y = touchpad_y;
    }
}


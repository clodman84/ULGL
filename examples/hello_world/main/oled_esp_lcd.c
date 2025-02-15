#include <stdio.h>
#include <string.h>
#include "driver/i2c_types.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_types.h"
#include "esp_lcd_panel_ops.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "freertos/idf_additions.h"
#include "portmacro.h"

#include "oled_esp_lcd.h"


// make sure the pins are correctly connected here
#define I2C_BUS_PORT  0
#define LCD_PIXEL_CLOCK_HZ    (400 * 1000)
#define PIN_NUM_SDA           21
#define PIN_NUM_SCL           22
#define PIN_NUM_RST           -1
#define I2C_HW_ADDR           0x3C

// Resolution 
#define LCD_H_RES              128
#define LCD_V_RES              64

// Bit number used to represent command and parameter
#define LCD_CMD_BITS           8
#define LCD_PARAM_BITS         8

// for reconfiguring panel options
#define SSD1306_CMD_SET_MEMORY_ADDR_MODE  0x20

static const char* TAG = "OLED Display";

i2c_master_bus_handle_t create_i2c_bus(){
	ESP_LOGI(TAG, "Creating i2c bus");
	i2c_master_bus_handle_t i2c_bus = NULL;
	i2c_master_bus_config_t bus_config = {
		.clk_source = I2C_CLK_SRC_DEFAULT,
		.glitch_ignore_cnt = 7,
		.i2c_port = I2C_BUS_PORT,
		.sda_io_num = PIN_NUM_SDA,
		.scl_io_num = PIN_NUM_SCL,
		.flags.enable_internal_pullup = true,
	};
	ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &i2c_bus));
	return i2c_bus;
}


esp_lcd_panel_io_handle_t allocate_i2c_device_handle(i2c_master_bus_handle_t i2c_bus){
	ESP_LOGI(TAG, "Allocating i2c device handle");
	esp_lcd_panel_io_handle_t io_handle = NULL;
	esp_lcd_panel_io_i2c_config_t io_config = {
		.dev_addr = I2C_HW_ADDR,
		.scl_speed_hz = LCD_PIXEL_CLOCK_HZ,
		.control_phase_bytes = 1, // refer to LCD spec
		.dc_bit_offset = 6,       // refer to LCD spec
		.lcd_cmd_bits = LCD_CMD_BITS,
		.lcd_param_bits = LCD_CMD_BITS,
	};
	ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus, &io_config, &io_handle));
	return io_handle;
}

esp_lcd_panel_handle_t install_panel_driver(esp_lcd_panel_io_handle_t io_handle){
	// Creates a panel handle after installing the ssd1306 driver
	ESP_LOGI(TAG, "Installing SSD1306 driver");
	esp_lcd_panel_handle_t panel = NULL;
	esp_lcd_panel_dev_config_t panel_config = {
		.reset_gpio_num = PIN_NUM_RST,
		.bits_per_pixel = 1
	};
	ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel));
	return panel;
}

esp_lcd_panel_handle_t initialise_oled(void)
{
	i2c_master_bus_handle_t i2c_bus = create_i2c_bus();
	esp_lcd_panel_io_handle_t io_handle = allocate_i2c_device_handle(i2c_bus);
	esp_lcd_panel_handle_t panel = install_panel_driver(io_handle);

	// performing some random panel operations
	
	ESP_LOGI(TAG, "Initialising the panel");

	// NO MATTER WHAT YOU NEED TO DO THESE THREE THINGS BEFORE USING THE DISPLAY idk why the docs don't mention this!!! 
	// for some other displays this process is even lengthier, read the docs
	ESP_ERROR_CHECK(esp_lcd_panel_reset(panel)); // this has to be called before attempting to init the panel
	ESP_ERROR_CHECK(esp_lcd_panel_init(panel));
	ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel, true));

	/*	
	ESP_ERROR_CHECK(esp_lcd_panel_io_tx_param(io_handle, SSD1306_CMD_SET_MEMORY_ADDR_MODE, (uint8_t[]) {
		0x01 // vertical addressing mode
	}, 1));
	*/

	return panel;
}

void verify_memory_address_mode(esp_lcd_panel_handle_t panel, uint8_t *screen){
	uint8_t* start = screen;
	for (int i = 0; i < 64; i++){
		for (int j = 0; j < 16; j++){
			memset(start, 0xFF, 1);
			start += 1;
	 		esp_lcd_panel_draw_bitmap(panel, 0, 0, 128, 64, screen);
			vTaskDelay(23 / portTICK_PERIOD_MS);	
		}
	}
}

void display_bitmap(esp_lcd_panel_handle_t panel, uint8_t *bitmap){
	esp_lcd_panel_draw_bitmap(panel, 0, 0, 128, 64, bitmap);
}


#ifndef OLED_INIT
#define OLED_INIT

#include "esp_lcd_types.h"
#include <stdint.h>


// display_bitmap is the only function that we use, the panel "object" is created within initialise_oled()
esp_lcd_panel_handle_t initialise_oled(void);
void display_bitmap(esp_lcd_panel_handle_t panel, uint8_t *bitmap);
void verify_memory_address_mode(esp_lcd_panel_handle_t panel, uint8_t *screen);

#endif

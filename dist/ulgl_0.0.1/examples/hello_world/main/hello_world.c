#include "ULGL.h"
#include "ssd1306_oled.h"
#include <stdint.h>

uint8_t screen[1024];

void app_main(void) {
    esp_lcd_panel_handle_t panel = initialise_oled();
    draw_text("Hello World", 3, screen);
    display_bitmap(panel, screen);
}


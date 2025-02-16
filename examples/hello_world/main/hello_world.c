#include "ULGL.h"
#include "ssd1306_oled.h"
#include <stdint.h>

uint8_t screen[1024];

void app_main(void) {
    esp_lcd_panel_handle_t panel = initialise_oled();
    draw_text("Hello", 34, 19, screen);
    draw_text("World", 51, 43, screen);
    print_bitmap_in_horizontal_mode(screen, 1024, 128);
    display_bitmap(panel, screen);
}

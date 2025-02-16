#include "ULGL.h"
#include "ssd1306_oled.h"

uint8_t screen[1024];

void app_main(void) {
    esp_lcd_panel_handle_t panel = initialise_oled();
    draw_text("Hello", 34, 11, screen, false);
    draw_text("World", 37, 15, screen, false);
    draw_text("Hello", 66, 43, screen, false);
    draw_text("World", 69, 47, screen, true);
    print_bitmap_in_horizontal_mode(screen, 1024, 128);
    display_bitmap(panel, screen);
}

#include "ULGL.h"
#include "ssd1306_oled.h"

uint8_t screen[1024];

void app_main(void) {
    esp_lcd_panel_handle_t panel = initialise_oled();

    draw_text("Hello", 34, 11, screen, false);
    draw_text("World", 37, 15, screen, false);
    draw_text("Hello", 66, 43, screen, false);
    draw_text("World", 69, 47, screen, true);
    draw_line(10, 5, 100, 20, screen);
    draw_line(20, 30, 80, 60, screen);
    draw_line(50, 10, 75, 55, screen);
    draw_line(0, 32, 127, 32, screen);
    draw_line(5, 60, 120, 10, screen);
    draw_line(60, 5, 60, 55, screen);
    print_bitmap_in_horizontal_mode(screen, 1024, 128);
    display_bitmap(panel, screen);
}

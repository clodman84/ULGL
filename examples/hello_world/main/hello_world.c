#include "ULGL.h"
#include "ssd1306_oled.h"

uint8_t screen[1024];

void app_main(void){
	esp_lcd_panel_handle_t panel = initialise_oled();

	draw_text(U"Hello", 34, 11, screen, false, false);
	draw_text(U"World", 37, 15, screen, false, false);
	draw_text(U"ハロー", 66, 43, screen, false, false);
	draw_text(U"ワールト゛", 69, 47, screen, true, false);
	draw_text(U"ULGL🔔", 18, 51, screen, false, true);
	draw_line(21, 43, 101, 23, screen);

	print_bitmap_in_horizontal_mode(screen, 1024, 128);
	display_bitmap(panel, screen);
}

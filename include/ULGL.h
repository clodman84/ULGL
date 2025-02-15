#ifndef ULGL
#define ULGL

#include <stdint.h>

/*
 ULTRA Lightweight Graphics Library, designed for this project 


 This doesn't directly write anything onto the display and only manipulates an array,
 the array needs to be displayed through oled_esp_lcd's display_bitmap function.

*/

void print_bitmap_in_horizontal_mode(uint8_t *screen, int height, int width);  // prints bitmap on serial out
void draw_text(char *text, int page, uint8_t *screen);  // uses a bitmap font that can be tweaked in images.h
void draw_logo(uint8_t *screen);  // its a key rn

#endif

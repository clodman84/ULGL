#ifndef ULGL
#define ULGL

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*

 ULTRA Lightweight Graphics Library, designed for this project 

 This doesn't directly write anything onto the display and only manipulates an array,
 the array needs to be displayed through oled_esp_lcd's display_bitmap function.

*/

void print_bitmap_in_horizontal_mode(uint8_t *bitmap, size_t sizeof_bitmap, int width);  // prints bitmap on serial out
void draw_bitmap(uint8_t *bitmap, size_t sizeof_bitmap, int width, int x, int y, uint8_t *screen, bool transparent);
void draw_text(char *text, int x, int y, uint8_t *screen, bool transparent);  // uses a bitmap font that can be tweaked in images.h
void draw_logo(uint8_t *screen);  // its a key rn
void draw_pixel(int x, int y, uint8_t *screen);
void draw_line(int x0, int y0, int x1, int y1, uint8_t *screen);

#endif

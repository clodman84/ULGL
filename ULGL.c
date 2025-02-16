/*
 ULTRA Lightweight Graphics Library
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ULGL.h"
#include "images.h"

// Resolution 
#define LCD_H_RES             128
#define LCD_V_RES             64

void print_bitmap_in_horizontal_mode(uint8_t *screen, int height, int width){
	// Helper function to verify the contents of a bitmap 
	// use this with the screen array, it will output what the display is supposed to be showing rn
	// helpful for debugging sometimes
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int page = y / 8;// Determine which byte
			int index = page * width + x;
			int bit = y & 0b111;// Determine the bit position (equivalent to y % 8)
			// Check if the pixel is set (1) or not (0)
			if (screen[index] & (1 << bit)){
				printf("#"); // Pixel is on
			} else {
				printf(" "); // Pixel is off
			}
		}
	printf("\n"); 
	} 
}

void draw_bitmap(uint8_t *bitmap, size_t sizeof_bitmap, int width, int x, int y, uint8_t *screen){    // (x,y) is position of top left corner
	int height = sizeof_bitmap / width;
    int bit = y % 8;
	if (bit == 0) {
	    for (int i = 0; i < height; i++){
		    memcpy(screen + (i+y/8)*LCD_H_RES + x, bitmap + i*width, width);
		}
	} else {
	    uint8_t *bitmap_new = calloc(width*(height+2), 1);
	    memcpy(bitmap_new+width, bitmap, width*height);
	    for (int i = 0; i < height+1; i++){
	    	for (int j = 0; j < width; j++){
	    		*(bitmap_new + i*width + j) = (*(bitmap_new + i*width + j) >> (8-bit)) | (*(bitmap_new + (i+1)*width + j) << bit);
	    	}
	    }
	    for (int i = 0; i < height+1; i++){
	    	memcpy(screen + (i+y/8)*LCD_H_RES + x, bitmap_new + i*width, width);
	    }
	}
}

void draw_text(char *text, int x, int y, uint8_t *screen){
	for (int i = 0; text[i] != '\0'; i++){
		char c = text[i];
		uint8_t* char_glyph = fontData7x8[c - 32];
		// printf("%c\n", c);
		// print_bitmap_in_horizontal_mode(char_glyph, 8, 7);
		// printf("\n");
		draw_bitmap(char_glyph, 7, 7, x, y, screen);   // the font is 7 bytes wide
		x += 7;
	}
};

void draw_logo(uint8_t *screen){
    draw_bitmap(logo, 1024, 128, (LCD_H_RES-128)/2, (LCD_V_RES-64)/2, screen);
}

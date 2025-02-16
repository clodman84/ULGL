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

void print_bitmap_in_horizontal_mode(uint8_t *bitmap, size_t sizeof_bitmap, int width){
	int height = sizeof_bitmap / width;
	for (int i = 0; i < 4*height; i++){
		for (int j = 0; j < width/2; j++){
			uint8_t *position = bitmap + (i/4)*width + 2*j;
			uint8_t unwrap(uint8_t *position){
				uint8_t temp1 = (*position     & (0b11 << (2*(i%4)))) >> (2*(i%4));
				uint8_t temp2 = (*(position+1) & (0b11 << (2*(i%4)))) >> (2*(i%4));
				return  (temp1 << 2) | temp2;
			}
			switch (unwrap(position)){
				case 0b0000: printf(" "); break;
				case 0b0001: printf("\xE2\x96\x9D"); break; // U+259D QUADRANT UPPER RIGHT
				case 0b0010: printf("\xE2\x96\x97"); break; // U+2597 QUADRANT LOWER RIGHT
				case 0b0011: printf("\xE2\x96\x90"); break; // U+2590 RIGHT HALF BLOCK
				case 0b0100: printf("\xE2\x96\x98"); break; // U+2598 QUADRANT UPPER LEFT
				case 0b0101: printf("\xE2\x96\x80"); break; // U+2580 UPPER HALF BLOCK
				case 0b0110: printf("\xE2\x96\x9A"); break; // U+259A QUADRANT UPPER LEFT AND LOWER RIGHT
				case 0b0111: printf("\xE2\x96\x9C"); break; // U+259C QUADRANT UPPER LEFT AND UPPER RIGHT AND LOWER RIGHT
				case 0b1000: printf("\xE2\x96\x96"); break; // U+2596 QUADRANT LOWER LEFT
				case 0b1001: printf("\xE2\x96\x9E"); break; // U+259E QUADRANT UPPER RIGHT AND LOWER LEFT
				case 0b1010: printf("\xE2\x96\x84"); break; // U+2584 LOWER HALF BLOCK
				case 0b1011: printf("\xE2\x96\x9F"); break; // U+259F QUADRANT UPPER RIGHT AND LOWER LEFT AND LOWER RIGHT
				case 0b1100: printf("\xE2\x96\x8C"); break; // U+258C LEFT HALF BLOCK
				case 0b1101: printf("\xE2\x96\x9B"); break; // U+259B QUADRANT UPPER LEFT AND UPPER RIGHT AND LOWER LEFT
				case 0b1110: printf("\xE2\x96\x99"); break; // U+2599 QUADRANT UPPER LEFT AND LOWER LEFT AND LOWER RIGHT
				case 0b1111: printf("\xE2\x96\x88"); break; // U+2588 FULL BLOCK
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

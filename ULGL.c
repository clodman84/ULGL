/*
 ULTRA Lightweight Graphics Library
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ULGL.h"
#include "images.h"

// Resolution 
#define LCD_H_RES             128
#define LCD_V_RES             64


uint8_t _unwrap(uint8_t *position, int block_x){
	uint8_t temp1 = (*position     & (0b11 << (2*(block_x%4)))) >> (2*(block_x%4));
	uint8_t temp2 = (*(position+1) & (0b11 << (2*(block_x%4)))) >> (2*(block_x%4));
	return  (temp1 << 2) | temp2;
}


void print_bitmap_in_horizontal_mode(uint8_t *bitmap, size_t sizeof_bitmap, int width){
	int height = sizeof_bitmap / width;
	for (int block_x = 0; block_x < 4*height; block_x++){
		for (int j = 0; j < width/2; j++){
			uint8_t *position = bitmap + (block_x/4)*width + 2*j;
			switch (_unwrap(position, block_x)){
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


void draw_bitmap(uint8_t *bitmap, size_t sizeof_bitmap, int width, int x, int y, uint8_t *screen, bool transparent){    // (x,y) is position of top left corner
	int height = sizeof_bitmap / width;
	int bit = y % 8;
	if (bit == 0){
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
		if (transparent){
			for (int i = 0; i < height+1; i++){
				for (int j = 0; j < width; j++){
					*(bitmap_new + i*width + j) |= *(screen + (i+y/8)*LCD_H_RES + x + j);
				}
			}
		} else {
			for (int j = 0; j < width; j++){
				*(bitmap_new + j) |= *(screen + (y/8)*LCD_H_RES + x + j) & (0b11111111 >> (8-bit));
			}
			for (int j = 0; j < width; j++){
				*(bitmap_new + height*width + j) |= *(screen + (height+y/8)*LCD_H_RES + x + j) & (0b11111111 << bit);
			}
		}
		for (int i = 0; i < height+1; i++){
			memcpy(screen + (i+y/8)*LCD_H_RES + x, bitmap_new + i*width, width);
		}
	}
}


void draw_text(char *text, int x, int y, uint8_t *screen, bool transparent){
	for (int i = 0; text[i] != '\0'; i++){
		char c = text[i];
		uint8_t* char_glyph = fontData7x8[c - 32];
		// printf("%c\n", c);
		// print_bitmap_in_horizontal_mode(char_glyph, 8, 7);
		// printf("\n");
		draw_bitmap(char_glyph, 7, 7, x, y, screen, transparent);   // the font is 7 bytes wide
		x += 7;
	}
};


void draw_logo(uint8_t *screen){
    draw_bitmap(logo, 1024, 128, (LCD_H_RES-128)/2, (LCD_V_RES-64)/2, screen, false);
}


void draw_pixel(int x, int y, uint8_t *screen){	
	int page = y / 8;
	int index = page * 128 + x;
	int bit = y & 0b111;
	screen[index] |= 1 << bit;
}


int abs(int x){
	return x > 0 ? x:-x;
}


void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}


void draw_line(int x0, int y0, int x1, int y1, uint8_t *screen){
        // Bresenham Algorithm from Wikipedia (not exactly but pretty much the same)
        bool steep = false;
        if (abs(y1-y0) > abs(x1-x0)){
                swap(&x0, &y0);
                swap(&x1, &y1);
                steep = true;
        }
        if (x0 > x1) {
                swap(&x0, &x1);
                swap(&y0, &y1);
        }
        int yi = 1;
        int dx = x1 - x0;
        int dy = y1 - y0;
        if (dy < 0){
                yi = -1;
                dy = -dy;
        }
        int D =  2*dy - dx;
        int y = y0;

        for (int x = x0; x <= x1; x++){
                if(steep) draw_pixel(y, x, screen); 
                else draw_pixel(x, y, screen);
                if (D > 0){
                        y += yi;
                        D -= 2*dx;
                }
                D += 2*dy;
        }
}

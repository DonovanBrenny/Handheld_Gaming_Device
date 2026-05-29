/* 
 * File:   OLED_drawings.h
 * Author: Donovan Brenny
 *
 * Created on November 18, 2025
 */


#include "xc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "game_ASM.h"
#include "6x8_font_table.h"
#include "OLED.h"


void draw_char(int x, int y, char c){
    
    if (c < 32 || c > 126) return;  // Outside table range
    
    unsigned char index = c - 32;   // characters start at 32, Array starts at 0. Have to offset
    
    
    // For any CHAR grab the 6 columns from the 6x8 font table and set the pixels
    for (int col = 0; col < 6; col++){
 
        const char column = font_6x8[index][col];
        
        for (int row = 0; row < 8; row++){
             if ((column >> row) & 1){
                oled_set_pixel(x + col, y + row, 1);
            }
        }
    }
}


void draw_text(int x, int y, const char *text){
    
    // Uses the draw_char function and makes it iterate over a string
    int cursor_x = x;
    
    while (*text) {
        
        if (*text == '\n') {
            
            y += 8;
            cursor_x = x;
            
        } else {
            
            draw_char(cursor_x, y, *text);
            cursor_x += 6;
            
        }
        
        text++;
    }
}


void del_char(int x, int y, char c){
    
    if (c < 32 || c > 126) return;  // Outside table range
    
    unsigned char index = c - 32;   // characters start at 32, Array starts at 0. Have to offset
    
    
    // For any CHAR grab the 6 columns from the 6x8 font table and set the pixels
    for (int col = 0; col < 6; col++){
 
        const char column = font_6x8[index][col];
        
        for (int row = 0; row < 8; row++){
             if ((column >> row) & 1){
                oled_set_pixel(x + col, y + row, 0);
            }
        }
    }
}


void del_text(int x, int y, const char *text){
    
    // Uses the draw_char function and makes it iterate over a string
    int cursor_x = x;
    
    while (*text) {
        
        if (*text == '\n') {
            
            y += 8;
            cursor_x = x;
            
        } else {
            
            del_char(cursor_x, y, *text);
            cursor_x += 6;
            
        }
        
        text++;
    }
}

// Finds the number of characters on a line
int chars_on_line(const char *s){
    
    int len = 0;
    while(*s && *s != '\n'){
        len++;
        s++;
    }
    return len;
}


//Horizontally centers the string on an OLED line
int center_text_position(const char *line){
    
    int len = chars_on_line(line);
    int text_width = len * FONT_6x8_WIDTH;
    int x = (OLED_WIDTH - text_width) / 2;
    if (x < 0) x = 0;
    return x;
}


// Draw any length HORIZONTAL line
void draw_hline(int x0, int x1, int y)
{   
    int x;
    for (x = x0; x <= x1; x++) {
        oled_set_pixel(x, y, 1);
    }
}


// Draw any length VERTICAL line
void draw_vline(int x, int y0, int y1)
{
    int y;
    for (y = y0; y <= y1; y++) {
        oled_set_pixel(x, y, 1);
    }
}


// Erase any length HORIZONTAL line
void del_hline(int x0, int x1, int y)
{   
    int x;
    for (x = x0; x <= x1; x++) {
        oled_set_pixel(x, y, 0);
    }
}


// Erase any length VERTICAL line
void del_vline(int x, int y0, int y1)
{
    int y;
    for (y = y0; y <= y1; y++) {
        oled_set_pixel(x, y, 0);
    }
}


void draw_rect_outline(int x, int y, int w, int h)
{
    draw_hline(x, w, y);
    draw_hline(x, w, h);
    draw_vline(x, y, h);
    draw_vline(w, y, h);
}


void del_rect_outline(int x, int y, int w, int h)
{
    del_hline(x, w, y);
    del_hline(x, w, h);
    del_vline(x, y, h);
    del_vline(w, y, h);
}



void draw_fill_rect(char x, char y, char w, char h){
    char x1 = x + w;   // convert width to end coordinate

    for (int j = 0; j < h; j++) {
        int row_y = y + j;
        draw_hline(x, x1, row_y);
    }
}


void del_fill_rect(char x, char y, char w, char h){
    char x1 = x + w;   // convert width to end coordinate

    for (int j = 0; j < h; j++) {
        int row_y = y + j;
        del_hline(x, x1, row_y);
    }
}


static const char ball_sprite[8] = {
    
    0x3c,
    0x7e,
    0xff,
    0xff,
    0xff,
    0xff,
    0x7e,
    0x3c
};


void draw_ball(char x, char y){
    
    
    for (int row = 0; row < 8; row++) {
        unsigned char bits = ball_sprite[row];
        for (int col = 0; col < 8; col++) {
            if (bits & (1 << col)) {
                oled_set_pixel(x + col, y + row, 1);
            }
        }
    }
}


void del_ball(char x, char y){
    
    
    for (int row = 0; row < 8; row++) {
        unsigned char bits = ball_sprite[row];
        for (int col = 0; col < 8; col++) {
            if (bits & (1 << col)) {
                oled_set_pixel(x + col, y + row, 0);
            }
        }
    }
}
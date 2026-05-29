/* 
 * File:   OLED_tests.c
 * Author: Donovan Brenny
 *
 * Created on November 18, 2025
 */

#include "6x8_font_table.h"
#include "OLED.h"


void four_corners_test(void){
    
    oled_clear();
    oled_set_pixel(0,   0,   1);   // top-left
    oled_set_pixel(127, 0,   1);   // top-right
    oled_set_pixel(0,   127, 1);   // bottom-left
    oled_set_pixel(127, 127, 1);   // bottom-right

    push_oled_frame();
    
}


void text_test(void){
    
    oled_clear();

    // simplest test: text in top-left
    //oled_draw_text(0, 0, "SUPER");
    //oled_draw_text(0, 10, "TEAM");

    push_oled_frame();
    
}


void draw_char_test(int x, int y, char c){
    
    // Ignore 'c' for now, just draw a solid 6x8 block
    for (int col = 0; col < 6; col++) {
        for (int row = 0; row < 8; row++) {
            oled_set_pixel(x + col, y + row, 1);
        }
    }
}

void draw_text_test(int x, int y, const char *text){
    
    int cursor_x = x;
    
    while (*text) {
        
        if (*text == '\n') {
            
            y += 8;
            cursor_x = x;
            
        } else {
            
            draw_char_test(cursor_x, y, *text);
            cursor_x += 6;
            
        }
        
        text++;
    }
}


void start_up_animation_test(void)
{
    oled_clear();
    draw_text_test(0, 0, (char *)"A");
    push_oled_frame();
}
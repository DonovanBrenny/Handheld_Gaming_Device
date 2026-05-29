/* 
 * File:   Confirm_screen.c
 * Author: Donovan Brenny
 *
 * Created on November 22, 2025
 */

#include "xc.h"
#include <stdio.h>
#include <stdlib.h>
#include "OLED.h"
#include "6x8_font_table.h"
#include "OLED_drawings.h"
#include "Joystick.h"
#include "device_main.h"


// Pop-up box size
#define POPUP_X 20
#define POPUP_Y 18
#define POPUP_W 112
#define POPUP_H 92


//Controls which option is highlighted
int no_yes_index = 0;
static int highlight_visible = 0;

// Controls how the YES / NO operates in the STATE_CONFIRM_POPUP
int confirm_index = 0;


// Displays a custom 2-line display screen with yes/no options
void draw_confirm_popup(const char *line1, const char *line2,
            const char *line3, const char *line4, int border) {
    
    // Clear the screen
    clear_oled();           
    char x = POPUP_X;
    char y = POPUP_Y;
    char w = POPUP_W;
    char h = POPUP_H;

    if(border) {
        
        // Draw the popup border
        draw_rect_outline(x, y, w, h);
    }
   
    
    // Center and Display line1
    int x1 = center_text_position(line1);
    int y1 = y + FONT_6x8_HEIGHT;
    draw_text(x1, y1, line1);
    
    
    // Center and Display line2
    int x2 = center_text_position(line2);
    int y2 = y1 + (2 * FONT_6x8_HEIGHT);
    draw_text(x2, y2, line2);
    
    
    int x3 = center_text_position(line3);
    int y3 = y2 + FONT_6x8_HEIGHT;
    draw_text(x3, y3, line3);
    
    int x4 = center_text_position(line4);
    int y4 = y3 + FONT_6x8_HEIGHT;
    draw_text(x4, y4, line4);
    
    // Display option text
    draw_text(x + 20, h - 14, "No");
    draw_text(w - 32, h - 14, "Yes");
   
    // Write to the OLED
    push_oled_frame();
}


// Updates the confirm pop-op to check for button and joystick inputs
void ui_confirm_update(void){
    
    // Check for joystick movement
    joystick_poll(0);
    
    
    // If on "NO" and joystick is pushed RIGHT move to "YES"
    if(right && !no_yes_index){
        no_yes_index = 1;
    }
    
    
    // If on "YES" and joystick is moved LEFT move to "NO"
    if(left && no_yes_index){
        no_yes_index = 0;
    }
    
    
    // Blinks the highlight around "NO"
    highlight_visible = !highlight_visible;
    if(!no_yes_index && highlight_visible){
        del_rect_outline(POPUP_W - 36, POPUP_H - 18, POPUP_W - 12, POPUP_H - 4);
        draw_rect_outline(POPUP_X + 16, POPUP_H - 18, POPUP_X + 34, POPUP_H - 4);
        push_oled_frame();
    }
    else if (!no_yes_index && !highlight_visible){
        del_rect_outline(POPUP_W - 36, POPUP_H - 18, POPUP_W - 12, POPUP_H - 4);
        del_rect_outline(POPUP_X + 16, POPUP_H - 18, POPUP_X + 34, POPUP_H - 4);
        push_oled_frame();
    }
    
    
    // Blinks the highlight around "YES"
    if(no_yes_index && highlight_visible){
        del_rect_outline(POPUP_X + 16, POPUP_H - 18, POPUP_X + 34, POPUP_H - 4);
        draw_rect_outline(POPUP_W - 36, POPUP_H - 18, POPUP_W - 12, POPUP_H - 4);
        push_oled_frame();
    }
    else if(no_yes_index && !highlight_visible){
        del_rect_outline(POPUP_X + 16, POPUP_H - 18, POPUP_X + 34, POPUP_H - 4);
        del_rect_outline(POPUP_W - 36, POPUP_H - 18, POPUP_W - 12, POPUP_H - 4);
        push_oled_frame();
    }
}
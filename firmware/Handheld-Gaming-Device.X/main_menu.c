/* 
 * File:   main_menu.h
 * Author: Donovan Brenny
 *
 * Created on November 20, 2025
 */

#include "OLED_drawings.h"
#include "OLED.h"
#include "OLED_drawings.h"
#include "6x8_font_table.h"
#include "main_menu.h"
#include "Joystick.h"


#define HEADER_HEIGHT 28
#define FOOTER_HEIGHT 20
#define MENU_ITEMS 3
#define MENU_LIST_H (OLED_HEIGHT - HEADER_HEIGHT - FOOTER_HEIGHT)

// List of Main Menu items
const char *items[MENU_ITEMS] = {"Games", "Settings", "Power Off"};

// Highlight tracker
static int highlight_visible = 0;

// Tracks the selected menu item
int selected_menu_index = 0;


// Draws the Main Menu HEADER
void menu_header(void){
    
    // Center and Display the HEADER text
    const char *line1 = "DEVICE_NAME";
    int x1 = center_text_position(line1);
    int y1 = FONT_6x8_HEIGHT;
    draw_text(x1, y1, line1);
    
    
    // Write to the OLED
    push_oled_frame();
}


// Draws the Main Menu FOOTER
void menu_footer(void){
    
    // Display the FOOTER text
    const char *line2 = "SELECT (B)";
    const char *line3 = "v1.0.0";
    int len = chars_on_line(line3);
    int right_offset = OLED_WIDTH - (len * 6);
    draw_text(0, OLED_HEIGHT - FONT_6x8_HEIGHT - 1, line2);
    draw_text(right_offset, OLED_HEIGHT - FONT_6x8_HEIGHT - 1, line3);

    
    // Write to OLED
    push_oled_frame();
}


// Draws the Main Menu item list
void menu_list(void){
    
    // Slot height per Menu item
    static int slot_height = MENU_LIST_H / MENU_ITEMS;
    if (slot_height < FONT_6x8_HEIGHT + 2) {
        slot_height = FONT_6x8_HEIGHT + 2;
    }
    
    
    // For each item display the name with the y-axis value of slot_h * slot #
    for (int item = 0; item < MENU_ITEMS; item++){
        const char *name = items[item];
        char slot_y1 = HEADER_HEIGHT + item * slot_height;
        char text_y = slot_y1 + (slot_height - FONT_6x8_HEIGHT) / 2;
        char text_x = center_text_position(name);
        draw_text(text_x, text_y, name);
        
        
        // Write to OLED
        push_oled_frame();
    }
}


// Display the Main Menu
void draw_main_menu(void){
    
    clear_oled();
    menu_header();
    menu_footer();
    menu_list();
    
}


// Updates the Main Menu to check for button and joystick inputs
void ui_menu_update(void){
    
    
    // Polls the Joystick to check for inputs
    joystick_poll(0);
    
    
    // When joystick is pushed UP move UP the list
    if(up){
        if(selected_menu_index > 0) selected_menu_index--;
    }
    
    
    // When joystick is pushed DOWN move DOWN the list
    else if(down){
        if(selected_menu_index < MENU_ITEMS - 1)selected_menu_index++;
    }
    
    
    // Height of a slot in the list
    static int slot_h = MENU_LIST_H / MENU_ITEMS;
    if (slot_h < FONT_6x8_HEIGHT + 2) {
        slot_h = FONT_6x8_HEIGHT + 2;
    }
    
    
    // WE BLINK A RECTANGLE AROUND THE ITEM CURRENTLY CHOSEN
    highlight_visible = !highlight_visible;
    for (int item = 0; item < MENU_ITEMS; item++){
        char slot_y1 = HEADER_HEIGHT + item * slot_h;
        
        
        // Coordinates of the selected item highlight
        if (item == selected_menu_index && highlight_visible){
            char top = slot_y1;
            char bottom = slot_y1 + slot_h - 1;
            
            
            // DRAW THE RECTANGLE           
            draw_rect_outline(0, top, OLED_WIDTH - 1, bottom);
            
            
            // Write to OLED
            push_oled_frame();
        }
        
        else{
            
            // Coordinates of the highlight
            char top = slot_y1;
            char bottom = slot_y1 + slot_h - 1;
            
            
            // Erase the highlight
            del_rect_outline(0, top, OLED_WIDTH - 1, bottom);
            
            
            // Write to OLED
            push_oled_frame();
        }
    }
}

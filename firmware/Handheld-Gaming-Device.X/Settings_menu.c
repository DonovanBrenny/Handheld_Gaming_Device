/* 
 * File:   Settings_screen.c
 * Author: Donovan Brenny
 *
 * Created on November 25, 2025
 */


#include "OLED_drawings.h"
#include "OLED.h"
#include "OLED_drawings.h"
#include "6x8_font_table.h"
#include "Joystick.h"
#include "Settings_menu.h"
#include "Buttons.h"

#define BRIGHTNESS_LEVELS 8
#define HEADER_HEIGHT 28
#define FOOTER_HEIGHT 12
#define SETTINGS_ITEMS 8
#define SETTINGS_LIST_H (OLED_HEIGHT - HEADER_HEIGHT - FOOTER_HEIGHT)


// The list of Settings items
const char *settings_items[SETTINGS_ITEMS] = {
    "Brightness",
    "",
    "",
    "Sound",
    "ON / OFF",
    "",
    "About",
    ""
};


// Tracks the screen brightness
static int brightness_index = 7; // start at 100%

// Tracks if sound in enabled
int sound_enabled = 1;

// List of brightness levels
static const unsigned char brightness_contrast[BRIGHTNESS_LEVELS] = {
    31, // 12.5%
    63, // 25%
    95, // 37.5%
    127, // 50%
    159, // 62.5%
    191, // 75%
    223, // 87.5%
    255 // 100%
};

// Tracks the selected setting item for the list
int selected_setting_index = 0;

// Highlight Tracker
static int highlight_visible = 0;


// We only want to select these in the Settings Menu / from the Setting_index
static const int selectable_settings[] = {0, 1, 4, 6};

// Tracker for which setting is chosen
static int settings_cursor = 0;

// Controls opening and closing of the about pop-up
int about_open = 0;

// Draws the HEADER for the Settings Menu
void settings_header(void) {
    
    // Center and Display the HEADER text
    const char *line1 = "SETTINGS";
    int x1 = center_text_position(line1);
    int y1 = FONT_6x8_HEIGHT;
    draw_text(x1, y1, line1);


    // Write to OLED
    push_oled_frame();

}


// Draws the FOOTER for the Settings Menu
void settings_footer(void) {

    
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


// Draw the Settings Menu item list
void settings_list(void) {

    // Height of a slot in the Settings list
    static int slot_height = SETTINGS_LIST_H / SETTINGS_ITEMS;
    if (slot_height < FONT_6x8_HEIGHT + 2) {
        slot_height = FONT_6x8_HEIGHT + 2;
    }


    // For each setting display the name with the y-axis value of slot_h * slot
    for (int item = 0; item < SETTINGS_ITEMS; item++) {
        
        /* WE NEED TO DISPLAY INTERACTIVE ITEMS ON CERTAIN LINES*/

        // Brightness Slider
        if (item == 1) {
            
            // Center and Display the "BRIGHTNESS" slider
            const char *name = settings_items[item];
            char slot_y1 = HEADER_HEIGHT + item * slot_height;
            char text_y = slot_y1 + (slot_height - FONT_6x8_HEIGHT) / 2;
            char text_x = center_text_position(name);
            draw_text(text_x, text_y, name);

            
            // Draw the BRIGHTNESS slider
            brightness_slider(slot_y1, slot_y1 + 8, 0);

            
            // Write to OLED
            push_oled_frame();
        }
        
        // SOUND ON/OFF 
        if (item == 4) {
            
            // Coordinates of the SOUND ON/OFF boxes
            int slot_y1 = HEADER_HEIGHT + item * slot_height;
            if(sound_enabled){
                on_off_box(slot_y1, slot_y1 + 8, 1);
            }
            else if(!sound_enabled){
                on_off_box(slot_y1, slot_y1 + 8, 0);
            }
        } 
  
        else {
            
            // Center and Display the other Settings items
            const char *name = settings_items[item];
            char slot_y1 = HEADER_HEIGHT + item * slot_height;
            char text_y = slot_y1 + (slot_height - FONT_6x8_HEIGHT) / 2;
            char text_x = center_text_position(name);
            draw_text(text_x, text_y, name);

            
            // Write to OLED
            push_oled_frame();

        }
    }
}


// Draws the BRIGHTNESS slider
void brightness_slider(int slot_y, int slot_h, int selected) {

    // Coordinates of the slider box outline
    int slider_x = center_text_position("        ");
    int slider_h = 8; // bar height
    int slider_y = slot_y + (slider_h / 2);
    int slider_w = 48;

    // Coordinates of the filled rectangle depending on BRIGHTNESS level
    int filled_w = ((brightness_index + 1) * slider_w) / BRIGHTNESS_LEVELS;
    int filled_x = slider_x;
    int filled_x2 = filled_w;
    int filled_y = slider_y + 1;
    int filled_y2 = slider_h - 2;

    // Coordinates for the knob at the end of the slider
    int knob_extra_h = 4;
    int knob_w = 3;
    int knob_x = filled_x + filled_w - knob_w + 1;
    int knob_y = slider_y - knob_extra_h / 2;
    int knob_h = slider_h + knob_extra_h;

    
    // Delete previous slider and re-draw the outline
    del_fill_rect(slider_x - 1, slider_y - knob_extra_h / 2 - 1, slider_w + 2, slider_h + knob_extra_h + 2);
    draw_rect_outline(slider_x, slider_y, slider_x + slider_w, slider_y + slider_h - 1);

    // Draw the new Brightness slider
    draw_fill_rect(filled_x, filled_y, filled_x2, filled_y2);

    
    // Only draw the knob at the end if the bar is selected
    if (selected && filled_w > 0) {
        
        draw_fill_rect(knob_x, knob_y, knob_w, knob_h);
    } 
    
    // Delete the end knob when the Brightness slider is not selected
    else if (!selected && filled_w > 0) {
        del_fill_rect(knob_x, knob_y, knob_w, knob_h);
        draw_rect_outline(slider_x, slider_y, slider_x + slider_w, slider_y + slider_h - 1);
        draw_fill_rect(filled_x, filled_y, filled_x2, filled_y2);
    }

}


// Changes the screen brightness
void apply_brightness(void) {

    set_oled_contrast(brightness_contrast[brightness_index]);
}


// Draws the on and off boxes
void on_off_box(int slot_y, int slot_h, int on_off) {

    // Coordinates for the SOUND ON/OFF boxes
    int box_w = 20;
    int box_h = 10;
    int box_y = slot_y;
    int on_x = center_text_position("ON / OFF") - 2;
    int off_x = on_x + 28;

    // If ON box selected
    if (on_off == 1) {
        
        // Blinks the text of the selected ON option
        del_fill_rect(off_x, box_y, box_w, box_h);
        draw_fill_rect(on_x, box_y, box_w, box_h);
        
        // Draw "ON" box and draw the "OFF" text
        del_text(on_x + 4, slot_y + 2, "ON");
        draw_text(off_x + 2, slot_y + 2, "OFF");
    }

    // If OFF box selected
    if (on_off == 0) {
        
        // Blinks the text of the selected OFF option
        del_fill_rect(on_x, box_y, box_w, box_h);
        draw_fill_rect(off_x, box_y, box_w, box_h);
        
        // Draw "OFF" box and draw the "ON" text
        del_text(off_x + 2, slot_y + 2, "OFF");
        draw_text(on_x + 4, slot_y + 2, "ON");
    }
}


// Updates the Settings Menu to check for button or joystick inputs
void ui_settings_update(void) {

    
    // Poll the joystick for inputs
    joystick_poll(0);

    
    // If joystick is pushed UP move UP the list
    if (up) {
        if (settings_cursor > 0) settings_cursor--;
    }
    
    // If joystick is pushed DOWN move DOWN the list
    else if (down) {
        if (settings_cursor < 3) settings_cursor++;
    }

    
    // We only move between 4 selectable settings
    selected_setting_index = selectable_settings[settings_cursor];

    //Height of setting slot in the list
    static int slot_h = SETTINGS_LIST_H / SETTINGS_ITEMS;
    if (slot_h < FONT_6x8_HEIGHT + 2) {
        slot_h = FONT_6x8_HEIGHT + 2;
    }

    
    int slot_y1 = HEADER_HEIGHT + 1 * slot_h;

    // If the brightness slider is selected
    if (selected_setting_index == 1) {
        int old_brightness = brightness_index;

        // Move joystick LEFT to adjust brightness DOWN
        if (left) {
            brightness_index--;
        } 
        
        // Move joystick RIGHT to adjust brightness UP
        else if (right) {
            brightness_index++;
        }

        // Capping the brightness index (MIN = 0 / MAX = 7)
        if (brightness_index < 0) brightness_index = 0;
        if (brightness_index > 7) brightness_index = 7;

        // Only apply brightness if it actually changed
        if (brightness_index != old_brightness) {
            apply_brightness();
        }

       // Selected -> draw slider with knob
        brightness_slider(slot_y1, slot_h, 1);
    } 
    
    // Not selected -> draw slider with NO knob
    else {
        brightness_slider(slot_y1, slot_h, 0);
    }
    
    // Write to OLED
    push_oled_frame();


    // WE BLINK A RECTANGLE AROUND THE ITEM CURRENTLY CHOSEN
    highlight_visible = !highlight_visible;
    for (int item = 0; item < SETTINGS_ITEMS; item++) {
        char slot_y1 = HEADER_HEIGHT + item * slot_h;
        
        // If ABOUT isn't selected make sure there's no residual highlight
        if (selected_setting_index != 6) {
            if (item == 6) {
                
                // Coordinates of the ABOUT highlight
                char top = slot_y1 - 3;
                char bottom = slot_y1 + slot_h;

                // DELETE ANY RESIDUAL HIGHLIGHT
                del_rect_outline(39, top, 86, bottom);
                push_oled_frame(); // Send frames to OLED
            }
        }
        
        // If SOUND ON/OFF row is selected
        if (selected_setting_index == 4 && sound_enabled) {
            
            // Draws the "OFF" text if the box is around ON
            int on_slot_y = ((slot_y1 * 5) / 2) + 2;
            draw_text(68, 74, "OFF");
            push_oled_frame();

            // If sound is ON and joystick is pushed RIGHT sound becomes OFF
            if (right) {
                sound_enabled = 0;
                
                // Re-draw the selected ON box
                on_off_box(on_slot_y, on_slot_y + 8, 0);
            }

            // Blinks the ON text when sound is enabled
            if (highlight_visible) {
                draw_text(42, 74, "ON");
                push_oled_frame();
            } else if (!highlight_visible) {
                del_text(42, 74, "ON");
                push_oled_frame();
            }
        } 
        
        // If SOUND ON/OFF row is selected
        else if (selected_setting_index == 4 && !sound_enabled) {

            // Draws the "ON" text if the box is around OFF
            int off_slot_y = ((slot_y1 * 5) / 2) + 2;
            draw_text(42, 74, "ON");
            push_oled_frame();

            
            // If sound is OFF and joystick is pushed LEFT sound becomes ON
            if (left) {
                sound_enabled = 1;
                on_off_box(off_slot_y, off_slot_y + 8, 1);
            }

            
            // Blinks the OFF text when sound is disabled
            if (highlight_visible) {
                draw_text(68, 74, "OFF");
                push_oled_frame();
            } else if (!highlight_visible) {
                del_text(68, 74, "OFF");
                push_oled_frame();
            }
        }
        
        // Certifies no text disappears when blinking and we move off that row
        else if (selected_setting_index != 4) {
   
            if (sound_enabled) {
                del_text(42, 74, "ON");
                draw_text(68, 74, "OFF");
                push_oled_frame();
            } 
            
            else if (!sound_enabled) {
                del_text(68, 74, "OFF");
                draw_text(42, 74, "ON");
                push_oled_frame();
            }
        }


        // If ABOUT is selected blink a highlight box around it
        if (selected_setting_index == 6 && highlight_visible) {

            // Coordinates of the ABOUT highlight box
            if (item == 6) {
                char top = slot_y1 - 3;
                char bottom = slot_y1 + slot_h;

                // DRAW THE RECTANGLE
                draw_rect_outline(39, top, 86, bottom);

                
                // Write to OLED
                push_oled_frame();
            }
        }
        if (selected_setting_index == 6 && !highlight_visible) {

            // Coordinates of the ABOUT highlight box
            if (item == 6) {
                char top = slot_y1 - 3;
                char bottom = slot_y1 + slot_h;

                // ERASE THE RECTANGLE
                del_rect_outline(39, top, 86, bottom);

                
                // Write to OLED
                push_oled_frame();
            }
        }
    }
}


//Draws the ABOUT pop-up screen
void draw_about_popup(void){
    
    clear_oled();
    
    int about_x = 0;
    int about_y = 0;
    int about_w = 127;
    int about_h = 127;

    draw_rect_outline(about_x, about_y, about_w, about_h);
    
    // Draw the first line of the About page
    const char *line1 = "";
    int line1_x = center_text_position(line1);
    int line1_y = about_y + 4;
    draw_text(line1_x, line1_y, line1);
    
    
    // Draw the second line of the About page
    const char *line2 = "GAME DEVICE";
    int line2_x = center_text_position(line2);
    int line2_y = about_y + (2 * FONT_6x8_HEIGHT);
    draw_text(line2_x, line2_y, line2);
    
      // Draw the third line of the About page
    const char *line2_1 = "-----------------------";
    int line2_1_x = center_text_position(line2_1);
    int line2_1_y = about_y + ((5 * FONT_6x8_HEIGHT) / 2) + 4;
    draw_text(line2_1_x, line2_1_y, line2_1);

    // Draw the fourth line of the About page
    const char *line3 = "Game Device v1.0.0";
    int line3_x = center_text_position(line3);
    int line3_y = about_y + (4 * FONT_6x8_HEIGHT) + 2;
    draw_text(line3_x, line3_y, line3);

      // Draw the fifth line of the About page
    const char *line3_1 = "OLED SCREEN";
    int line3_1_x = center_text_position(line3_1);
    int line3_1_y = about_y + ((13 * FONT_6x8_HEIGHT) / 2);
    draw_text(line3_1_x, line3_1_y, line3_1);

    // Draw the sixth line of the About page
    const char *line4 = "16-bit MCU";
    int line4_x = center_text_position(line4);
    int line4_y = about_y + ((17 * FONT_6x8_HEIGHT) / 2);
    draw_text(line4_x, line4_y, line4);

    // Draw the seventh line of the About page
    const char *line5 = "PIC24FJ64GA002";
    int line5_x = center_text_position(line5);
    int line5_y = about_y + ((21 * FONT_6x8_HEIGHT) / 2);
    draw_text(line5_x, line5_y, line5);

    // Draw the eighth line of the About page
    const char *line6 = "64KB FLASH MEMORY";
    int line6_x = center_text_position(line6);
    int line6_y = about_y + ((25 * FONT_6x8_HEIGHT) / 2);
    draw_text(line6_x, line6_y, line6);

    // Draw the ninth line of the About page
    const char *line7 = "8KB RAM";
    int line7_x = center_text_position(line7);
    int line7_y = about_y + ((29 * FONT_6x8_HEIGHT) / 2);
    draw_text(line7_x, line7_y, line7);
    
    
    // Write to OLED
    push_oled_frame();
}

// Display the Settings Menu
void draw_settings_menu(void) {

    clear_oled();
    settings_header();
    settings_footer();
    settings_list();
}

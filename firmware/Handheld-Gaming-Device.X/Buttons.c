/* 
 * File:   Buttons.c
 * Author: Donovan Brenny
 *
 * Created on November 22, 2025
 */

#include "xc.h"
#include <stdint.h>
#include "Buttons.h"
#include "Speaker.h"

#define DEBOUNCE_TIME 3

static char red_stable = 1;
static char blue_stable = 1;
static char joy_stable = 1;

static char red_raw = 1;
static char blue_raw = 1;
static char joy_raw = 1;

static char red_count = 0;
static char blue_count = 0;
static char joy_count = 0;

// Checks for button presses
char buttons_update(void){
    
    // Raw RED press state
    if(RED_PIN == 0) {
        red_raw = 0;
    }
    else{
        red_raw = 1;
    }
    
    // Raw BLUE press state
    if(BLUE_PIN == 0){
        blue_raw = 0;
    }
    else{
        blue_raw = 1;
    }
    
    if(JOY_PIN == 0){
        joy_raw = 0;
    }
    else{
        joy_raw = 1;
    }
    
    // Default even is no press
    char event = NAV_NONE;

    // Debounces are counted so we ignore the first few till it's stable
    if (blue_raw != blue_stable){
        
        if(++blue_count >= DEBOUNCE_TIME){
            blue_stable = blue_raw;
            blue_count = 0;
            
            if(blue_stable == 0){
                beep(120,250);
                event = NAV_SELECT;
            }
        }
    }
    
    // Reset blue count
    else {  
        blue_count = 0;
    }
    
    
     // Debounces are counted so we ignore the first few till it's stable
    if (red_raw != red_stable){
        if(++red_count >= DEBOUNCE_TIME){
            red_stable = red_raw;
            red_count = 0;
            
            if(red_stable == 0){
                beep(2000,250);
                event = NAV_BACK;
            }
        }
    }
    
    // Reset the red count
    else {  
        red_count = 0;
    }
    
    
    // Debounces are counted so we ignore the first few till it's stable
    if (joy_raw != joy_stable){
        
        if(++joy_count >= DEBOUNCE_TIME){
            joy_stable = joy_raw;
            joy_count = 0;
            
            if(joy_stable == 0){
                beep(1000,250);
                event = JOY_CLICK;
            }
        }
    }
    
    // Reset joystick count
    else {  
        joy_count = 0;
    }

    
    // Return the press event
    return event;
}


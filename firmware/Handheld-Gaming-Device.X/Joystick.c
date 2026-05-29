/*
 * File:   Joystick.c
 * Author: Donovan Brenny
 *
 * Created on November 21, 2025
 */


#include "xc.h"
#include <stdint.h>
#include "game_ASM.h"
#include "Joystick.h"


#define JOY_CENTER 512
#define JOY_DEADZONE 12

// Initialize the read channel
static int read_joystick_channel(int channel);

// Raw axis levels
int x_axis_raw = 512;
int y_axis_raw = 512;

// Initial states of directions
int up = 0;
int down = 0;
int left = 0;
int right = 0;


// Initialize the joystick
void joystick_init(void){
    
    AD1CON1 = 0;
    AD1CON1bits.FORM = 0;
    AD1CON1bits.SSRC = 0b111;
    AD1CON1bits.ASAM = 0;
    
    // Sample time
    AD1CON3bits.SAMC = 16;    
    
    // Tad = (ADCS+1)*Tcy
    AD1CON3bits.ADCS = 2;     

    // Will set CH0SA in joystick_read_channel
    AD1CHS = 0;

    // Clear interrupt flag
    IFS0bits.AD1IF = 0;
    
    // We are polling, no interrupt
    IEC0bits.AD1IE = 0;

    // Turn on ADC
    AD1CON1bits.ADON = 1;
}


// Read the joystick
static int read_joystick_channel(int channel){
    
    // Select ANx
    AD1CHSbits.CH0SA = channel;

    AD1CON2 = 0;
    
    // Start sampling
    AD1CON1bits.SAMP = 1;
    
    // Small sample delay
    st_5us_delay();
    
    // Start conversion
    AD1CON1bits.SAMP = 0;

    // Wait for conversion
    while (!AD1CON1bits.DONE);   
    
    // Clear DONE
    AD1CON1bits.DONE = 0;

    return ADC1BUF0;
}


// Poll the joystick
void joystick_poll(int joystick){   // 0 for left, 1 for right
    
    // Variables for the change in the X / y directions of the joystick
    int x_change;
    int y_change;
    
    // Read the joystick data
    if (joystick){
        x_axis_raw = read_joystick_channel(JOY_X_CH_R);
        y_axis_raw = read_joystick_channel(JOY_Y_CH_R);
    }
    if (!joystick){
        x_axis_raw = read_joystick_channel(JOY_X_CH_L);
        y_axis_raw = read_joystick_channel(JOY_Y_CH_L);
    }
    
    // Change in the X / y directions of the joystick
    x_change = (x_axis_raw - JOY_CENTER);
    y_change = (y_axis_raw - JOY_CENTER);
    
    // SET / RESET the direction states
    up = 0;
    down = 0;
    left = 0;
    right = 0;
    
    /*IF X / Y CHANGE IS GREATER THAN DEADZONE SET NEW DIRECTION STATE */
    if(x_change > JOY_DEADZONE){
        right = 1;
        
    } else if (x_change < -JOY_DEADZONE) {
        left = 1;
    }

    if (y_change > JOY_DEADZONE) {
        down = 1;
        
    } else if (y_change < -JOY_DEADZONE) {
        up = 1;
    }
}

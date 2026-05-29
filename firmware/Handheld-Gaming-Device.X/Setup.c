/* 
 * File:   Setup.c
 * Author: Donovan Brenny
 *
 * Created on November 22, 2025
 */

#include "xc.h"
#include <stdio.h>
#include <stdlib.h>
#include "game_menu.h"
#include "Setup.h"
#include "Joystick.h"
#include "Speaker.h"
#include "OLED.h"
#include "OLED_drawings.h"
#include "6x8_font_table.h"
#include "game_ASM.h"

volatile char blink = 0; 

// BLINKING HIGHLIGHT TIMER
void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void) {
    
    IFS0bits.T1IF = 0;
    
    blink = 1;
}


/*INITIALIZE PIC24 I/O PINS*/
void setup(void){
    
    CLKDIVbits.RCDIV = 0;   // SET CLOCK SPEED 16-MHz
    
    AD1PCFG = 0x9fff;       // SET PINS TO DIGITAL
    AD1PCFGbits.PCFG4 = 0;  // RB2 = AN4 = left analog y
    AD1PCFGbits.PCFG5 = 0;  // RB3 = AN5 = left analog x
    AD1PCFGbits.PCFG12 = 0; // RB12 = AN12 = right analog x
    AD1PCFGbits.PCFG11 = 0; // RB13 = AN11 = right analog y
    
    /*INPUT DEVICES*/
    TRISBbits.TRISB2 = 1;   // ANAlOG LEFT JOYSTICK Y
    TRISBbits.TRISB3 = 1;   // ANALOG LEFT JOYSTICK X
    TRISBbits.TRISB12 = 1;  // ANALOG RIGHT JOYSTICK X
    TRISBbits.TRISB13 = 1;  // ANALOG RIGHT JOYSTICK Y
    TRISBbits.TRISB15 = 1;  // BLUE BUTTON 
    TRISBbits.TRISB14 = 1;  // RED BUTTON
    TRISBbits.TRISB4 = 1;   // JOYSTICK CLICK
    
    /*OUTPUT DEVICES*/
    TRISBbits.TRISB5 = 0;   // SPEAKER
    TRISBbits.TRISB6 = 0;   // OLED RESET
    TRISBbits.TRISB7 = 0;   // OLED DATA/COMMAND SELECT
    TRISBbits.TRISB8 = 0;   // OLED CHIP SELECT
    TRISBbits.TRISB9 = 0;   // OLED SERIAL DATA OUT (MOSI)
    TRISBbits.TRISB10 = 0;  // OLED SERIAL CLOCK
    
    /*NOT USED BUT NEEDS TO BE SET FOR SPI TO FUNCTION*/
    TRISBbits.TRISB11 = 1;  // OLED SERIAL DATA IN (MISO) 
    
    LATBbits.LATB5 = 0;     // Set speaker LOW to start
    LATBbits.LATB6 = 1;     // Set reset pin HIGH for reset IDLE.
    LATBbits.LATB7 = 0;     // DC = command by default
    LATBbits.LATB8 = 1;     // CS high (deselected)
    
    
    
    /* Unlock PPS */
    __builtin_write_OSCCONL(OSCCON & 0xBF); 

    
    RPOR4bits.RP9R = 7;     // Map SDO1 (function code 7) to RP9
    RPOR5bits.RP10R = 8;    // Map SCK1 Output (function code 8) to RP10
    RPINR20bits.SDI1R = 11;  // Map SDI1 (function code 6) to RP11
    RPINR20bits.SCK1R = 10; // Map SCK1 Input to RP10


    /* Lock PPS */
    __builtin_write_OSCCONL(OSCCON | 0x40);
    
    CNPU1bits.CN1PUE = 1;   // Joystick button
    CNPU1bits.CN11PUE = 1;  // Internal pull-up resistors for buttons
    CNPU1bits.CN12PUE = 1;
    
    joystick_init();
    
    init_speaker();
}


void blink_timer(void){

    T1CONbits.TON = 0;
    T1CONbits.TCS = 0;

    T1CONbits.TCKPS = 0b11;

    TMR1 = 0;
    PR1 = 20000;        // Speed of the highlight blinking

    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;

    T1CONbits.TON = 1;
}


// CN interrupt is used purely as a sleep wake source - clear and return.
void __attribute__((__interrupt__, __auto_psv__)) _CNInterrupt(void) {
    IFS1bits.CNIF = 0;
}


/* POWER OFF: blank the screen, stop peripherals, sleep the CPU.
 * The blue button wakes us back up via the CN interrupt, after which
 * the device performs a soft reset for a clean startup sequence. */
void power_off_device(void) {

    // Show a brief goodbye message
    clear_oled();
    const char *msg = "Goodbye";
    int x = center_text_position(msg);
    int y = (OLED_HEIGHT - FONT_6x8_HEIGHT) / 2;
    draw_text(x, y, msg);
    push_oled_frame();
    st_3s_delay();

    // Blank the panel and put the OLED to sleep
    clear_oled();
    push_oled_frame();
    oled_off();

    // Stop every timer that could wake us spuriously
    T1CONbits.TON = 0;          // Blink highlight timer
    T2CONbits.TON = 0;          // Speaker tone
    T3CONbits.TON = 0;          // Speaker duration
    T4CONbits.TON = 0;          // Game frame tick

    // Make sure the speaker line is held low (no DC across the piezo)
    LATBbits.LATB5 = 0;

    // Arm CN wake on the blue button (RB15 = CN11). The pull-up is already
    // enabled from setup(), so a press pulls the line low and trips CN.
    CNEN1bits.CN11IE = 1;
    IFS1bits.CNIF = 0;
    IEC1bits.CNIE = 1;

    // Enter sleep mode (PWRSAV #0). CN interrupt brings us back here.
    Sleep();

    // On wake, soft-reset the chip so the device starts fresh in setup().
    __asm__ volatile ("RESET");
}

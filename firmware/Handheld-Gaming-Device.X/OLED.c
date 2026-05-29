/*
 * File:   OLED.c
 * Author: Donovan Brenny (brenn840)
 *
 * Created on November 15, 2025
 */

#include "xc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "game_ASM.h"
#include "6x8_font_table.h"
#include "OLED.h"
#include "OLED_drawings.h"


#define DATA_CMD_BIT LATBbits.LATB7
#define CHIP_SEL_BIT LATBbits.LATB8

#define BUFFER_SIZE (OLED_WIDTH * (OLED_HEIGHT / 8))

char OLED_BUFFER[BUFFER_SIZE];  // LOAD EACH OLED FRAME INTO HERE


void init_spi(void){
    
    IFS0bits.SPI1IF = 0;        // Clear the Interrupt flag
    IEC0bits.SPI1IE = 0;        // Disable the interrupt
    
    
    SPI1STATbits.SPIEN = 0;     //disable SPI1 to configure
    
    
    SPI1CON1bits.MSTEN = 1;     // Master mode
    SPI1CON1bits.MODE16 = 0;    // 8-bit for OLED commands
    SPI1CON1bits.CKP   = 0;     // Clock idle low
    SPI1CON1bits.CKE   = 1;     // Data changes on active-to-idle
    SPI1CON1bits.SMP   = 0;     // Sample in middle
    
    SPI1CON1bits.PPRE = 0b10;   // primary prescale 4:1
    SPI1CON1bits.SPRE = 0b111;  // secondary prescale 1:1
    
    
    SPI1STATbits.SPIROV = 0;     // Clear overflow
    //SPI1STATbits.SPISIDL = 0;  // Continue in idle
    
    SPI1STATbits.SPIEN = 1;      //Enable SPI1
}


unsigned char spi_write(char data_out){
    
    volatile unsigned char dummy;
    
    if (SPI1STATbits.SPIROV) {      // If an overflow ever happens clear it
        dummy = SPI1BUF;            // read to reset the buffer
        SPI1STATbits.SPIROV = 0;    // clear overflow flag
    }
    
    while(SPI1STATbits.SPITBF);     // Wait until TX buffer can accept data
    SPI1BUF = data_out;             // Write to TX buffer data
    
    while (!SPI1STATbits.SPIRBF);   // Wait for RX byte
    return (char)SPI1BUF;           // Return the data (It's Useless)
}


void oled_reset(void){
    
    LATBbits.LATB6 = 0;         // Pull RESET low
    st_1ms_delay();             // 1 millisecond delay
    LATBbits.LATB6 = 1;         // Pull RESET high
}

void oled_write_cmd(char cmd){
    
    CHIP_SEL_BIT = 0;           // CHIP SELECT LOW TO SELECT OLED
    DATA_CMD_BIT = 0;           // DATA/COMMAND LOW FOR COMMAND
    
    spi_write(cmd);             // SEND COMMAND
    
    CHIP_SEL_BIT = 1;           // CHIP SELECT HIGH TO DESELECT OLED
    
}

void oled_write_data(char data){
    
    CHIP_SEL_BIT = 0;           // CHIP SELECT BIT LOW TO SELECT OLED
    DATA_CMD_BIT = 1;           // DATA/COMMAND HIGH FOR DATA
    
    spi_write(data);            // SEND DATA
    
    CHIP_SEL_BIT = 1;           // CHIP SELECT HIGH TO DESELECT OLED
}

void oled_write_data_block(char *data, int len){
    
    CHIP_SEL_BIT = 0;           // CHIP SELECT BIT LOW TO SELECT OLED
    DATA_CMD_BIT = 1;           // DATA/COMMAND HIGH FOR DATA
    
    for(int i = 0; i < len; i++){
        spi_write(data[i]);
    }
    
    CHIP_SEL_BIT = 1;           // CHIP SELECT HIGH TO DESELECT OLED
}

void init_oled(void){
    
    oled_reset();               // Reset OLED
    
    oled_write_cmd(0xAE);       // Display OFF (sleep mode)
    
    // Display start line
    oled_write_cmd(0xDC);
    oled_write_cmd(0x00);       // start line = 0

    // Set contrast
    oled_write_cmd(0x81);
    oled_write_cmd(0x2F);       // medium contrast

    // Set memory addressing mode (horizontal)
    oled_write_cmd(0x20);
    oled_write_cmd(0x00);

    // Segment remap & COM scan direction (orientation)
    oled_write_cmd(0xA0);       // normal SEG mapping
    oled_write_cmd(0xC0);       // COM0 -> COM127

    // Multiplex ratio (height - 1)
    oled_write_cmd(0xA8);
    oled_write_cmd(0x7F);       // 0x7F = 127 => 128 rows

    // Display offset (vertical shift)
    oled_write_cmd(0xD3);
    oled_write_cmd(0x00);       // no offset for Adafruit 128x128 board

    // DC-DC control (internal booster on)
    oled_write_cmd(0xAD);
    oled_write_cmd(0x81);       // internal regulator on (datasheet power-on default)

    // Precharge & VCOMH (reasonable defaults)
    oled_write_cmd(0xD9);
    oled_write_cmd(0x22);

    oled_write_cmd(0xDB);
    oled_write_cmd(0x35);

    // Display follows RAM
    oled_write_cmd(0xA4);

    // Normal (non-inverted) display
    oled_write_cmd(0xA6);

    st_100ms_delay();           // let power settle

    oled_write_cmd(0xAF);       // display ON
    
    
    st_100ms_delay();           // Wait 100 milliseconds before sending data  
}


void clear_oled(void){

    memset(OLED_BUFFER,0x00,sizeof(OLED_BUFFER));   // Set each frame in the BUFFER to blank

}

void set_oled_contrast(unsigned char value){
    
    oled_write_cmd(0x81);
    oled_write_cmd((char)value);
}

void oled_set_pixel(int x, int y, int light){
    
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT) return;    // OUT OF BOUNDS
    else if (x < 0 || y < 0) return;
    
    unsigned int index = x + (y/8) * OLED_WIDTH;    // Page indexing so divide y by 8
    
    int mask = 1 << (y & 7);              // 0 index the selected pixel
    
    if (light) {
        OLED_BUFFER[index] |= mask;     // Light = 1 to light pixel
    }
    else {
        OLED_BUFFER[index] &= ~mask;    // Light != 1 pixel stays dark
    }
}


void push_oled_frame(void){
    
    /*OLED STORES DATA IN PAGES(y) AND COLUMNS(x). 
     *USING PAGE INDEXING THERE IS 1 PIXEL/BIT PER PAGE AND 8 PER COLUMN
     *THE COLUMN BYTE SHOWS WHICH PIXELS ARE ON(1) or OFF(0)*/
    
    char pages = OLED_HEIGHT / 8;   // 8 pixels per page: 2048 pages
    
    for(int page = 0; page < pages; page++){
        
        oled_write_cmd(0xB0 | (page & 0x0F));       // Command to select a page

        
        unsigned int col = 0x00;        // Column counter

        oled_write_cmd(0x00 | (col & 0x0F));               // Set column low bits
        oled_write_cmd(0x10 | ((col >> 4) & 0x0F));        // Set column high bits

        
        char *pointer = &OLED_BUFFER[page *OLED_WIDTH];     // grabs memory data
        oled_write_data_block(pointer, OLED_WIDTH);         // writes a block of data
    }
}


void start_up_animation(void){

    const char *line1 = "GAME DEVICE";      // Title line at the top
    const char *line2 = "Donovan Brenny";   // Author line below the title

    int x1 = center_text_position(line1);   // Find center of line 1
    int x2 = center_text_position(line2);   // Find center of line 2

    int y_center = OLED_HEIGHT / 2;         // Center of Y-Axis

    int y1 = y_center - FONT_6x8_HEIGHT;    // Title sits just above the middle
    int y2 = y_center + (FONT_6x8_HEIGHT / 2);  // Author sits just below the middle

    draw_text(x1, y1, line1);               // Draw the title string on OLED
    draw_text(x2, y2, line2);               // Draw the author string on OLED

    push_oled_frame();                      // Push frames from OLED buffer

}

void power_up_sequence(void){
    
    init_spi();             // Initialize SPI 1
    init_oled();            // Initialize OLED
    
    clear_oled();           // Clear display
    st_10ms_delay();
    start_up_animation();   // Startup Animation
    st_3s_delay();          // Delay before showing the menu
    clear_oled();           // Clear display

}

void oled_off(void){
    // Send Display OFF command (0xAE)
    oled_write_cmd(0xAE);  
    st_100ms_delay();
}

void oled_on(void) {
    // Send Display ON command (0xAF)
    oled_write_cmd(0xAF);
}
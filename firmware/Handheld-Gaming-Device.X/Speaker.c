/*
 * File:   Speaker.c
 * Author: Donovan Brenny
 *
 * Created on December 9, 2025
 */


#include "xc.h"
#include <stdint.h>
#include <string.h>
#include "Speaker.h"
#include "Settings_menu.h"


#define FCY 16000000        // Instruction clock (16 MHz)


// Variable for beep duration in milliseconds (decremented by Timer 3 ISR)
volatile unsigned int beep_ms_remaining = 0;


// Lookup row: note name string and its frequency in Hertz
typedef struct {
    char note[4];
    const float frequency;
} music_chart;


// Three octaves of equal temperament notes, indexed by name string
const music_chart chart[36] = {         // Musical notes for three different octaves
    {"A1", 440/2},        {"A2", 440},        {"A3", 440*2},
    {"A1#", 466.16/2},    {"A2#", 466.16},    {"A3#", 466.16*2},
    {"B1", 493.88/2},     {"B2", 493.88},     {"B3", 493.88*2},
    {"C1", 523.25/2},     {"C2", 523.25},     {"C3", 523.25*2},
    {"C1#", 554.37/2},    {"C2#", 554.37},    {"C3#", 554.37*2},
    {"D1", 587.33/2},     {"D2", 587.33},     {"D3", 587.33*2},
    {"D1#", 622.25/2},    {"D2#", 622.25},    {"D3#", 622.25*2},
    {"E1", 659.26/2},     {"E2", 659.26},     {"E3", 659.26*2},
    {"F1", 698.46/2},     {"F2", 698.46},     {"F3", 698.46*2},
    {"F1#", 739.99/2},    {"F2#", 739.99},    {"F3#", 739.99*2},
    {"G1", 783.99/2},     {"G2", 783.99},     {"G3", 783.99*2},
    {"G1#", 830.61/2},    {"G2#", 830.61},    {"G3#", 830.61*2}
};


// Square wave generator: toggles RB5 on each Timer 2 interrupt to make sound
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
{
    LATBbits.LATB5 ^= 1;    // Toggle RB5 (HIGH/LOW or LOW/HIGH) each interrupt
    IFS0bits.T2IF = 0;      // Clear Timer 2 interrupt flag so we can get the next one
}


// Duration timer: counts down beep_ms_remaining once per millisecond
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void)
{
    // Only count down if a beep is currently active
    if (beep_ms_remaining > 0)
    {
        beep_ms_remaining--;        // One millisecond of beep time has passed

        // If we just hit zero, time to stop the tone
        if (beep_ms_remaining == 0)
        {
            T2CONbits.TON = 0;      // Turn off Timer 2: no more square wave
            LATBbits.LATB5 = 0;     // Force speaker line LOW (silence)
        }
    }
    IFS0bits.T3IF = 0;              // Clear Timer 3 interrupt flag so it can fire again
}


// Setup speaker timers
void init_speaker(void){

    // Timer 2: generates the audible square wave by toggling RB5
    T2CONbits.TON = 0;          // Timer 2 OFF while configuring
    T2CONbits.TCS = 0;          // Internal clock
    T2CONbits.TCKPS = 0b00;     // 1:1 prescaler for maximum frequency resolution
    TMR2 = 0;                   // Start counting from 0
    PR2 = 0;                    // Period set later by beep()
    IFS0bits.T2IF = 0;          // Clear pending interrupt
    IEC0bits.T2IE = 1;          // Enable Timer 2 interrupt


    // Timer 3: ticks at 1 kHz and decrements beep_ms_remaining
    T3CONbits.TON = 0;          // Timer 3 OFF while configuring
    T3CONbits.TCS = 0;          // Internal clock
    T3CONbits.TCKPS = 0b10;     // 1:64 prescaler
    TMR3 = 0;                   // Start counting from 0
    PR3 = 249;                  // (249+1) * 64 = 16000 cycles = 1 ms at 16 MHz
    IFS0bits.T3IF = 0;          // Clear pending interrupt
    IEC0bits.T3IE = 1;          // Enable Timer 3 interrupt

    T3CONbits.TON = 1;          // Start Timer 3 (always ticking once enabled)
}


// Generate a beep (MINIMUM FREQ_HZ = 120 / MAXIMUM FREQ_HZ = 4000)
void beep(unsigned int freq_hz, unsigned int duration_ms){

    // Respect the user's sound toggle from the Settings menu
    if (sound_enabled){

        // freq_hz = 0 or duration_ms = 0 means silence/stop
        if (freq_hz == 0 || duration_ms == 0){
            T2CONbits.TON = 0;          // Turn off Timer 2
            LATBbits.LATB5 = 0;         // Force speaker line LOW (silence)
            beep_ms_remaining = 0;      // Clear any remaining duration
            return;
        }

        // PR2 = FCY / (2 * freq) gives a full square wave at the requested frequency
        unsigned long pr2 = (unsigned long)FCY / ((unsigned long)2 * freq_hz);
        pr2 -= (unsigned long)1;        // PR is 0-indexed
        if (pr2 > 0xffff) pr2 = 0xffff; // Clamp to 16-bit Timer 2 range


        T2CONbits.TON = 0;              // Pause Timer 2 while we reload PR2
        TMR2 = 0;                       // Start from 0 so the first toggle is clean
        PR2 = (unsigned int)pr2;        // Apply the new period

        IFS0bits.T2IF = 0;              // Clear pending flag from the previous tone
        T2CONbits.TON = 1;              // Start Timer 2 ticking the square wave

        beep_ms_remaining = duration_ms;    // Timer 3 will silence the tone after N ms
    }
}


// Look up the frequency for a note name; returns 0 if the name is empty or unknown
float get_frequency(const char* note_name) {

    // Empty/null name means "rest" - caller should interpret 0 Hz as silence
    if (note_name == 0 || note_name[0] == 0) return 0.0f;

    // Linear search through the 36-entry chart
    for (int i = 0; i < 36; i++) {
        if (strcmp(chart[i].note, note_name) == 0) {
            return chart[i].frequency;
        }
    }
    return 0.0f;                        // Note name not found
}

/*
 * File:   game_main.c
 * Author: Donovan Brenny (brenn840)
 *
 * Created on November 15, 2025, 3:26 PM
 */


#include "xc.h"
#include <stdio.h>
#include <stdlib.h>
#include "game_ASM.h"
#include "OLED.h"
#include "game_menu.h"
#include "OLED_tests.h"
#include "main_menu.h"
#include "Setup.h"
#include "Buttons.h"
#include "Confirm_popup_menu.h"
#include "Settings_menu.h"
#include "Pong.h"
#include "Ackanoid.h"
#include "Dinosaur_game.h"
#include "Tetris.h"                 // Hosts the Maze raycaster (maze_*)


// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)

// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Fail-Safe Monitor Enabled
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

/*DEFINE STATE MACHINE AND GAME_STATES*/
typedef enum{
    
    STATE_STARTUP = 0,
    STATE_MAIN_MENU,
    STATE_GAME_RUNNING,
    STATE_GAME_MENU,
    STATE_SETTINGS_MENU,
    STATE_POWER_OFF,
    STATE_CONFIRM_POPUP
            
} game_state_t;

// State trackers
static game_state_t current_state;
static game_state_t previous_state;


int main(void) {

    // SETUP THE DEVICE
    setup();

    // Run start sequence
    current_state = STATE_STARTUP;
    previous_state = 0;

    // START the blinking timer
    blink_timer();

    // DEVICE LOOP
    while (1) {

        // EVERY LOOP CHECK FOR BUTTON INPUTS
        char curr_event = buttons_update();

        // SWITCH BETWEEN STATES
        switch (current_state) {

            case STATE_STARTUP:
                
                // power up animation
                power_up_sequence();
                
                // Change to menu
                current_state = STATE_MAIN_MENU;
                
                // Display User Interface
                draw_main_menu(); 
                break;

                
                
                
            /* MAIN MENU STATE */
            case STATE_MAIN_MENU:
                
                // Update every other loop
                if (blink == 1) {
                    blink = 0;
                    ui_menu_update();
                }
                
                // WHEN SELECT BUTTON IS PRESSED
                if (curr_event == NAV_SELECT) {

                    // Change to correct state based on selected menu item
                    switch (selected_menu_index) {

                        case MENU_GAMES:
                            draw_games_menu();
                            current_state = STATE_GAME_MENU;
                            break;

                        case MENU_SETTINGS:
                            draw_settings_menu();
                            current_state = STATE_SETTINGS_MENU;
                            break;

                        case MENU_POWEROFF:
                            confirm_index = 2;          // Power-off confirmation
                            no_yes_index = 0;
                            previous_state = current_state;
                            current_state = STATE_CONFIRM_POPUP;
                            draw_confirm_popup("Power off",
                                    "the device?", "", "", 1);
                            break;
                    }
                }
                break;

                
                
                
                
                
            /* GAME MENU STATE */
            case STATE_GAME_MENU:
                
                // Update every other loop
                if (blink == 1) {
                    blink = 0;
                    ui_game_update();
                }

                // WHEN SELECT BUTTON IS PRESSED
                if (curr_event == NAV_SELECT) {

                    // Draw a pop-up to confirm the selected game
                    switch (selected_game_index) {

                        case GAME_ACKANOID:
                            confirm_index = 1;
                            previous_state = current_state;
                            current_state = STATE_CONFIRM_POPUP;
                            draw_confirm_popup(" Start the game", "Ackanoid?","","",1);
                            break;

                        case GAME_PONG:
                            confirm_index = 1;
                            previous_state = current_state;
                            current_state = STATE_CONFIRM_POPUP;
                            draw_confirm_popup(" Start the game", "Pong?","","",1);
                            break;

                        case GAME_MAZE:
                            confirm_index = 1;
                            previous_state = current_state;
                            current_state = STATE_CONFIRM_POPUP;
                            draw_confirm_popup(" Start the game", "Maze?","","",1);
                            break;

                        case GAME_DINOSAUR:
                            confirm_index = 1;
                            previous_state = current_state;
                            current_state = STATE_CONFIRM_POPUP;
                            draw_confirm_popup(" Start the game", "Dinosaur?","","",1);
                            break;
                    }
                }

                // IF BACK BUTTON IS PRESSED 
                if (curr_event == NAV_BACK) {
                    no_yes_index = 0;
                    confirm_index = 0;
                    previous_state = current_state;
                    
                    // Pop-up appears to confirm return to MAIN MENU
                    current_state = STATE_CONFIRM_POPUP;
                    draw_confirm_popup("Go back to", "Main Menu?","","",1);
                }
                break;


                
                
                
                
            /* SETTINGS MENU STATE */
            case STATE_SETTINGS_MENU:

                // Update every other loop
                if (blink == 1) {
                    blink = 0;
                    ui_settings_update();
                }

                // WHEN SELECT BUTTON IS PRESSED AND ABOUT ROW IS CHOSEN
                if (curr_event == NAV_SELECT && selected_setting_index == 6) {

                    
                    about_open = 1;
                    draw_about_popup();
                    
                    while(about_open) {
                        
                        char about_event = buttons_update();
                        
                        if (about_event == NAV_BACK){
                            
                            about_open = 0;
                            draw_settings_menu();
                        }
                    }
                }


                // IF BACK BUTTON IS PRESSED
                if (curr_event == NAV_BACK) {
                    no_yes_index = 0;
                    previous_state = current_state;
                    
                    // Pop-up appears to confirm return to MAIN MENU
                    current_state = STATE_CONFIRM_POPUP;
                    draw_confirm_popup("Go back to", "Main Menu?","","",1);
                }
                break;


                
                
                
                
            /* POWER OFF STATE
             * Reached only if something routes here directly. The user-facing
             * path runs through STATE_CONFIRM_POPUP (confirm_index == 2),
             * which calls power_off_device() and resets the chip.            */
            case STATE_POWER_OFF:
                power_off_device();
                break;

                
                
                
                
                
            /* GAME RUNNNING STATE */
            case STATE_GAME_RUNNING:

                // Universal back-out chord: joystick click + red simultaneously
                if (PORTBbits.RB4 == 0 && PORTBbits.RB14 == 0){

                    timer4_pause();
                    previous_state = current_state;
                    confirm_index = 0;
                    clear_oled();
                    draw_confirm_popup("Go back to", "Main Menu?","","",1);
                    current_state = STATE_CONFIRM_POPUP;
                    break;
                }

                // Per-game step on the shared Timer 4 frame tick (~10ms)
                if (pong_tick) {
                    pong_tick = 0;

                    switch (selected_game_index) {

                        case GAME_PONG:
                            pong_step();
                            if (pong_winner() == 1) {
                                previous_state = current_state;
                                confirm_index = 0;
                                clear_oled();
                                draw_confirm_popup("LEFT PLAYER WINS!",
                                        "Return to Main Menu?","","No to play again",0);
                                current_state = STATE_CONFIRM_POPUP;
                            }
                            else if (pong_winner() == 0) {
                                previous_state = current_state;
                                confirm_index = 0;
                                clear_oled();
                                draw_confirm_popup("RIGHT PLAYER WINS!",
                                        "Return to Main Menu?","","No to play again",0);
                                current_state = STATE_CONFIRM_POPUP;
                            }
                            break;

                        case GAME_ACKANOID:
                            ackanoid_step();
                            if (ackanoid_is_over()) {
                                char ack_buf[16];
                                sprintf(ack_buf, "Score: %d", ackanoid_final_score);
                                previous_state = current_state;
                                confirm_index = 0;
                                clear_oled();
                                draw_confirm_popup("Game Over!", ack_buf,
                                        "Return to Main Menu?","No to play again",0);
                                current_state = STATE_CONFIRM_POPUP;
                            }
                            break;

                        case GAME_DINOSAUR:
                            dino_step();
                            if (dino_is_over()) {
                                char dino_buf[16];
                                sprintf(dino_buf, "Score: %d", dino_final_score);
                                previous_state = current_state;
                                confirm_index = 0;
                                clear_oled();
                                draw_confirm_popup("Game Over!", dino_buf,
                                        "Return to Main Menu?","No to play again",0);
                                current_state = STATE_CONFIRM_POPUP;
                            }
                            break;

                        case GAME_MAZE:
                            maze_step();
                            if (maze_is_over()) {
                                char maze_buf[20];
                                sprintf(maze_buf, "Time: %d.%ds",
                                        maze_elapsed_ms / 1000,
                                        (maze_elapsed_ms / 100) % 10);
                                previous_state = current_state;
                                confirm_index = 0;
                                clear_oled();
                                draw_confirm_popup("You Escaped!", maze_buf,
                                        "Return to Main Menu?","No to play again",0);
                                current_state = STATE_CONFIRM_POPUP;
                            }
                            break;
                    }
                }
                break;

                
                
                
                
                
            /* CONFIRM POP-UP STATE */
            case STATE_CONFIRM_POPUP:

                // Update every other loop
                if (blink == 1) {
                    blink = 0;
                    ui_confirm_update();
                }

                // WHEN SELECT BUTTON IS PRESSED
                if (curr_event == NAV_SELECT) {
                    
                    // IF NO IS SELECTED RETURN TO PREVIOUS STATE
                    if (no_yes_index == NO) {
                        // NO: cancel, go back to where we came from
                        current_state = previous_state;

                        switch (previous_state) {
                            case STATE_GAME_MENU:
                                draw_games_menu();
                                break;

                            case STATE_MAIN_MENU:
                                draw_main_menu();
                                break;

                            case STATE_SETTINGS_MENU:
                                draw_settings_menu();
                                break;
                                
                            case STATE_GAME_RUNNING:

                                clear_oled();

                                switch (selected_game_index) {

                                    case GAME_ACKANOID:
                                        if (ackanoid_is_over()) {
                                            game_ackanoid_start();
                                        } else {
                                            timer4_unpause();
                                        }
                                        break;

                                    case GAME_PONG:
                                        if (game_over) {
                                            game_pong_start();
                                        } else {
                                            timer4_unpause();
                                            pong_draw();
                                        }
                                        break;

                                    case GAME_DINOSAUR:
                                        if (dino_is_over()) {
                                            game_dinosaur_start();
                                        } else {
                                            timer4_unpause();
                                        }
                                        break;

                                    case GAME_MAZE:
                                        if (maze_is_over()) {
                                            game_maze_start();
                                        } else {
                                            timer4_unpause();
                                        }
                                        break;
                                }
                                break;

                            default:
                                break;
                        }
                    }
                    
                    // IF YES IS SELECTED
                    else{

                        // ENTER MAIN MENU STATE BASED ON CONFIRM_INDEX
                        if (confirm_index == 0){
                        current_state = STATE_MAIN_MENU;
                        draw_main_menu();
                        }

                        //ENTER GAME RUNNING STATE BASED ON CONFIRM_INDEX
                        else if (confirm_index == 1) {

                            switch (selected_game_index) {

                                case GAME_ACKANOID:
                                    current_state = STATE_GAME_RUNNING;
                                    game_ackanoid_start();
                                    break;

                                case GAME_PONG:
                                    current_state = STATE_GAME_RUNNING;
                                    game_pong_start();
                                    break;

                                case GAME_MAZE:
                                    current_state = STATE_GAME_RUNNING;
                                    game_maze_start();
                                    break;

                                case GAME_DINOSAUR:
                                    current_state = STATE_GAME_RUNNING;
                                    game_dinosaur_start();
                                    break;
                            }
                        }

                        // POWER OFF (confirm_index == 2)
                        // Calls Sleep() and then RESETs - does not return.
                        else if (confirm_index == 2) {
                            power_off_device();
                        }
                    }
                }
         

                // IF BACK IS SELECTED
                if (curr_event == NAV_BACK) {
                    
                    // Treat BACK as NO same as cancel
                    
                    no_yes_index = 0;
                    current_state = previous_state;

                    // RETURN TO PREVIOUS STATE
                    switch (previous_state) {
                        case STATE_GAME_MENU:
                            draw_games_menu();
                            break;

                        case STATE_MAIN_MENU:
                            draw_main_menu();
                            break;

                        case STATE_SETTINGS_MENU:
                            draw_settings_menu();
                            break;

                        case STATE_GAME_RUNNING:

                            clear_oled();

                            switch (selected_game_index) {

                                case GAME_ACKANOID:
                                    if (ackanoid_is_over()) {
                                        game_ackanoid_start();
                                    } else {
                                        timer4_unpause();
                                    }
                                    break;

                                case GAME_PONG:
                                    if (game_over) {
                                        game_pong_start();
                                    } else {
                                        timer4_unpause();
                                        pong_draw();
                                    }
                                    break;

                                case GAME_DINOSAUR:
                                    if (dino_is_over()) {
                                        game_dinosaur_start();
                                    } else {
                                        timer4_unpause();
                                    }
                                    break;

                                case GAME_MAZE:
                                    if (maze_is_over()) {
                                        game_maze_start();
                                    } else {
                                        timer4_unpause();
                                    }
                                    break;
                            }
                            break;

                        default:
                            break;
                    }
                }
                break;
        }
    };
}

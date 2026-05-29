/*
 * File:   game_menu.c
 * Author: Donovan Brenny (brenn840)
 *
 * Created on November 17, 2025
 */

#include "xc.h"
#include <stdio.h>
#include <stdlib.h>
#include "OLED.h"
#include "6x8_font_table.h"
#include "OLED_drawings.h"
#include "Ackanoid.h"
#include "Pong.h"
#include "Tetris.h"                 // Hosts the Maze raycaster
#include "Dinosaur_game.h"
#include "game_menu.h"
#include "Joystick.h"


/*THIS IS THE STRUCTURE FOR EACH GAME. A TITLE AND A START FUNCTION*/
typedef struct {
    const char *name;
    void (*start_fn)(void);
} game_entry_t;


/*INITIALIZE EACH START_GAME FUNCTION HERE*/
void game_ackanoid_start(void);
void game_pong_start(void);
void game_maze_start(void);
void game_dinosaur_start(void);


/*LIST GAMES HERE*/
static game_entry_t games[] = {

    {"Ackanoid", game_ackanoid_start},
    {"Pong",     game_pong_start},
    {"Maze",     game_maze_start},
    {"Dinosaur", game_dinosaur_start}
    // ADD MORE GAMES HERE
};


#define TEXT_OFFSET 6
#define HEADER_H 28
#define FOOTER_H 18
#define GAME_LIST_H (OLED_HEIGHT - HEADER_H - FOOTER_H)
#define NUM_GAMES (sizeof(games) / sizeof(games[0]))


// Highlight tracker
static int highlight_visible = 0;

// Tracks the selected game
int selected_game_index = 0;


// Draws the header for the Game Menu
void games_header(void){
    
    // Center and Display the HEADER text
    const char *line1 = "SELECT A GAME";
    int x1 = center_text_position(line1);
    int y1 = FONT_6x8_HEIGHT;  
    draw_text(x1, y1, line1);
    
    
    // Write to OLED
    push_oled_frame();
}


// Draws the footer for the Game Menu
void games_footer(void){
    
    // Center and Display the FOOTER text
    const char *line2 = "SELECT (B) BACK (R)";
    draw_text(0, OLED_HEIGHT - FONT_6x8_HEIGHT - 1, line2);

    
    // Write to OLED
    push_oled_frame();
}

// Display the list of games
void game_list(void){
    
    // Divide the number of games evenly into rectangles of height slot_h
    static int slot_h = GAME_LIST_H / NUM_GAMES;
    if (slot_h < FONT_6x8_HEIGHT + 2) {
        slot_h = FONT_6x8_HEIGHT + 2;
    }
    
    
    // For each game display the name with the y-axis value of slot_h * slot #
    for (int game = 0; game < NUM_GAMES; game++){
        const char *name = games[game].name;
        char slot_y1 = HEADER_H + game * slot_h;
        char text_y = slot_y1 + (slot_h - FONT_6x8_HEIGHT) / 2;
        draw_text(TEXT_OFFSET, text_y, name);
        
        
        // Write to OLED
        push_oled_frame();
    }
}


// Draw the entire Game Menu
void draw_games_menu(void){
    
    clear_oled();
    games_header();
    games_footer();
    game_list();
}


// Updates the Game menu to check for Button and Joystick inputs
void ui_game_update(void){
    
    
    // Poll the Joystick for inputs
    joystick_poll(0);

    
    // When Joystick is pushed UP move UP the list
    if(up) {
        if (selected_game_index > 0) selected_game_index--;
    } 
    
    
    // When Joystick is pushed DOWN move DOWN the list
    else if(down) {
        if (selected_game_index < NUM_GAMES - 1) selected_game_index++;
    }

    
    // Height of a slot in the list
    static int slot_h = GAME_LIST_H / NUM_GAMES;
    if (slot_h < FONT_6x8_HEIGHT + 2) {
        slot_h = FONT_6x8_HEIGHT + 2;
    }
    
    
    // WE BLINK A RECTANGLE AROUND THE GAME CURRENTLY CHOSEN
    highlight_visible = !highlight_visible;
    for (int game = 0; game < NUM_GAMES; game++){
        char slot_y1 = HEADER_H + game * slot_h;
        
        
        // Coordinates the selected game highlight
        if (game == selected_game_index && highlight_visible){
            char top = slot_y1;
            char bottom = slot_y1 + slot_h - 1;
            
            
            // DRAW THE RECTANGLE           
            draw_rect_outline(0, top, OLED_WIDTH - 1, bottom);
            
            
            // Send frames to OLED
            push_oled_frame();
        }
        
        else{
            
            // Coordinates of the selected game highlight
            char top = slot_y1;
            char bottom = slot_y1 + slot_h - 1;
            
           
            // Erase the highlight
            del_rect_outline(0, top, OLED_WIDTH - 1, bottom);
            
            
             // Write to OLED
            push_oled_frame();
        }
    }
}

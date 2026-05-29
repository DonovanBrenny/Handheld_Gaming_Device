/*
 * File:   Dinosaur_game.c
 * Author: Donovan Brenny
 *
 * Created on December 9, 2025
 *
 * Single-screen endless runner. The dinosaur stands on the left, the red button
 * triggers a gravity-driven jump, and cacti scroll in from the right. Hitting
 * a cactus ends the run; surviving longer raises the score and the speed.
 */


#include "xc.h"
#include <stdio.h>
#include <stdlib.h>
#include "Dinosaur_game.h"
#include "OLED.h"
#include "OLED_drawings.h"
#include "Buttons.h"
#include "Speaker.h"
#include "Pong.h"                       // For shared timer4_init()


/* GAMEPLAY CONSTANTS */
#define PERSON_WIDTH        8           // Dino sprite width in pixels
#define PERSON_HEIGHT       16          // Dino sprite height in pixels
#define FLOOR_Y             96          // Ground line y coordinate
#define GRAVITY             1           // Pixels of downward velocity added per frame
#define JUMP_VELOCITY      -10          // Initial upward velocity at jump start (negative = up)
#define MAX_OBSTACLES       2           // Maximum cacti on screen at once
#define BASE_SPEED          2           // Starting horizontal scroll speed
#define MAX_SPEED           6           // Cap on scroll speed
#define SPEED_RAMP_FRAMES   300         // Bump scroll_speed every N frames
#define SCORE_TICK_FRAMES   4           // Score increments every N frames


/* DINOSAUR STATE */
static int player_x;                    // Fixed left edge of the dino
static int player_y;                    // Top-left y of the dino (changes during jump)
static int player_vy;                   // Vertical velocity in pixels/frame (signed)
static int is_jumping;                  // 1 while the dino is airborne


/* OBSTACLE STATE */
typedef struct {
    int x;                              // Left edge x; -1 means inactive
    int w;                              // Cactus width
    int h;                              // Cactus height
} cactus_t;

static cactus_t cacti[MAX_OBSTACLES];   // Active cacti slots
static int      scroll_speed;           // Current pixel scroll speed per frame
static int      frame_counter;          // Frames elapsed since game start
static int      spawn_cooldown;         // Frames left until the next cactus may spawn


/* GLOBAL STATE EXPOSED TO device_main.c */
int dino_game_over;                     // 1 once a collision happens
int dino_final_score;                   // Snapshot of score at game over


/* FORWARD DECLARATIONS */
static void draw_dino(int erase);
static void draw_cactus(cactus_t *c, int erase);
static int  aabb_overlap(int ax, int ay, int aw, int ah,
                         int bx, int by, int bw, int bh);
static void update_score_display(void);


// Standard four-side AABB overlap test
static int aabb_overlap(int ax, int ay, int aw, int ah,
                        int bx, int by, int bw, int bh) {
    return (ax < bx + bw) && (ax + aw > bx) &&
           (ay < by + bh) && (ay + ah > by);
}


// Draw or erase the dinosaur sprite (a body block plus a small head bump)
static void draw_dino(int erase) {
    if (erase) {
        del_fill_rect(player_x, player_y, PERSON_WIDTH, PERSON_HEIGHT);
        del_fill_rect(player_x + 4, player_y - 3, 4, 3);    // head bump
    } else {
        draw_fill_rect(player_x, player_y, PERSON_WIDTH, PERSON_HEIGHT);
        draw_fill_rect(player_x + 4, player_y - 3, 4, 3);   // head bump
    }
}


// Draw or erase a single cactus, anchored to the floor line
static void draw_cactus(cactus_t *c, int erase) {
    int y = FLOOR_Y - c->h;
    if (erase) del_fill_rect(c->x, y, c->w, c->h);
    else       draw_fill_rect(c->x, y, c->w, c->h);
}


// Refresh the score number in the top-left HUD
static void update_score_display(void) {
    char buf[8];
    sprintf(buf, "%5d", dino_final_score);
    del_fill_rect(40, 0, 6 * 5, 8);
    draw_text(40, 0, buf);
}


// Initialize all state and draw the first frame
void game_dinosaur_start(void) {

    clear_oled();
    srand(TMR1);

    // Reset all game state
    dino_game_over = 0;
    dino_final_score = 0;
    frame_counter = 0;
    scroll_speed = BASE_SPEED;
    spawn_cooldown = 30;

    // Place the dino on the floor at the left
    player_x  = 16;
    player_y  = FLOOR_Y - PERSON_HEIGHT;
    player_vy = 0;
    is_jumping = 0;

    // Clear out any cacti from a previous run
    for (int i = 0; i < MAX_OBSTACLES; i++) cacti[i].x = -1;

    // Draw the static UI: score label and floor line
    draw_text(0, 0, "Score:");
    draw_hline(0, OLED_WIDTH - 1, FLOOR_Y);

    // Draw the dino's starting pose, push to the OLED, then start the frame timer
    draw_dino(0);
    update_score_display();
    push_oled_frame();

    timer4_init();                      // Shared 10ms frame tick (used by all games)
}


// One game frame
void dino_step(void) {

    // Erase the dino and every active cactus so we can redraw them moved
    draw_dino(1);
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (cacti[i].x != -1) draw_cactus(&cacti[i], 1);
    }

    // Blue button is active-low (pulled up). Start a jump only if grounded.
    if (!is_jumping && BLUE_PIN == 0) {
        is_jumping = 1;
        player_vy = JUMP_VELOCITY;
        beep(800, 60);                  // Short jump chirp
    }

    // Jump physics: integrate position then gravity, snap to floor on landing
    if (is_jumping) {
        player_y += player_vy;
        player_vy += GRAVITY;
        if (player_y + PERSON_HEIGHT >= FLOOR_Y) {
            player_y  = FLOOR_Y - PERSON_HEIGHT;
            player_vy = 0;
            is_jumping = 0;
        }
    }

    // Scroll active cacti left and recycle once fully off-screen
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (cacti[i].x != -1) {
            cacti[i].x -= scroll_speed;
            if (cacti[i].x + cacti[i].w < 0) cacti[i].x = -1;
        }
    }

    // Spawn a new cactus once the cooldown elapses and a slot is free
    if (spawn_cooldown > 0) {
        spawn_cooldown--;
    } else {
        for (int i = 0; i < MAX_OBSTACLES; i++) {
            if (cacti[i].x == -1) {
                cacti[i].x = OLED_WIDTH - 1;
                cacti[i].w = 4 + (rand() & 3);              // 4..7 px wide
                cacti[i].h = 8 + (rand() & 7);              // 8..15 px tall
                spawn_cooldown = 40 + (rand() % 60);        // 40..99 frame gap
                break;
            }
        }
    }

    // Bump the frame counter, then update score and speed at their own cadences
    frame_counter++;
    if ((frame_counter & (SCORE_TICK_FRAMES - 1)) == 0) {
        dino_final_score++;
        update_score_display();
    }
    if (frame_counter % SPEED_RAMP_FRAMES == 0 && scroll_speed < MAX_SPEED) {
        scroll_speed++;
    }

    // Collision check: dino vs each active cactus
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (cacti[i].x != -1 &&
            aabb_overlap(player_x, player_y, PERSON_WIDTH, PERSON_HEIGHT,
                         cacti[i].x, FLOOR_Y - cacti[i].h, cacti[i].w, cacti[i].h)) {
            dino_game_over = 1;
            beep(120, 400);             // Low "you died" tone
        }
    }

    // Redraw the dino, cacti, and floor line (cactus erase may have nicked it)
    draw_dino(0);
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (cacti[i].x != -1) draw_cactus(&cacti[i], 0);
    }
    draw_hline(0, OLED_WIDTH - 1, FLOOR_Y);

    push_oled_frame();
}


// Reports whether the run has ended
int dino_is_over(void) {
    return dino_game_over;
}

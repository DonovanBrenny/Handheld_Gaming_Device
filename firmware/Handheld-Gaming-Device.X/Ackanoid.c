/*
 * File:   Ackanoid.c
 * Author: Donovan Brenny
 *
 * Created on December 9, 2025
 *
 * Brick-breaker game. The paddle moves left/right on the bottom of the screen
 * via the left joystick. The ball bounces off the walls, paddle, and bricks.
 * Clearing all bricks advances the level. Three misses ends the run.
 */


#include "xc.h"
#include <stdio.h>
#include <stdlib.h>
#include "Ackanoid.h"
#include "OLED.h"
#include "OLED_drawings.h"
#include "Joystick.h"
#include "Speaker.h"
#include "Ball_physics.h"


// Forward declaration of the shared Timer 4 setup (lives in Pong.c).
// Avoid #including Pong.h so the static paddle_update below doesn't conflict
// with the public paddle_update declared there.
void timer4_init(void);


/* GAMEPLAY CONSTANTS */
#define BLOCK_W         16          // Brick width in pixels
#define BLOCK_H         8           // Brick height in pixels
#define BLOCK_VAL       10          // Points awarded per brick cleared

#define PADDLE_W        24          // Paddle width in pixels
#define PADDLE_H        3           // Paddle height in pixels

#define MAX_BRICKS      18          // Max bricks the level table can hold
#define MAX_LEVELS      3           // Number of hand-authored levels

#define PLAY_TOP        10          // Top edge of the playfield (below the HUD)
#define PLAY_BOTTOM     127         // Bottom edge (paddle line at the very bottom)
#define PLAY_LEFT       0           // Left wall x coordinate
#define PLAY_RIGHT      127         // Right wall x coordinate

#define START_LIVES     3           // Lives the player begins with

#define TICKS_PER_STEP  2           // Run physics every Nth shared 10ms tick
                                    // (halves ball pace while keeping the
                                    //  higher-resolution bounce angles)


/* GAME STATE */
static Ball ball;                   // The ball (value, not pointer)

static int paddle_x;                // Top-left x of the paddle
static int paddle_y;                // Top-left y of the paddle (fixed)

static int lifes;                   // Lives left in the current run
static int score;                   // Cumulative score across all levels
static int level_index;             // Which level is loaded (0..MAX_LEVELS-1)
static int blocks_left;             // Number of bricks still alive on this level
static int ball_speed_x;            // Base horizontal speed (grows per level)
static int ball_speed_y;            // Base vertical speed (grows per level)

int ackanoid_game_over;             // 1 once lifes hits 0
int ackanoid_final_score;           // Snapshot of score at game over


/* LEVEL TABLES (RAM working copy; refilled each level from the const init table)
 * Each row: {x, y, alive}.  alive == 0 means the brick has been cleared. */
static int bricks[MAX_BRICKS][3];

// Level 1: solid 3x5 wall near the top
static const int level1_init[MAX_BRICKS][3] = {
    {  8, 18, 1}, { 28, 18, 1}, { 48, 18, 1}, { 68, 18, 1}, { 88, 18, 1},
    {  8, 30, 1}, { 28, 30, 1}, { 48, 30, 1}, { 68, 30, 1}, { 88, 30, 1},
    {  8, 42, 1}, { 28, 42, 1}, { 48, 42, 1}, { 68, 42, 1}, { 88, 42, 1},
    {  0,  0, 0}, {  0,  0, 0}, {  0,  0, 0}        // unused slots
};

// Level 2: diamond layout
static const int level2_init[MAX_BRICKS][3] = {
    { 48, 18, 1},
    { 28, 30, 1}, { 48, 30, 1}, { 68, 30, 1},
    {  8, 42, 1}, { 28, 42, 1}, { 48, 42, 1}, { 68, 42, 1}, { 88, 42, 1},
    { 28, 54, 1}, { 48, 54, 1}, { 68, 54, 1},
    { 48, 66, 1},
    {  0,  0, 0}, {  0,  0, 0}, {  0,  0, 0}, {  0,  0, 0}, {  0,  0, 0}
};

// Level 3: full 3x6 grid
static const int level3_init[MAX_BRICKS][3] = {
    {  0, 18, 1}, { 20, 18, 1}, { 40, 18, 1}, { 60, 18, 1}, { 80, 18, 1}, {100, 18, 1},
    {  0, 30, 1}, { 20, 30, 1}, { 40, 30, 1}, { 60, 30, 1}, { 80, 30, 1}, {100, 30, 1},
    {  0, 42, 1}, { 20, 42, 1}, { 40, 42, 1}, { 60, 42, 1}, { 80, 42, 1}, {100, 42, 1}
};


/* FORWARD DECLARATIONS */
static void load_level(int which);
static void draw_bricks(void);
static void draw_hud(void);
static void update_hud(void);
static void paddle_update(void);
static void ball_collision(void);
static void respawn_ball(void);


// Copy a level's brick layout from flash into the RAM working table
static void load_level(int which) {

    // Pick the right source table based on the requested level index
    const int (*src)[3];
    if (which == 0)       src = level1_init;
    else if (which == 1)  src = level2_init;
    else                  src = level3_init;

    // Copy entries and tally how many bricks start alive
    blocks_left = 0;
    for (int i = 0; i < MAX_BRICKS; i++) {
        bricks[i][0] = src[i][0];
        bricks[i][1] = src[i][1];
        bricks[i][2] = src[i][2];
        if (bricks[i][2] == 1) blocks_left++;
    }
}


// Draw all alive bricks (used once per level load)
static void draw_bricks(void) {
    for (int i = 0; i < MAX_BRICKS; i++) {
        if (bricks[i][2] == 1) {
            draw_fill_rect(bricks[i][0], bricks[i][1], BLOCK_W, BLOCK_H);
        }
    }
}


// Draw the static HUD text and a horizontal line dividing it from the playfield
static void draw_hud(void) {
    draw_text(0, 0, "S:");                          // Score label
    draw_text(70, 0, "L:");                         // Lives label
    draw_hline(0, OLED_WIDTH - 1, PLAY_TOP - 1);    // Divider line below the HUD
    update_hud();
}


// Refresh the dynamic numbers in the HUD (score + lives)
static void update_hud(void) {

    // Local buffers for the printed values
    char score_buf[8];
    char lives_buf[4];

    // Format the values - score up to 9999, lives 0..9
    sprintf(score_buf, "%4d", score);
    sprintf(lives_buf, "%d", lifes);

    // Erase the previous numbers, then redraw
    del_fill_rect(14, 0, 6 * 4, 8);                 // score number area
    del_fill_rect(82, 0, 6 * 2, 8);                 // lives number area
    draw_text(14, 0, score_buf);
    draw_text(82, 0, lives_buf);
}


// Read the left joystick and move the paddle within the playfield bounds
static void paddle_update(void) {

    // Erase the previous paddle position
    del_fill_rect(paddle_x, paddle_y, PADDLE_W, PADDLE_H);

    // Sample the left joystick (channel 0)
    joystick_poll(0);

    // Move based on joystick direction, clamping to the playfield edges
    if (left  && paddle_x > PLAY_LEFT)                       paddle_x -= 3;
    if (right && paddle_x + PADDLE_W < PLAY_RIGHT)           paddle_x += 3;

    // Draw the paddle at its new position
    draw_fill_rect(paddle_x, paddle_y, PADDLE_W, PADDLE_H);
}


// Reset the ball to the center of the playfield with a fresh random direction
static void respawn_ball(void) {
    del_ball(ball.x, ball.y);
    ball_reset_center_random(&ball, OLED_WIDTH / 2, OLED_HEIGHT / 2,
                             ball_speed_x, ball_speed_y);
}


// Handle wall, paddle, and brick collisions for the ball this frame
static void ball_collision(void) {

    // Snapshot the ball's edges for AABB tests
    int ball_left   = ball.x;
    int ball_right  = ball.x + BALL_SIZE;
    int ball_top    = ball.y;
    int ball_bottom = ball.y + BALL_SIZE;

    // Top wall bounce (single-sided: only flip if moving up)
    if (ball_top <= PLAY_TOP && ball.vy < 0) {
        ball.y  = PLAY_TOP;
        ball.vy = -ball.vy;
    }

    // Left wall bounce
    if (ball_left <= PLAY_LEFT && ball.vx < 0) {
        ball.x  = PLAY_LEFT;
        ball.vx = -ball.vx;
    }

    // Right wall bounce
    if (ball_right >= PLAY_RIGHT && ball.vx > 0) {
        ball.x  = PLAY_RIGHT - BALL_SIZE;
        ball.vx = -ball.vx;
    }

    // Paddle bounce: the ball hits the top surface of the paddle
    if (ball.vy > 0 &&
        ball_bottom >= paddle_y &&
        ball_bottom <= paddle_y + PADDLE_H + 2 &&
        ball_right  >= paddle_x &&
        ball_left   <= paddle_x + PADDLE_W) {

        // Snap the ball above the paddle so it doesn't tunnel
        ball.y = paddle_y - BALL_SIZE;

        // Angle the bounce based on where on the paddle the ball hit.
        // is_top_paddle = 0 -> bottom paddle (ball sent upward). Passing the
        // vertical speed as the horizontal max caps edge hits at ~45 deg.
        ball_bounce_horizontal_paddle(&ball, paddle_x, PADDLE_W,
                                      0, ball_speed_y, ball_speed_y);

        beep(200, 60);                              // Short paddle thunk
    }

    // Missed the paddle - lose a life and respawn
    if (ball.y > PLAY_BOTTOM) {
        lifes--;
        beep(120, 250);                             // Low "you lost a life" tone
        update_hud();
        if (lifes <= 0) {
            ackanoid_game_over = 1;
            ackanoid_final_score = score;
            return;
        }
        respawn_ball();
        return;
    }

    // Brick collisions - first hit wins so the ball doesn't pass through walls
    for (int i = 0; i < MAX_BRICKS; i++) {
        if (bricks[i][2] != 1) continue;

        int bx = bricks[i][0];
        int by = bricks[i][1];

        // Standard AABB overlap test between ball and brick
        if (ball_right > bx && ball_left < bx + BLOCK_W &&
            ball_bottom > by && ball_top < by + BLOCK_H) {

            // Erase the brick from the OLED and mark it dead
            del_fill_rect(bx, by, BLOCK_W, BLOCK_H);
            bricks[i][2] = 0;
            blocks_left--;

            // Score + sound
            score += BLOCK_VAL;
            beep(600, 40);                          // Quick high pop
            update_hud();

            // Pick a flip axis based on which side of the brick was deeper
            int overlap_x = (ball.vx > 0)
                          ? (ball_right - bx)
                          : (bx + BLOCK_W - ball_left);
            int overlap_y = (ball.vy > 0)
                          ? (ball_bottom - by)
                          : (by + BLOCK_H - ball_top);
            if (overlap_x < overlap_y) {
                ball.vx = -ball.vx;
            } else {
                ball.vy = -ball.vy;
            }

            // Only handle one brick per frame to keep physics stable
            break;
        }
    }
}


// Initialize a fresh game and draw the first frame
void game_ackanoid_start(void) {

    clear_oled();
    srand(TMR1);

    // Reset run-wide state
    lifes = START_LIVES;
    score = 0;
    level_index = 0;
    ball_speed_x = 2;          // Min for natural bounce angles; ramps per level
    ball_speed_y = 2;
    ackanoid_game_over = 0;
    ackanoid_final_score = 0;

    // Position the paddle centered along the bottom
    paddle_x = (OLED_WIDTH - PADDLE_W) / 2;
    paddle_y = OLED_HEIGHT - PADDLE_H - 1;

    // Spawn the ball above the paddle
    ball_reset_center_random(&ball, OLED_WIDTH / 2, OLED_HEIGHT / 2,
                             ball_speed_x, ball_speed_y);

    // Build the first level table and draw the static UI
    load_level(level_index);
    draw_hud();
    draw_bricks();
    draw_fill_rect(paddle_x, paddle_y, PADDLE_W, PADDLE_H);
    draw_ball(ball.x, ball.y);
    push_oled_frame();

    // Reuse the shared frame-tick timer that Pong already owns
    timer4_init();
}


// One game frame: erase ball, move it, collide, advance level, redraw
void ackanoid_step(void) {

    // Throttle physics to every Nth shared tick to keep the ball pace slow
    static int frame_div = 0;
    if (++frame_div < TICKS_PER_STEP) return;
    frame_div = 0;

    // Erase the ball's old position
    del_ball(ball.x, ball.y);

    // Move the paddle based on joystick input
    paddle_update();

    // Move the ball
    ball_update_position(&ball);

    // Resolve all collisions for this frame
    ball_collision();

    // If the collision step ended the run, leave the final frame on screen
    if (ackanoid_game_over) {
        push_oled_frame();
        return;
    }

    // Advance to the next level once every brick is cleared
    if (blocks_left == 0) {
        level_index++;
        if (level_index >= MAX_LEVELS) level_index = 0;     // Loop levels
        if (ball_speed_x < 3) ball_speed_x++;               // Gentle ramp
        if (ball_speed_y < 3) ball_speed_y++;
        clear_oled();
        load_level(level_index);
        draw_hud();
        draw_bricks();
        respawn_ball();
    }

    // Draw the ball at its new position and push the frame to the OLED
    draw_ball(ball.x, ball.y);
    push_oled_frame();
}


// Reports whether the run has ended
int ackanoid_is_over(void) {
    return ackanoid_game_over;
}

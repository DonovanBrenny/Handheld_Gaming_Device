/*
 * File:   Tetris.c
 * Author: Donovan Brenny
 *
 * Created on December 9, 2025
 *
 * 2.5D raycasting maze game. Walks the player through a 16x16 grid maze
 * rendered as a first-person perspective on the 128x128 1-bit OLED.
 * Left joystick = walk forward/back. Right joystick = turn left/right.
 * Reach the exit cell to win - elapsed time is shown in the end popup.
 *
 * Implementation notes:
 *   * Positions stored in Q12 fixed-point (1 cell = 4096 units).
 *   * 64 angle steps (5.625 degrees each). 64 rays at 2 px wide each.
 *   * DDA traversal with precomputed sin/cos and reciprocal tables in flash.
 *   * Wall depth shown via dithered shading (1-bit display).
 *   * Game ticks every 6 shared Timer 4 frames (~60 ms = ~16 fps cap).
 */


#include "xc.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "Tetris.h"
#include "OLED.h"
#include "OLED_drawings.h"
#include "Joystick.h"
#include "Buttons.h"
#include "Pong.h"                       // For shared timer4_init() / pong_tick


// Frame buffer lives in OLED.c - the maze clears it page-aligned for speed
extern char OLED_BUFFER[];


/* RENDERING + GAMEPLAY CONSTANTS */
#define MAP_W           16              // Maze grid width in cells
#define MAP_H           16              // Maze grid height in cells
#define Q12             4096            // 1 cell expressed in Q12 fixed-point
#define RENDER_Y_TOP    8               // First y of the 3D render band (page-aligned)
#define RENDER_H        96              // Height of the 3D render band in pixels
#define RAY_COUNT       64              // Number of rays cast per frame
#define ANGLE_STEPS     64              // Number of quantized direction steps
#define FOV_STEPS       12              // Horizontal field of view (in angle steps, ~67 deg)
#define PLAYER_SPEED    900             // Forward/back motion per step (Q12, ~0.22 cells)
#define TURN_SPEED      3               // Angle steps turned per game tick
#define FRAME_MS        60              // Real time per game tick (matches throttling below)
#define TICKS_PER_STEP  6               // Step every N shared 10ms frame ticks


/* PUBLIC GAME STATE (declared extern in Tetris.h) */
int maze_won;                           // 1 once the player reaches the exit cell
int maze_elapsed_ms;                    // Elapsed game time in milliseconds


/* PLAYER STATE */
static int32_t  player_px;              // X position in Q12 cell units
static int32_t  player_py;              // Y position in Q12 cell units
static int      player_dir;             // Direction index 0..63
static int      tick_counter;           // Shared-tick counter for step throttling
static int      step_counter;           // Total game steps taken (for time/score)


/* MAZE LAYOUT - 16x16 grid stored as a bitmask per row (1 = wall, 0 = open).
 * MSB (bit 15) is column 0 so the bits read left-to-right.
 * Rebuilt every run by generate_maze(): rooms live on odd cells (1,3,..,13)
 * and the wall between two rooms is the even cell separating them. A
 * recursive-backtracker carves a perfect (always solvable) maze full of
 * branching corridors and dead ends. */
static uint16_t maze_map[MAP_H];


// Lookup: returns 1 if cell (x,y) is a wall, 0 otherwise.
#define MAP_AT(x,y) ((maze_map[(y)] >> (15 - (x))) & 1)


/* START AND EXIT CELLS */
#define START_CX        1               // Start column
#define START_CY        1               // Start row
#define START_DIR       16              // Start facing south (+Y)
#define EXIT_CX         13              // Exit column (far-corner room cell)
#define EXIT_CY         13              // Exit row   (far-corner room cell)


/* TRIG TABLE - sine in Q15 for 64 directions covering a full circle.
 * cos = sin(angle + 90 deg) = sin(angle + 16 steps). */
static const int16_t sin_q15[ANGLE_STEPS] = {
        0,   3212,   6393,   9512,  12539,  15446,  18204,  20787,
    23170,  25330,  27246,  28898,  30273,  31357,  32138,  32610,
    32767,  32610,  32138,  31357,  30273,  28898,  27246,  25330,
    23170,  20787,  18204,  15446,  12539,   9512,   6393,   3212,
        0,  -3212,  -6393,  -9512, -12539, -15446, -18204, -20787,
   -23170, -25330, -27246, -28898, -30273, -31357, -32138, -32610,
   -32767, -32610, -32138, -31357, -30273, -28898, -27246, -25330,
   -23170, -20787, -18204, -15446, -12539,  -9512,  -6393,  -3212
};


/* SHORTCUT cosine accessor (sin shifted by 90 degrees = 16 steps backward) */
static inline int16_t cos_q15(int angle) {
    return sin_q15[(angle + 16) & (ANGLE_STEPS - 1)];
}


/* INV-SIN TABLE - 4096 / |sin(angle)| in Q12, clamped to 131072 (32 cells)
 * when sin is zero. Used in the DDA "delta distance" math. */
static const int32_t inv_sin_q12[ANGLE_STEPS] = {
    131072,  41785,  20993,  14110,  10704,   8689,   7373,   6457,
      5793,   5299,   4926,   4644,   4433,   4280,   4176,   4116,
      4096,   4116,   4176,   4280,   4433,   4644,   4926,   5299,
      5793,   6457,   7373,   8689,  10704,  14110,  20993,  41785,
    131072,  41785,  20993,  14110,  10704,   8689,   7373,   6457,
      5793,   5299,   4926,   4644,   4433,   4280,   4176,   4116,
      4096,   4116,   4176,   4280,   4433,   4644,   4926,   5299,
      5793,   6457,   7373,   8689,  10704,  14110,  20993,  41785
};


/* INV-COS is INV-SIN shifted by 90 degrees (16 steps) */
static inline int32_t inv_cos_q12(int angle) {
    return inv_sin_q12[(angle + 16) & (ANGLE_STEPS - 1)];
}


/* FORWARD DECLARATIONS */
static void generate_maze(void);
static void maze_render_frame(void);
static void cast_ray(int col);
static void draw_wall_column(int x, int top, int bottom, int side, int shade);
static void draw_status_bar(void);
static void draw_minimap(void);


/* Carve a perfect maze into maze_map with an iterative recursive-backtracker.
 * Rooms sit on odd cells (1..13); the wall between two rooms is the even cell
 * separating them. Every room ends up reachable, so there is always a path
 * from the start room (1,1) to the exit room (EXIT_CX, EXIT_CY). */
static void generate_maze(void) {

    // Start with every cell solid
    for (int y = 0; y < MAP_H; y++) maze_map[y] = 0xFFFF;

    // Explicit DFS stack of packed cells (x in low nibble, y in high nibble)
    uint8_t stack[64];
    int sp = 0;

    // Open the start room and seed the stack
    int cx = 1, cy = 1;
    maze_map[cy] &= ~(1 << (15 - cx));
    stack[sp++] = (uint8_t)((cy << 4) | cx);

    while (sp > 0) {

        // Look at the room on top of the stack
        cx = stack[sp - 1] & 0x0F;
        cy = (stack[sp - 1] >> 4) & 0x0F;

        // Collect unvisited neighbor rooms (2 cells away, still walled off)
        int dirs[4];
        int n = 0;
        if (cy - 2 >= 1  && (maze_map[cy - 2] & (1 << (15 - cx))))   dirs[n++] = 0; // up
        if (cy + 2 <= 13 && (maze_map[cy + 2] & (1 << (15 - cx))))   dirs[n++] = 1; // down
        if (cx - 2 >= 1  && (maze_map[cy] & (1 << (15 - (cx - 2))))) dirs[n++] = 2; // left
        if (cx + 2 <= 13 && (maze_map[cy] & (1 << (15 - (cx + 2))))) dirs[n++] = 3; // right

        // Dead end - backtrack
        if (n == 0) { sp--; continue; }

        // Pick a random unvisited neighbor, knock down the wall between them
        int nx = cx, ny = cy, wx = cx, wy = cy;
        switch (dirs[rand() % n]) {
            case 0: ny = cy - 2; wy = cy - 1; break;
            case 1: ny = cy + 2; wy = cy + 1; break;
            case 2: nx = cx - 2; wx = cx - 1; break;
            case 3: nx = cx + 2; wx = cx + 1; break;
        }
        maze_map[wy] &= ~(1 << (15 - wx));   // open the wall between
        maze_map[ny] &= ~(1 << (15 - nx));   // open the neighbor room

        // Recurse into the neighbor
        stack[sp++] = (uint8_t)((ny << 4) | nx);
    }
}


// Initialize a fresh maze run and draw the first frame
void game_maze_start(void) {

    clear_oled();

    // Build a fresh random maze for this run (TMR1 gives a varying seed)
    srand(TMR1);
    generate_maze();

    // Start the player in the middle of the start cell, facing south
    player_px  = (START_CX * Q12) + (Q12 / 2);
    player_py  = (START_CY * Q12) + (Q12 / 2);
    player_dir = START_DIR;

    // Reset run state
    maze_won = 0;
    maze_elapsed_ms = 0;
    tick_counter = 0;
    step_counter = 0;

    // Paint the first frame and start the shared 10ms tick timer
    maze_render_frame();
    timer4_init();
}


// One game tick - throttled to ~60 ms via tick_counter
void maze_step(void) {

    // Throttle: only act on every Nth shared tick
    if (++tick_counter < TICKS_PER_STEP) return;
    tick_counter = 0;
    step_counter++;
    maze_elapsed_ms = step_counter * FRAME_MS;

    // Sample the left joystick for forward/back motion
    joystick_poll(0);
    int fwd = up ? 1 : (down ? -1 : 0);

    // Sample the right joystick for rotation
    joystick_poll(1);
    int turn = right ? 1 : (left ? -1 : 0);

    // Apply rotation immediately (wraps modulo ANGLE_STEPS)
    player_dir = (player_dir + turn * TURN_SPEED + ANGLE_STEPS) & (ANGLE_STEPS - 1);

    // Apply forward/back motion with axis-separated wall sliding
    if (fwd) {
        int32_t step  = (fwd > 0) ? PLAYER_SPEED : -PLAYER_SPEED;
        int32_t dx    = ((int32_t)cos_q15(player_dir) * step) >> 15;
        int32_t dy    = ((int32_t)sin_q15[player_dir] * step) >> 15;
        int32_t new_x = player_px + dx;
        int32_t new_y = player_py + dy;

        // Move along X only if the destination cell is open
        if (!MAP_AT((new_x >> 12), (player_py >> 12))) {
            player_px = new_x;
        }
        // Move along Y only if the destination cell is open
        if (!MAP_AT((player_px >> 12), (new_y >> 12))) {
            player_py = new_y;
        }
    }

    // Check for win condition
    if ((player_px >> 12) == EXIT_CX && (player_py >> 12) == EXIT_CY) {
        maze_won = 1;
    }

    // Redraw the world
    maze_render_frame();
}


// Reports whether the run has ended
int maze_is_over(void) {
    return maze_won;
}


// Legacy alias kept so any straggling references still link
void game_tetris_start(void) {
    game_maze_start();
}


// Draw one full perspective frame plus the HUD and minimap
static void maze_render_frame(void) {

    // Clear only the 3D render band (pages 1..12, 96 pixels tall starting at y=8)
    memset(&OLED_BUFFER[1 * OLED_WIDTH], 0, 12 * OLED_WIDTH);

    // Cast every ray and draw its wall column
    for (int c = 0; c < RAY_COUNT; c++) cast_ray(c);

    // Top status bar + bottom-right minimap go on top of the render
    draw_status_bar();
    draw_minimap();

    push_oled_frame();
}


// Cast a single ray and draw the wall column it produces
static void cast_ray(int col) {

    // Spread the FOV evenly across the rays so column 0 is the leftmost view edge
    int angle = (player_dir + (col - RAY_COUNT / 2) * FOV_STEPS / RAY_COUNT)
                & (ANGLE_STEPS - 1);

    // Direction components in Q15
    int32_t cos_a = cos_q15(angle);
    int32_t sin_a = sin_q15[angle];

    // Player's current cell coordinates and Q12 fractional offsets inside the cell
    int cellX = player_px >> 12;
    int cellY = player_py >> 12;
    int32_t fracX = player_px & 0x0FFF;
    int32_t fracY = player_py & 0x0FFF;

    // Step direction per axis (+/-1)
    int stepX = (cos_a >= 0) ? 1 : -1;
    int stepY = (sin_a >= 0) ? 1 : -1;

    // Distance from player to the next grid line on each axis (Q12)
    int32_t nextDistX = (stepX > 0) ? (Q12 - fracX) : fracX;
    int32_t nextDistY = (stepY > 0) ? (Q12 - fracY) : fracY;

    // Ray-travel distance per grid crossing (Q12, precomputed reciprocals)
    int32_t deltaDistX = inv_cos_q12(angle);
    int32_t deltaDistY = inv_sin_q12[angle];

    // Initial t-values: ray distance to first crossing on each axis (Q12)
    int32_t tMaxX = (nextDistX * deltaDistX) >> 12;
    int32_t tMaxY = (nextDistY * deltaDistY) >> 12;

    // DDA loop - step into the closer grid line until we hit a wall
    int side = 0;                       // 0 = vertical wall hit, 1 = horizontal
    int hit  = 0;
    int safety = MAP_W + MAP_H;         // upper bound on steps
    while (!hit && safety--) {
        if (tMaxX < tMaxY) {
            tMaxX += deltaDistX;
            cellX += stepX;
            side = 0;
        } else {
            tMaxY += deltaDistY;
            cellY += stepY;
            side = 1;
        }
        // Off the map - draw nothing for this column
        if (cellX < 0 || cellX >= MAP_W || cellY < 0 || cellY >= MAP_H) return;
        if (MAP_AT(cellX, cellY)) hit = 1;
    }
    if (!hit) return;

    // Distance to the wall along the ray (Q12). Subtract the last step we added.
    int32_t dist = (side == 0) ? (tMaxX - deltaDistX) : (tMaxY - deltaDistY);
    if (dist < (Q12 / 8)) dist = Q12 / 8;       // Clamp to avoid divide-by-tiny

    // Wall column height in pixels: RENDER_H / dist (with Q12 scale baked in)
    int32_t lineH = ((int32_t)RENDER_H << 12) / dist;
    if (lineH > RENDER_H) lineH = RENDER_H;

    // Center the wall column vertically in the render band
    int top    = RENDER_Y_TOP + (RENDER_H - (int)lineH) / 2;
    int bottom = top + (int)lineH;

    // Distance buckets map to four shading densities (0=outline, 3=solid)
    int shade;
    if      (dist < (1 * Q12)) shade = 3;
    else if (dist < (2 * Q12)) shade = 2;
    else if (dist < (4 * Q12)) shade = 1;
    else                       shade = 0;

    // Each ray is two pixels wide
    int sx = col * 2;
    draw_wall_column(sx,     top, bottom, side, shade);
    draw_wall_column(sx + 1, top, bottom, side, shade);
}


// Draw a single 1-px-wide vertical wall slice with depth shading
static void draw_wall_column(int x, int top, int bottom, int side, int shade) {

    if (top < RENDER_Y_TOP) top = RENDER_Y_TOP;
    if (bottom > RENDER_Y_TOP + RENDER_H) bottom = RENDER_Y_TOP + RENDER_H;

    // Always draw the top and bottom edges so wall outlines are visible
    oled_set_pixel(x, top,        1);
    if (bottom - 1 > top) oled_set_pixel(x, bottom - 1, 1);

    if (shade == 0) return;                     // Far walls - edges only

    // Fill body of the column based on shade and side
    for (int y = top + 1; y < bottom - 1; y++) {
        int draw;
        if (shade == 3) {
            // Solid fill for closest walls; horizontal walls get scanline pattern
            draw = (side == 0) ? 1 : ((y & 1) == 0);
        }
        else if (shade == 2) {
            // 50% checker dither for mid-distance
            draw = ((x + y) & 1) == 0;
        }
        else {
            // Sparse dither for far walls
            draw = ((x & 1) == 0) && ((y & 3) == 0);
        }
        if (draw) oled_set_pixel(x, y, 1);
    }
}


// Top status bar: current direction (compass letter) + elapsed time
static void draw_status_bar(void) {

    // Clear the top 8 px so the previous frame's text doesn't ghost
    memset(&OLED_BUFFER[0], 0, OLED_WIDTH);

    // Compass letter: N/E/S/W (4 sectors of 16 steps each)
    static const char compass[4] = { 'E', 'S', 'W', 'N' };
    char dir_buf[6];
    dir_buf[0] = 'D';
    dir_buf[1] = ':';
    dir_buf[2] = compass[(player_dir + 8) / 16 & 3];
    dir_buf[3] = 0;
    draw_text(0, 0, dir_buf);

    // Elapsed seconds on the right
    char time_buf[10];
    sprintf(time_buf, "T:%d.%ds",
            maze_elapsed_ms / 1000,
            (maze_elapsed_ms / 100) % 10);
    draw_text(OLED_WIDTH - 6 * 8, 0, time_buf);
}


// Bottom-right minimap: one pixel per cell, with player position blinking
static void draw_minimap(void) {

    // Anchor in the bottom-right corner with a 2-px margin
    int mx = OLED_WIDTH - MAP_W - 2;
    int my = OLED_HEIGHT - MAP_H - 2;

    // Clear the minimap region first so it always reflects current state
    for (int y = -1; y <= MAP_H; y++) {
        for (int x = -1; x <= MAP_W; x++) {
            oled_set_pixel(mx + x, my + y, 0);
        }
    }

    // Frame around the minimap (draw_rect_outline takes coordinates, not dimensions)
    draw_rect_outline(mx - 1, my - 1, mx + MAP_W, my + MAP_H);

    // One pixel per wall cell
    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            if (MAP_AT(x, y)) oled_set_pixel(mx + x, my + y, 1);
        }
    }

    // Exit cell marker (always on)
    oled_set_pixel(mx + EXIT_CX, my + EXIT_CY, 1);

    // Player marker plus a heading pixel pointing the way you face, so the
    // minimap's orientation matches the first-person view.
    static const signed char arrow_dx[8] = { 1, 1, 0, -1, -1, -1,  0,  1};
    static const signed char arrow_dy[8] = { 0, 1, 1,  1,  0, -1, -1, -1};
    int dotx = mx + (player_px >> 12);
    int doty = my + (player_py >> 12);
    int sector = ((player_dir + 4) >> 3) & 7;   // 0=E,1=SE,2=S,...,7=NE
    oled_set_pixel(dotx, doty, 1);
    oled_set_pixel(dotx + arrow_dx[sector], doty + arrow_dy[sector], 1);
}

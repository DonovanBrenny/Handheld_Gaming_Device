/* 
 * File:   Ball_physics.c
 * Author: Donovan Brenny
 *
 * Created on December 5
 */


#include "Ball_physics.h"
#include <stdlib.h>


// On ball reset choose a random horizontal direction and vertical speed
void ball_reset_center_random(Ball *b, int center_x, int center_y,
                              int speed_x, int speed_y) {
    
    b->x = center_x;
    b->y = center_y;

    // Left or right
    if (rand() & 1) {
        b->vx = speed_x;
    } else {
        b->vx = -speed_x;
    }
    
    // The range of vertical speed values
    int range = (speed_y * 2) + 1;
    
    // A random Y value from -speed_y to +speed_y
    b->vy = (rand() % range) - speed_y;

    // Guarantee some vertical motion
    if (b->vy == 0 && speed_y > 0) {
        b->vy = 1;
    }
}

// Moves the ball
void ball_update_position(Ball *b) {
    b->x += b->vx;
    b->y += b->vy;
}


void ball_bounce_top_bottom(Ball *b, int top_border, int bottom_border) {
    
    // Top wall
    if (b->y <= top_border && b->vy < 0) {
        b->y = top_border;
        b->vy = -b->vy;
    }

    // Bottom wall
    if (b->y >= bottom_border && b->vy > 0) {
        b->y = bottom_border;
        b->vy = -b->vy;
    }
}


/* PONG: VERTICAL PADDLE BOUNCE*/
void ball_bounce_vertical_paddle(Ball *b, int paddle_y, int paddle_h,
                        int is_left_paddle, int speed_x, int speed_y) {
    
    // Ball and paddle y-axis centers
    int ball_center_y = b->y + BALL_SIZE / 2;
    int paddle_center_y = paddle_y + paddle_h / 2;

    
    /* Distance from paddle center when the ball hits a paddle
     * (negative = hit toward top, positive = hit towards bottom)*/
    int distance = ball_center_y - paddle_center_y;

    // Paddle edges
    int max_distance = paddle_h / 2;
    if (distance > max_distance) distance = max_distance;
    if (distance < -max_distance) distance = -max_distance;

    // Map distance to vertical speed
    if (max_distance <= 0) {
        b->vy = 0;
    } else {
        b->vy = (distance * speed_y) / max_distance;
    }



    /*Since we use INT math rounding leads to velocity being 0 when close 
     * to the paddle center this prevents having 0 vertical movement */
    if (b->vy == 0 && distance != 0) {
        if (distance > 0) {
            b->vy = 1;
        } else b->vy = -1;
    }

    // Horizontal velocity always away from the paddle
    if(is_left_paddle){
        b->vx = speed_x;
    } else b->vx = -speed_x;
}


/* ACKANOID: HORIZONTAL PADDLE BOUNCE
 * The exit angle depends on WHERE the ball struck the paddle:
 *   - dead center  -> straight up (vx = 0)
 *   - toward edges -> peels off to that side, up to ~45 deg at the very edge
 * Vertical speed is kept constant so the ball always climbs at a healthy
 * rate and never crawls along nearly horizontal. speed_x_max sets how
 * sharp an edge hit can get (pass it equal to speed_y for a 45 deg max). */
void ball_bounce_horizontal_paddle(Ball *b, int paddle_x, int paddle_w,
                            int is_top_paddle, int speed_y, int speed_x_max) {

    // Hit position relative to the paddle center: -half (left) .. +half (right)
    int half = paddle_w / 2;
    if (half < 1) half = 1;

    int offset = (b->x + BALL_SIZE / 2) - (paddle_x + paddle_w / 2);
    if (offset >  half) offset =  half;
    if (offset < -half) offset = -half;

    // Horizontal speed scales linearly with the hit offset (center = 0 = up)
    b->vx = (offset * speed_x_max) / half;

    // Always send the ball back away from the paddle at full vertical speed
    b->vy = is_top_paddle ? speed_y : -speed_y;
}
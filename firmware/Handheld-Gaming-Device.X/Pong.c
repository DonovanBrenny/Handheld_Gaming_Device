/*
 * File:   Pong.c
 * Author: Donovan Brenny
 *
 * Created on November 18, 2025
 */



// Things to do: Add scoreboard and end the game when enough points are scored
// Things already done: Initializing game, paddle movement, interrupt for change of direction, ball movement

#include "xc.h"
#include <stdio.h>
#include <stdlib.h>
#include "Pong.h"
#include "OLED.h"
#include "OLED_drawings.h"
#include "Joystick.h"
#include "Speaker.h"
#include "Ball_physics.h"


#define PADDLE_W 4
#define PADDLE_H 30
#define WIN_SCORE 5

static Ball pong_ball;

static int center_x = OLED_WIDTH / 2;
static int center_y = OLED_HEIGHT / 2;

// Rectangles are drawn starting from top left corner
static int paddle1_x = 0;    // Left joystick controls. Starts hugging left edge
static int paddle1_y = (OLED_HEIGHT - PADDLE_H) / 2;

// Right joystick controls. Starts hugging right edge
static int paddle2_x = OLED_WIDTH - PADDLE_W - 1;
static int paddle2_y = (OLED_HEIGHT - PADDLE_H) / 2;

static int score_L = 0;
static int score_R = 0;

static int bounce_count = 0;

static void draw_score(void);

volatile int pong_tick = 0;

int game_over = 0;

// Interrupt is supposed to trigger every 5 interrupts (is that too long, each interrupt is 10ms)
void __attribute__((interrupt, no_auto_psv)) _T4Interrupt(void) {
    
    IFS1bits.T4IF = 0; // clear interrupt flag
    pong_tick = 1;
    
}

void pong_init(void) {
    
    game_over = 0;
    
    clear_oled();
    
    bounce_count = 0;

    // speed_x = 1, speed_y = 1 for a slow serve
    ball_reset_center_random(&pong_ball, center_x, center_y, 1, 1);

    timer4_init();

}


void ball_collision(void){

    // Ball dimensions (use the full sprite, not magic offsets)
    int ball_left   = pong_ball.x;
    int ball_right  = pong_ball.x + BALL_SIZE;
    int ball_top    = pong_ball.y;
    int ball_bottom = pong_ball.y + BALL_SIZE;

    // Top/Bottom Wall Collision Detection
    ball_bounce_top_bottom(&pong_ball, 0, OLED_HEIGHT - BALL_SIZE);


    // Paddle dimensions
    int pad1_top = paddle1_y;
    int pad1_bottom = paddle1_y + PADDLE_H;

    int pad2_top = paddle2_y;
    int pad2_bottom = paddle2_y + PADDLE_H;


    // Left Paddle Collision Detection
    if (ball_left <= paddle1_x + PADDLE_W)
        if (ball_bottom >= pad1_top && ball_top <= pad1_bottom) {

            bounce_count++;
            beep(200, 100);

            pong_ball.x = paddle1_x + PADDLE_W + 1;

            // Order matters: check the higher threshold first
            if (bounce_count > 10) {
                ball_bounce_vertical_paddle(&pong_ball, paddle1_y, PADDLE_H, 1, 4, 4);
            }
            else if (bounce_count > 5) {
                ball_bounce_vertical_paddle(&pong_ball, paddle1_y, PADDLE_H, 1, 3, 3);
            }
            else {
                ball_bounce_vertical_paddle(&pong_ball, paddle1_y, PADDLE_H, 1, 2, 2);
            }
        }

    // Right Paddle Collision Detection
    if (ball_right >= paddle2_x)
        if (ball_bottom >= pad2_top && ball_top <= pad2_bottom) {

            bounce_count++;
            beep(200, 100);

            pong_ball.x = paddle2_x - BALL_SIZE;

            // Order matters: check the higher threshold first
            if (bounce_count > 10) {
                ball_bounce_vertical_paddle(&pong_ball, paddle2_y, PADDLE_H, 0, 4, 4);
            }
            else if (bounce_count > 5) {
                ball_bounce_vertical_paddle(&pong_ball, paddle2_y, PADDLE_H, 0, 3, 3);
            }
            else {
                ball_bounce_vertical_paddle(&pong_ball, paddle2_y, PADDLE_H, 0, 2, 2);
            }
        }


    /* SCORING */
    if (ball_right < 0) {
        score_R++;
        draw_score();
        reset_ball();
        return;
    }

    if (ball_left > OLED_WIDTH) {
        score_L++;
        draw_score();
        reset_ball();
        return;
    }
}


// Draw the centered scoreboard at the top of the screen ("L - R")
static void draw_score(void) {
    char buf[8];
    sprintf(buf, "%d - %d", score_L, score_R);
    // Wipe the previous score region, then redraw
    del_fill_rect(OLED_WIDTH / 2 - 16, 0, 32, 8);
    int x = center_text_position(buf);
    draw_text(x, 0, buf);
}




void paddle_update(void){
    // Erase previous paddle states
    del_fill_rect(paddle1_x, paddle1_y, PADDLE_W, PADDLE_H);
    del_fill_rect(paddle2_x, paddle2_y, PADDLE_W, PADDLE_H);
    // Poll from left paddle
    joystick_poll(0);
    if (up && paddle1_y > 0){    // bounds for paddles
        paddle1_y -= 3;
    }
    if (down && paddle1_y + PADDLE_H < OLED_HEIGHT){
        paddle1_y += 3;
    }
    // Right paddle
    joystick_poll(1);
    if (up && paddle2_y > 0){
        paddle2_y -= 3;
    }
    if (down && paddle2_y + PADDLE_H < OLED_HEIGHT){
        paddle2_y += 3;
    }
    // Draw new paddle states
    draw_fill_rect(paddle1_x, paddle1_y, PADDLE_W, PADDLE_H);
    draw_fill_rect(paddle2_x, paddle2_y, PADDLE_W, PADDLE_H);

}

// Update the ball position
void ball_update(void){
 
    ball_update_position(&pong_ball);
}


void pong_draw(void){

    // Draw paddle1
    draw_fill_rect(paddle1_x, paddle1_y, PADDLE_W, PADDLE_H);
    //Draw paddle2
    draw_fill_rect(paddle2_x, paddle2_y, PADDLE_W, PADDLE_H);

    // Draw ball
    draw_ball(pong_ball.x, pong_ball.y);
    draw_score();
    push_oled_frame();
}

// Reset the ball
void reset_ball(void){
    
    bounce_count = 0;
    
    del_ball(pong_ball.x, pong_ball.y);
    
    ball_reset_center_random(&pong_ball, center_x, center_y, 1, 1);
    
    draw_ball(pong_ball.x, pong_ball.y);
}


// Called each loop/frame to run the game
void pong_step(void){
    
    paddle_update();
    del_ball(pong_ball.x, pong_ball.y);
    ball_update();
    ball_collision();
    draw_ball(pong_ball.x, pong_ball.y);
    push_oled_frame();
    
}

// Start the game
void game_pong_start(void){
    score_R = 0;
    score_L = 0;
    pong_init();
    pong_draw();
}

int pong_winner(void){

    if(score_L == WIN_SCORE){
        game_over = 1;
        timer4_pause();
        return 1;
    }
    else if(score_R == WIN_SCORE){
        game_over = 1;
        timer4_pause();
        return 0;
    }
    else{
        return 2;
    }
}


void timer4_init(void){
    T4CONbits.TON = 0;
    TMR4 = 0;
    T4CONbits.TCKPS = 0b11; // Prescaler of 256
    PR4 = 624;                    // 10ms period
    IFS1bits.T4IF = 0;
    IEC1bits.T4IE = 1;
    T4CONbits.TON = 1;
}


// Pause the game
void timer4_pause(void){
    T4CONbits.TON = 0;
}


// Unpause the game
void timer4_unpause(void){
    T4CONbits.TON = 1;
}




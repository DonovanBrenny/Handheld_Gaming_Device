/* 
 * File:   Ball_physics.h
 * Author: Donovan Brenny
 *
 * Created on December 5
 */

#ifndef BALL_PHYSICS_H
#define	BALL_PHYSICS_H

#ifdef	__cplusplus
extern "C" {
#endif

    
    #define BALL_SIZE 7

    typedef struct {
        int x;   // top-left x
        int y;   // top-left y
        int vx;  // velocity in x (pixels per update)
        int vy;  // velocity in y (pixels per update)
    } Ball;

    
    void ball_reset_center_random(Ball *b, int center_x, int center_y, int speed_x, int speed_y_max);
    void ball_update_position(Ball *b);
    void ball_bounce_top_bottom(Ball *b, int top_limit, int bottom_limit);
    void ball_bounce_vertical_paddle(Ball *b, int paddle_y, int paddle_h, int is_left_paddle, int speed_x, int speed_y_max);
    void ball_bounce_horizontal_paddle(Ball *b, int paddle_x, int paddle_w, int is_top_paddle, int speed_y, int speed_x_max);


#ifdef	__cplusplus
}
#endif

#endif	/* BALL_PHYSICS_H */


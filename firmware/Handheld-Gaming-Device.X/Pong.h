/* 
 * File:   Pong.h
 * Author: dbren
 *
 * Created on November 18, 2025, 9:36 PM
 */

#ifndef PONG_H
#define	PONG_H

#ifdef	__cplusplus
extern "C" {
#endif

    extern int game_over;
    extern volatile int pong_tick;
    void game_pong_start(void);
    void pong_init(void);
    void pong_draw(void);
    void paddle_update(void);
    void ball_update(void);
    void reset_ball(void);
    void timer4_init(void);
    void timer4_pause(void);
    void timer4_unpause(void);
    void pong_step(void);
    int pong_winner(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* PONG_H */


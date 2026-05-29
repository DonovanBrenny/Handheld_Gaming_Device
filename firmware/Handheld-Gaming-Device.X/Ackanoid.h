/*
 * File:   Ackanoid.h
 * Author: Donovan Brenny
 *
 * Created on December 9, 2025
 */

#ifndef ACKANOID_H
#define	ACKANOID_H

#ifdef	__cplusplus
extern "C" {
#endif

    // Game-over flag set when the player runs out of lives
    extern int ackanoid_game_over;

    // Final score surfaced to the end-of-game popup
    extern int ackanoid_final_score;


    // Public API used by the device main state machine
    void game_ackanoid_start(void);     // Initialize state and draw the first frame
    void ackanoid_step(void);           // Step one game frame
    int  ackanoid_is_over(void);        // Returns 1 once the player has lost

#ifdef	__cplusplus
}
#endif

#endif	/* ACKANOID_H */

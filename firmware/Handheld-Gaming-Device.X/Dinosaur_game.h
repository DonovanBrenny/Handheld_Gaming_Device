/*
 * File:   Dinosaur_game.h
 * Author: Donovan Brenny
 *
 * Created on December 9, 2025
 */

#ifndef DINOSAUR_GAME_H
#define	DINOSAUR_GAME_H

#ifdef	__cplusplus
extern "C" {
#endif

    // Game-over flag set when the dinosaur collides with a cactus
    extern int dino_game_over;

    // Final score surfaced to the end-of-game popup
    extern int dino_final_score;


    // Public API used by the device main state machine
    void game_dinosaur_start(void);     // Initialize state and draw the first frame
    void dino_step(void);               // Step one game frame
    int  dino_is_over(void);            // Returns 1 once the player has collided

#ifdef	__cplusplus
}
#endif

#endif	/* DINOSAUR_GAME_H */

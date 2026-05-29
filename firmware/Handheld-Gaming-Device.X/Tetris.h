/*
 * File:   Tetris.h
 * Author: Donovan Brenny
 *
 * Created on December 9, 2025
 *
 * This translation unit hosts the 2.5D Maze raycaster (see Tetris.c).
 * The file name is preserved so the MPLAB project structure stays untouched.
 */

#ifndef TETRIS_H
#define	TETRIS_H

#ifdef	__cplusplus
extern "C" {
#endif

    // Maze game state flags surfaced to device_main.c for the end-game popup
    extern int maze_won;            // 1 once the player reaches the exit cell
    extern int maze_elapsed_ms;     // total elapsed time in milliseconds

    // Maze public API used by the state machine
    void game_maze_start(void);     // initialize state and draw the first frame
    void maze_step(void);           // step one game tick (input + render)
    int  maze_is_over(void);        // returns 1 if the run has ended

    // Legacy stub kept so any straggling references still link
    void game_tetris_start(void);

#ifdef	__cplusplus
}
#endif

#endif	/* TETRIS_H */

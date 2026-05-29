/* 
 * File:   user_interface.h
 * Author: dbren
 *
 * Created on November 17, 2025, 1:02 AM
 */

#ifndef USER_INTERFACE_H
#define	USER_INTERFACE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    #define GAME_ACKANOID 0
    #define GAME_PONG     1
    #define GAME_MAZE     2
    #define GAME_DINOSAUR 3

    extern int selected_game_index;
    void ui_game_update(void);
    void draw_games_menu(void);
    

#ifdef	__cplusplus
}
#endif

#endif	/* USER_INTERFACE_H */


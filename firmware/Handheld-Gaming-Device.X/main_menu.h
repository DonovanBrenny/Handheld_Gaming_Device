/* 
 * File:   main_menu.h
 * Author: dbren
 *
 * Created on November 20, 2025
 */

#ifndef MAIN_MENU_H
#define	MAIN_MENU_H

#ifdef	__cplusplus
extern "C" {
#endif

    #define MENU_GAMES 0
    #define MENU_SETTINGS 1
    #define MENU_POWEROFF 2

    extern int selected_menu_index;
    void draw_main_menu(void);
    void ui_menu_update(void);


#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_MENU_H */


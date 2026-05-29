/* 
 * File:   device_main.h
 * Author: dbren
 *
 * Created on December 1, 2025, 10:49 AM
 */

#ifndef DEVICE_MAIN_H
#define	DEVICE_MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif


/*DEFINE STATE MACHINE AND GAME_STATES*/
typedef enum{

    STATE_STARTUP = 0,
    STATE_MAIN_MENU,
    STATE_GAME_RUNNING,
    STATE_GAME_MENU,
    STATE_SETTINGS_MENU,
    STATE_POWER_OFF,
    STATE_CONFIRM_POPUP

} game_state_t;


#ifdef	__cplusplus
}
#endif

#endif	/* DEVICE_MAIN_H */


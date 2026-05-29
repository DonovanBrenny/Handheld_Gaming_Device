/* 
 * File:   Buttons.h
 * Author: Donovan Brenny
 *
 * Created on November 22, 2025
 */

#ifndef BUTTONS_H
#define	BUTTONS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    #define NAV_NONE   0
    #define NAV_SELECT 1   // blue
    #define NAV_BACK   2   // red
    #define JOY_CLICK  3   // joystick click

    #define BLUE_PIN PORTBbits.RB15
    #define RED_PIN PORTBbits.RB14
    #define JOY_PIN PORTBbits.RB4
    
    char buttons_update(void);

#ifdef	__cplusplus
}
#endif

#endif	/* BUTTONS_H */


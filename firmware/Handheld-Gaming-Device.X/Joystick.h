/*
 * File:   Joystick.h
 * Author: Donovan Brenny
 *
 * Created on November 20, 2025
 */

#ifndef JOYSTICKS_H
#define	JOYSTICKS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    #define JOY_X_CH_L   5   // AN5  (RB3)
    #define JOY_Y_CH_L   4   // AN4  (RB2)
    #define JOY_X_CH_R  12   // AN12 (RB12)
    #define JOY_Y_CH_R  11   // AN11 (RB13)

    // Raw ADC values (0?1023)
    extern int x_axis_raw;
    extern int y_axis_raw;

    // Direction flags (1 = true, 0 = false)
    extern int up;
    extern int down;
    extern int left;
    extern int right;

    void joystick_init(void);
    void joystick_poll(int joystick);
    

#ifdef	__cplusplus
}
#endif

#endif	/* JOYSTICKS_H */


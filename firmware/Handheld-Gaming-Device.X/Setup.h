/* 
 * File:   Setup.h
 * Author: Donovan Brenny
 *
 * Created on November 22, 2025
 */

#ifndef SETUP_H
#define	SETUP_H

#ifdef	__cplusplus
extern "C" {
#endif

    extern volatile char blink;
    void setup(void);
    void blink_timer(void);
    void power_off_device(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SETUP_H */


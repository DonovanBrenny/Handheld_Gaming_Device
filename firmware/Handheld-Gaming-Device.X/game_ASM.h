/* 
 * File:   game_asm.h
 * Author: dbren
 *
 * Created on November 15, 2025, 7:10 PM
 */

#ifndef GAME_ASM_H
#define	GAME_ASM_H

#ifdef	__cplusplus
extern "C" {
#endif

    void st_5us_delay(void);
    void st_1ms_delay(void);
    void st_10ms_delay(void);
    void st_20ms_delay(void);
    void st_100ms_delay(void);
    void st_3s_delay(void);
    void st_250ms_delay(void);

#ifdef	__cplusplus
}
#endif

#endif	/* GAME_ASM_H */


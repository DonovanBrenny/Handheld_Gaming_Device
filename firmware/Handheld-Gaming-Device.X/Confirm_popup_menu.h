/* 
 * File:   Confirm_back_menu.h
 * Author: Donovan Brenny
 *
 * Created on November 22, 2025
 */

#ifndef CONFIRM_BACK_H
#define	CONFIRM_BACK_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    #define NO 0
    #define YES 1
    extern int no_yes_index;
    extern int confirm_index;
    void draw_confirm_popup(const char *line1, const char *line2,
        const char *line3, const char *line4, int border);
    void ui_confirm_update(void);

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIRM_BACK_H */


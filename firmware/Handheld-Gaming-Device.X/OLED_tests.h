/* 
 * File:   OLED_tests.h
 * Author: Donovan Brenny
 *
 * Created on November 18, 2025
 */

#ifndef OLED_TESTS_H
#define	OLED_TESTS_H

#ifdef	__cplusplus
extern "C" {
#endif


    void four_corners_test(void);
    void text_test(void);
    void draw_char_test(int x, int y, unsigned char c);
    void draw_text_test(int x, int y, const char *text);
    void start_up_animation_test(void);
    

#ifdef	__cplusplus
}
#endif

#endif	/* OLED_TESTS_H */


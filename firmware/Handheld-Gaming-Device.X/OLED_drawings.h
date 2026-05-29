/* 
 * File:   OLED_drawings.h
 * Author: Donovan Brenny
 *
 * Created on November 18, 2025
 */

#ifndef OLED_DRAWINGS_H
#define	OLED_DRAWINGS_H

#ifdef	__cplusplus
extern "C" {
#endif

    void draw_char(int x, int y, char c);
    void draw_text(int x, int y, const char *text);
    void del_char(int x, int y, char c);
    void del_text(int x, int y, const char *text);
    int chars_on_line(const char *s);
    int center_text_position(const char *line);
    void draw_hline(int x0, int x1, int y);
    void draw_vline(int x, int y0, int y1);
    void del_hline(int x0, int x1, int y);
    void del_vline(int x, int y0, int y1);
    void draw_rect_outline(int x, int y, int w, int h);
    void del_rect_outline(int x, int y, int w, int h);
    void draw_fill_rect(char x, char y, char w, char h);
    void del_fill_rect(char x, char y, char w, char h);
    void draw_ball(char x, char y);
    void del_ball(char x, char y);
    
#ifdef	__cplusplus
}
#endif

#endif	/* OLED_DRAWINGS_H */


/* 
 * File:   Settings_screen.h
 * Author: Donovan Brenny
 *
 * Created on November 25, 2025
 */

#ifndef SETTINGS_SCREEN_H
#define	SETTINGS_SCREEN_H

#ifdef	__cplusplus
extern "C" {
#endif

    #define BRIGHTNESS 0
    #define SOUND 1
    #define ABOUT 2

    extern int about_open;
    extern int selected_setting_index;
    extern int sound_enabled;
    
    void draw_settings_menu(void);
    void ui_settings_update(void);
    void on_off_box(int slot_y, int slot_h, int on_off);
    void apply_brightness(void);
    void brightness_slider(int slot_y, int slot_h, int selected);
    void draw_about_popup(void);
    

#ifdef	__cplusplus
}
#endif

#endif	/* SETTINGS_SCREEN_H */


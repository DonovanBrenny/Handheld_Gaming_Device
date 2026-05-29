/* 
 * File:   OLED.h
 * Author: dbren
 *
 * Created on November 15, 2025, 6:45 PM
 */

#ifndef OLED_H
#define	OLED_H

#ifdef	__cplusplus
extern "C" {
#endif

    #define OLED_WIDTH 128
    #define OLED_HEIGHT 128
    
    void init_spi(void);
    void init_oled(void);
    unsigned char spi_write(char data_out);
    void oled_write_cmd(char cmd);
    void oled_write_data(char data);
    void oled_write_data_block(char *data, int len);
    void oled_set_pixel(int x, int y, int light);
    void push_oled_frame(void);
    void start_up_animation(void);
    void oled_off(void);
    void oled_on(void);
    void clear_oled(void);
    void power_up_sequence(void);
    void set_oled_contrast(unsigned char value);

#ifdef	__cplusplus
}
#endif

#endif	/* OLED_H */


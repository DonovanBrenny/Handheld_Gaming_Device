/*
 * File:   Speaker.h
 * Author: Donovan Brenny
 *
 * Created on December 9, 2025
 */

#ifndef SPEAKER_H
#define	SPEAKER_H

#ifdef	__cplusplus
extern "C" {
#endif

    void init_speaker(void);
    void beep(unsigned int freq_hz, unsigned int duration_ms);
    float get_frequency(const char* note_name);


#ifdef	__cplusplus
}
#endif

#endif	/* SPEAKER_H */


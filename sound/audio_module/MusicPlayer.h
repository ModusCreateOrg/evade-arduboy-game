#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <stddef.h>
#include <inttypes.h>
#include <Arduino.h>
#include <avr/pgmspace.h>

extern void playMusic();
extern void stopMusic();

extern void playTone();

#endif

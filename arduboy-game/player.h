#ifndef PLAYER_H
#define PLAYER_H

#include "Arduboy.h"
#include "globals.h"

struct Player {
  byte x;
  byte y;
  byte width = 16;
  byte height = 16;
  byte frame;
  byte dying;
  byte gunTemp;

  void set() {
    x = 2;
    y = 32;
    frame = 2;
    dying = 0;
    gunTemp = 0;
  }

  void reset() {
    x = 2;
    y = 32;
    frame = 2;
    dying = 0;
    gunTemp = 0;
  }
};

#endif

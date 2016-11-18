#ifndef PLAYER_H
#define PLAYER_H

#include "Arduboy.h"
#include "globals.h"

struct Player {
  byte x;
  byte y;
  byte dying;
  int frame;

  void set() {
    x = 2;
    y = 32;
    frame = 2;
    dying = 0;
  }

  void reset() {
    x = 2;
    y = 32;
    frame = 2;
    dying = 0;
  }
};

#endif

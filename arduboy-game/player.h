#ifndef PLAYER_H
#define PLAYER_H

#include "Arduboy.h"
#include "globals.h"

struct Player {
  byte x;
  byte y;
  byte frame;

  void set() {
    x = 2;
    y = 32;
    frame = 2;
  }

  void reset() {
    x = 2;
    y = 32;
    frame = 2;
  }
};

#endif

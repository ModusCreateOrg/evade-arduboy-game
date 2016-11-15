#ifndef PLAYER_H
#define PLAYER_H

#include "Arduboy.h"
#include "globals.h"

struct Player {
  byte x;
  byte y;
  byte width;
  byte height;
  int frame;
  byte health;

  void set() {
    x = 2;
    y = 32;
    frame = 2;
    width = 16;
    height = 16;
    health = 100;
  }

  void reset() {
    x = 2;
    y = 32;
    frame = 2;
    health = 100;
  }
};

#endif

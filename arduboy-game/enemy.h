#ifndef ENEMY_H
#define ENEMY_H

#include "Arduboy.h"
#include "globals.h"
#include "bitmaps.h"

struct Enemy {
  int x;
  int y;
  int width;
  int height;
  int frame;
  int health;
  int type;
  const uint8_t *bitmap;

  void set(int _x, int _y, int _type) {
    x = _x;
    y = _y;
    width = 16;
    height = 16;
    type = _type;
    if(type == 1) {
      bitmap = enemy1;
    } else if(type == 2) {
      bitmap = enemy2;
    }
  }
};

#endif

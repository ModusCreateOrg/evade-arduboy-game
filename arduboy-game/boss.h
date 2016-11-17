#ifndef BOSS_H
#define BOSS_H

#include "Arduboy.h"
#include "globals.h"
#include "bitmaps.h"

struct Boss {
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
    type = _type;

    switch(type) {
      case 1:
        bitmap = boss1;
        width = 59;
        height = 53;
        health = 100;
        break;
      case 2:
        bitmap = boss2;
        width = 32;
        height = 16;
        health = 200;
        break;
      default:
        width = 59;
        height = 53;
        health = 100;
        bitmap = enemy1;
    }
  }
};

#endif

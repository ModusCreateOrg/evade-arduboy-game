#ifndef ENEMY_H
#define ENEMY_H

#include "Arduboy.h"
#include "globals.h"
#include "bitmaps.h"

struct Enemy {
  byte x;
  byte y;
  int health;
  byte type;
  // isMovingLeft (0), isMovingDown (1)
  byte direction;
  const uint8_t *bitmap;

  void set(byte _x, byte _y, byte _type) {
    x = _x;
    y = _y;
    type = _type;
    direction |= random(2) << 0;
    direction |= random(2) << 1;

    switch(type) {
      case 1:
        bitmap = enemy1;
        health = 25;
        break;
      case 2:
        bitmap = enemy2;
        health = 50;
        break;
      case 3:
        bitmap = enemy3;
        health = 100;
        break;
      default:
        bitmap = enemy1;
        health = 25;
    }
  }

  void move() {
    if (random(10) == 0) {
      changeDirection();
      
      int newX = x + (isMovingLeft() ? -1 : 1);
      int newY = y + (isMovingDown() ? -1 : 1);
      
      if ((newX >= MIN_ENEMY_SHIP_X) && (newX <= MAX_ENEMY_SHIP_X)) {
        x = newX;
      }
      if ((newY >= MIN_SHIP_Y) && (newY <= MAX_SHIP_Y)) {
        y = newY;
      }
    }
  }

  void changeDirection() {
    if (random(10) == 0) {
      direction ^= 1 << 0;
    }
    if (random(10) == 0) {
      direction ^= 1 << 1;
    }
  }

  boolean isMovingLeft() {
    return direction & (1 << 0);
  }

  boolean isMovingDown() {
    return direction & (1 << 1);
  }
};

#endif

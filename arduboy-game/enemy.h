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
  boolean isMovingLeft;
  boolean isMovingDown;
  const uint8_t *bitmap;

  void set(int _x, int _y, int _type) {
    x = _x;
    y = _y;
    width = 16;
    height = 16;
    type = _type;
    isMovingLeft = random(2) ? true : false;
    isMovingDown = random(2) ? true : false;

    switch(type) {
      case 1:
        bitmap = enemy1;
        health = 5;
        break;
      case 2:
        bitmap = enemy2;
        health = 10;
        break;
      case 3:
        bitmap = enemy3;
        health = 15;
        break;
      default:
        bitmap = enemy1;
    }
  }

  void move() {
    if (random(10) == 0) {
      changeDirection();
      
      int newX = x + (isMovingLeft ? -1 : 1);
      int newY = y + (isMovingDown ? -1 : 1);
      
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
      isMovingLeft = isMovingLeft ? 0 : 1;
    }
    if (random(10) == 0) {
      isMovingDown = isMovingDown ? 0 : 1;
    }
  }
};

#endif

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
  // isMovingLeft (0), isMovingDown (1)
  byte direction;
  const uint8_t *bitmap;

  void set(int _x, int _y, int _type) {
    x = _x;
    y = _y;
    type = _type;
    
    direction |= random(2) << 0;
    direction |= random(2) << 1;

    switch(type) {
      case 1:
        bitmap = boss1;
        width = 59;
        height = 53;
        health = 1000;
        break;
      case 2:
        bitmap = boss2;
        width = 32;
        height = 16;
        health = 2000;
        break;
      default:
        width = 59;
        height = 53;
        health = 100;
        bitmap = enemy1;
    }
  }

  void update() {
    move();
    draw();
  }

  void move() {
    changeDirection();
    
    if (type == 1) {
      if ((x > 69) && (inGameFrame % 5 == 0)) {
        x--;
      }
    } else {
      if (x > 110) {
        if (inGameFrame % 5 == 0) {
          x--;
        }
      } else {
        if (random(3) == 0) {
          byte newX = x + (isMovingLeft() ? -1 : 1);
          byte newY = y + (isMovingDown() ? -1 : 1);
          
          if ((newX >= 97) && (newX <= 109)) {
            x = newX;
          }
          if ((newY >= MIN_SHIP_Y) && (newY <= (MAX_SHIP_Y + 16 - height))) {
            y = newY;
          }
        }
      }
    }
  }

  void draw() {
    drawBitmap(x, y, bitmap, 0);
  }

  void changeDirection() {
    if (random(50) == 0) {
      direction ^= 1 << 0;
    }
    if (random(20) == 0) {
      direction ^= 1 << 1;
    }
  }

  boolean isMovingLeft() {
    return (direction & (1 << 0));
  }

  boolean isMovingDown() {
    return (direction & (1 << 1));
  }
};

#endif

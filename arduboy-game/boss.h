#ifndef BOSS_H
#define BOSS_H

#include "Arduboy.h"
#include "globals.h"
#include "bitmaps.h"

struct Boss {
  byte x;
  byte y;
  byte width;
  byte height;
  int health;
  byte type;
  
  Bullet bullets[5];
  // isMovingLeft (0), isMovingDown (1)
  byte direction;
  const uint8_t *bitmap;
  int animFrame;

  void set(int _x, int _y, int _type) {
    x = _x;
    y = _y;
    type = _type;

    for (byte i = 0; i < MAX_BOSS_BULLETS; i++) {
      bullets[i].hide();
    }
    
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

    if (((type == 1) && (x == 69)) || ((type == 2) && (x <= 110))) {
      for (byte i = 0; i < MAX_BOSS_BULLETS; i++) {
        if (!bullets[i].isVisible()) {
          fire(i);
        } else {
          bullets[i].update();
        }
      }
    }
  }

  void move() {
    changeDirection();
    
    if (type == 1) {
      if ((x > 69) && (inGameFrame % 5 == 0)) {
        x--;
      }
    } else {
      if (x > 110) {
        if (inGameFrame % 3 == 0) {
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
    if(type == 1) {
      drawBitmap(x, y, bitmap, 0);  
    } else {
      if(inGameFrame % 13 == 0) {
        animFrame++;
      }
      drawBitmap(x, y, bitmap, animFrame);
      if(animFrame > 2) {
        animFrame = 0;
      }
    }
    
  }

  void fire(byte bulletIndex) {
    if (type == 1) {
      bullets[bulletIndex].set(x, random(MIN_SHIP_Y, (MAX_SHIP_Y + 8)), false, 1, 0.4, false);
    } else if ((inGameFrame % 50 == 0) && (random(2) == 0)) {
      if (random(4) == 0) {
        bullets[bulletIndex].set(x, (y + (height / 2) - 1), false, 1, 0.6, true);
      } else {
        bullets[bulletIndex].set(x, (y + (height / 2) - 1), false, 1, 0.8, false);
      }
    }
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

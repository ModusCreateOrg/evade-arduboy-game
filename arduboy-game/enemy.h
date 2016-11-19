#ifndef ENEMY_H
#define ENEMY_H

#include "Arduboy.h"
#include "globals.h"
#include "bitmaps.h"

struct Enemy {
  byte x = 0;  // Use this to know if enemy is in play or not to avoid death logic issues
  byte y;
  int health;
  byte difficulty;
  byte dying;
  // isMovingLeft (0), isMovingDown (1)
  byte direction;
  const uint8_t *bitmap;

  void set(byte _x, byte _y) {
    x = _x;
    y = _y;
    dying = 0;
    
    direction |= random(2) << 0;
    direction |= random(2) << 1;

    byte typeRando = random(10);

    if (typeRando == 0) {
      bitmap = enemy3;
      health = 500;
      difficulty = 4;
    } else if (typeRando < 5) {
      bitmap = enemy2;
      health = 150;
      difficulty = 2;
    } else {
      bitmap = enemy1;
      health = 25;
      difficulty = 1;
    }
  }

  void move() {
    if (random(10 / difficulty) == 0) {
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
    if (random(30) == 0) {
      direction ^= 1 << 0;
    }
    if (random(10) == 0) {
      direction ^= 1 << 1;
    }
  }

  boolean doFire() {
    // Enemy can't fire whilst dying
    return (dying == 0 ? random(1000 / difficulty) == 0 : false);
  }

  boolean isMovingLeft() {
    return direction & (1 << 0);
  }

  boolean isMovingDown() {
    return direction & (1 << 1);
  }
};

#endif

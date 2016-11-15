/*
 * bullet
*/
#ifndef BULLET_H
#define BULLET_H

#include "Arduboy.h"
#include "globals.h"

struct Bullet {
  public:
    byte posX;
    byte posY;
    byte damage;
    byte speedX;
    boolean isVisible;

    void set(byte x, byte y) {
      posX = x;
      posY = y;
      damage = 1;
      speedX = 3;
      isVisible = true;
    }

    void update() {
      if (isVisible) {
        posX += speedX;
        
        if (posX > arduboy.width()) {
          isVisible = false;
        }
      }
    }

    void draw() {
      if (isVisible) {
        arduboy.fillRect(posX, posY, 2, 2, 1);
      }
    }
};

#endif

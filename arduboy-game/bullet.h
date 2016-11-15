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
    // Visibility (0), Direction (1)
    byte options;

    void set(byte x, byte y, boolean firedByPlayer) {
      posX = x;
      posY = y;
      damage = 1;
      speedX = firedByPlayer ? 3 : 1;
      options |= 1 << 0;
      options |= firedByPlayer << 1;
    }

    void update() {
      if (isVisible()) {
        if (isMovingRight()) {
          posX += speedX;
          if (posX > arduboy.width()) {
            options ^= 1 << 0;
          }
        } else {
          posX -= speedX;
          if (posX < 0) {
            options ^= 1 << 0;
          }
        }
      }
    }

    void draw() {
      if (isVisible()) {
        arduboy.fillRect(posX, posY, 2, 2, 1);
      }
    }

    boolean isVisible() {
      return options & (1 << 0);
    }

    boolean isMovingRight() {
      return options & (1 << 1);
    }
};

#endif

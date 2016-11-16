/*
 * bullet
*/
#ifndef BULLET_H
#define BULLET_H

#include "Arduboy.h"
#include "globals.h"
#include "bitmaps.h"

struct Bullet {
  public:
    byte posX;
    byte posY;
    byte damage;
    byte speedX;
    // Visibility (0), Direction (1)
    byte options;

    void set(byte x, byte y, boolean firedByPlayer, byte _damage) {
      posX = x;
      posY = y;
      damage = _damage;
      speedX = firedByPlayer ? 3 : 1;
      options |= 1 << 0;
      options |= firedByPlayer << 1;
    }

    void update() {
      if (isVisible()) {
        if (isMovingRight()) {
          posX += speedX;
          if (posX > arduboy.width()) {
            hide();
          }
        } else {
          posX -= speedX;
          if (posX <= 0) {
            hide();
          }
        }
      }
    }

    void draw() {
      if (isVisible()) {
        if (isMovingRight()) {
 
          drawBitmap(posX, posY, playerBulletA, 0);
        }
        arduboy.fillRect(posX, posY, 2, 2, 1);
      }
    }

    boolean isVisible() {
      return options & (1 << 0);
    }

    boolean isMovingRight() {
      return options & (1 << 1);
    }

    void hide() {
      if (isVisible()) {
        options ^= 1 << 0;
      }
    }
};

#endif

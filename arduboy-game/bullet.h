/*
 * bullet
*/
#ifndef BULLET_H
#define BULLET_H

#include "globals.h"
#include "bitmaps.h"
#include "bullet.h"

#define A_BULLET_DAMAGE 75
#define B_BULLET_DAMAGE 40

struct Bullet {
  public:
    float x;
    byte y;
    byte damage;
    float speedX;
    // Visibility (0), Direction (1)
    byte options;

    void set(byte _x, byte _y, boolean firedByPlayer, byte _damage, float _speedX) {
      x = _x;
      y = _y;
      damage = _damage;
      speedX = _speedX;
      options |= 1 << 0;
      options |= firedByPlayer << 1;

      draw();
    }

    void update() {
      move();
      draw();
    }

    void move() {
      if (isVisible()) {
        if (isMovingRight()) {
          x += speedX;
          if (x > arduboy.width()) {
            hide();
          }
        } else {
          x -= speedX;
          if (x <= 0) {
            hide();
          }
        }
      }
    }

    void draw() {
      if (isVisible()) {
        if (isMovingRight()) {
          drawBitmap(x, y, (damage == A_BULLET_DAMAGE ? playerBulletA : playerBulletB), 0);
        } else {
          drawBitmap(x, y, enemyBullet, 0);
        }
      }
    }

    boolean isHittingObject(byte objectX, byte objectY, byte objectWidth, byte objectHeight) {
      if ((isVisible()) &&
        (x >= objectX) &&
        (x <= (objectX + objectWidth)) &&
        (y >= objectY) &&
        (y <= (objectY + objectHeight))) {
          return true;
        } else {
          return false;
        }
    }

    boolean isVisible() {
      return (options & (1 << 0));
    }

    boolean isMovingRight() {
      return (options & (1 << 1));
    }

    void hide() {
      if (isVisible()) {
        options ^= 1 << 0;
      }
    }
};

#endif

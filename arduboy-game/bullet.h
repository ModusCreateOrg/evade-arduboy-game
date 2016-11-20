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
    // Visibility (0), Direction (1), Laser Beam (2)
    byte options;

    void set(byte _x, byte _y, boolean firedByPlayer, byte _damage, float _speedX, boolean isLaserBeam) {
      x = _x;
      y = _y;
      damage = _damage;
      speedX = _speedX;

      options &= ~(1 << 0);
      options &= ~(1 << 1);
      options &= ~(1 << 2);
      
      options |= 1 << 0;
      options |= firedByPlayer << 1;
      options |= isLaserBeam << 2;

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
          if ((!isLaserBeam()) && (x <= 0)) {
            hide();
          } else if ((isLaserBeam()) && (x <= -50)) {
            hide();
          }
        }
      }
    }

    void draw() {
      if (isVisible()) {
        if (isMovingRight()) {
          drawBitmap(x, y, (damage == A_BULLET_DAMAGE ? playerBulletA : playerBulletB), 0);
        } else if (!isLaserBeam()) {
          drawBitmap(x, y, enemyBullet, 0);
        } else {
          arduboy.fillRect(x, y, 50, 2, 1);
        }
      }
    }

    boolean isHittingObject(byte objectX, byte objectY, byte objectWidth, byte objectHeight) {
      if ((isVisible()) &&
        (((isLaserBeam()) && (x >= (objectX - 50))) || ((!isLaserBeam()) && (x >= objectX))) &&
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

    boolean isLaserBeam() {
      return (options & (1 << 2));
    }

    void hide() {
      if (isVisible()) {
        options ^= 1 << 0;
      }
    }
};

#endif

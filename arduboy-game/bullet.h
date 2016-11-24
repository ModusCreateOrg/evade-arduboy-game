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
    unsigned long inGameFireFrame;
    
    void set(byte _x, byte _y, boolean firedByPlayer, byte _damage, float _speedX, boolean isLaserBeam) {
      x = _x;
      y = _y;
      damage = _damage;
      speedX = _speedX;

      options &= ~(1 << 1);
      options &= ~(1 << 2);
      
      options |= 1 << 0;
      options |= firedByPlayer << 1;
      options |= isLaserBeam << 2;

      inGameFireFrame = inGameFrame;

      draw();
    }

    void update() {
      if (isVisible()) {
        move();
        draw();
      }
    }

    void move() {
      if (isMovingRight()) {
        x += speedX;
        if (x > 128) {
          hide();
        }
      } else {
        x -= speedX;
        if ((!isLaserBeam()) && (x <= 0)) {
          hide();
        } else if ((isLaserBeam()) && (x <= -30)) {
          hide();
        }
      }
    
    }

    void draw() {
  
        if (isMovingRight()) {
          const bool isBulletA = (damage == A_BULLET_DAMAGE);
//          const int tone = ;
          playTone(isBulletA ? 600 - (x + 2) : 900 - (x + 3) - x, 10);
//          arduboy.tunes.tone(tone - x, 10);
          drawBitmap(x, y, (isBulletA ? playerBulletA : playerBulletB), 0);
        } else if (!isLaserBeam()) {
          drawBitmap(x, y, enemyBullet, 0);
        } else {
          arduboy.fillRect(x, y, min((inGameFrame, inGameFireFrame), 30), 2, 1);
        }
      
    }

    boolean isHittingObject(byte objectX, byte objectY, byte objectWidth, byte objectHeight) {
      if ((isVisible()) &&
        ((isLaserBeam() && (x >= (objectX - 30))) || (!isLaserBeam() && (x >= objectX))) &&
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

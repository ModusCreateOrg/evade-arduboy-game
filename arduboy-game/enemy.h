#ifndef ENEMY_H
#define ENEMY_H

#include "globals.h"
#include "bitmaps.h"
#include "bullet.h"

struct Enemy {
  byte x;
  byte y;
  byte width = 16;
  byte height = 16;
  byte health;
  byte difficulty;
  byte dying;
  // isMovingLeft (0), isMovingDown (1), tookDamage (2)
  byte options;
  const uint8_t *bitmap;
  unsigned long damageFrame;
  Bullet bullet;

  void set(byte _x, byte _y) {
    x = _x;
    y = _y;
    dying = 0;
    bullet.hide();
    
    options |= random(2) << 0;
    options |= random(2) << 1;

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

    draw();
  }

  void update() {
    if (bullet.isVisible()) {
      bullet.update();
    }

    if (isAlive() && !(options & (1 << 2))) {
      move();
      draw();

      if (!bullet.isVisible()) {
        fire();
      }
    } else if (isDying()) {
      updateDeathSequence();
    } else if (random(700) == 0) {
      spawn();
    }

    
    if(inGameFrame > (damageFrame + 4) && options & (1 << 2)) {
      if(inGameFrame % 4) {
        options &= ~(1 << 2);
      }
    }
  }

  void spawn() {
    byte enemyX = random(MIN_ENEMY_SHIP_X, MAX_ENEMY_SHIP_X);
    byte enemyY = random(MIN_SHIP_Y, MAX_SHIP_Y);
    set(enemyX, enemyY);
  }

  void move() {
    if (random(10 / difficulty) == 0) {
      changeDirection();
      
      byte newX = x + (isMovingLeft() ? -1 : 1);
      byte newY = y + (isMovingDown() ? -1 : 1);
      
      if ((newX >= MIN_ENEMY_SHIP_X) && (newX <= MAX_ENEMY_SHIP_X)) {
        x = newX;
      }
      if ((newY >= MIN_SHIP_Y) && (newY <= MAX_SHIP_Y)) {
        y = newY;
      }
    }
  }

  void draw() {
    drawBitmap(x, y, bitmap, 0);
  }

  void updateDeathSequence() {
    arduboy.drawCircle(x, y, dying, 1);
    arduboy.tunes.tone(200 + (dying * 4), 10);
    if (dying < 65) {
      dying++;
    } else {
      // Fully dead, reset it so it can respawn
      dying = 0;
    }
  }

  void changeDirection() {
    if (random(30) == 0) {
      options ^= 1 << 0;
    }
    if (random(10) == 0) {
      options ^= 1 << 1;
    }
  }

  void fire() {
    if ((dying == 0) &&
      (!bullet.isVisible()) &&
      (random(1000 / difficulty) == 0)) {
      bullet.set(x, (y + (height / 2) - 1), false, 1, .7, false);
    }
  }

  void takeDamage() {
    options |= 1 << 2;
    damageFrame = inGameFrame;
  }

  boolean isAlive() {
    return ((health > 0) && (dying == 0));
  }

  boolean isDying() {
    return (dying > 0);
  }

  boolean isMovingLeft() {
    return (options & (1 << 0));
  }

  boolean isMovingDown() {
    return (options & (1 << 1));
  }
};

#endif

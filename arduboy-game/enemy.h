#ifndef ENEMY_H
#define ENEMY_H

#include "globals.h"
#include "bitmaps.h"
#include "bullet.h"

struct Enemy {
  byte x;
  byte y;
  byte width;
  byte height;
  int health;
  byte type;
  byte difficulty;
  byte dying;
  // isMovingLeft (0), isMovingDown (1), tookDamage (2)
  byte options;
  const uint8_t *bitmap;
  unsigned long damageFrame;
  Bullet bullets[MAX_BOSS_BULLETS];
  int animFrame;

  void set(byte _x, byte _y, byte _type, byte currentIteration) {
    x = _x;
    y = _y;
    type = _type;
    dying = 0;

    // Hide all bullets
    for (byte i = 0; i < MAX_BOSS_BULLETS; i++) {
      bullets[i].hide();
    }

    // Randomize direction
    if (random(2)) {
      options ^= 1 << 0;
    }
    if (random(2)) {
      options ^= 1 << 1;
    }

    width = 16;
    height = 16;
    difficulty = 4;

//    type=7; // For debug
    if (type < 5) {
      difficulty = (currentIteration == 0 ? 1: 2);
      bitmap = enemy1;
      health = 25;
    } else if (type < 9) {
      difficulty = 2;
      bitmap = enemy2;
      health = 150 + (currentIteration * B_BULLET_DAMAGE);
    } else if (type == 9) {
      bitmap = enemy3;  
      health = 500 + (currentIteration * A_BULLET_DAMAGE);
    } else if (type == 128) {
      bitmap = boss1;
      health = 1000 + (currentIteration * 500);
      width = 32;
    } else if (type == 129) {
      bitmap = boss2;
      health = 2000 + (currentIteration * 1000);
      width = 32;
    } else if (type == 130) {
      bitmap = boss3;
      health = 3000 + (currentIteration * 3000);
      width = 59;
      height = 53;
    }


//    health = 5;

    draw();
  }

  void update(boolean stopSpawningEnemies, byte currentIteration) {
    if ((inGameFrame > (damageFrame + 4))
      && (isTakingDamage())
      && (inGameFrame % 4)) {
      options &= ~(1 << 2);
    }
    
    if (isAlive()) {
      move();

      if (!isTakingDamage()) {
        draw();
      }
    } else if (dying > 0) {
      updateDeathSequence();
    } else if ((type <= 9) && (! stopSpawningEnemies) && (random(600) == 0)) {
      spawn(currentIteration);
    }
    
    if ((type <= 9)
      || ((type == 128) && (x == MIN_ENEMY_SHIP_X))
      || ((type == 129) && (x <= 110))
      || ((type == 130) && (x == 69))) {
        
      for (byte i = 0; i < MAX_BOSS_BULLETS; i++) {
        if (!bullets[i].isVisible()) {
          fire(i, currentIteration);
        } else {
          bullets[i].update();
        }
      }
    }
  }

  void spawn(byte currentIteration) {
    byte enemyX = random(MIN_ENEMY_SHIP_X, MAX_ENEMY_SHIP_X);
    byte enemyY = random(MIN_PLAYER_Y, MAX_PLAYER_Y);
    set(enemyX, enemyY, random(10), currentIteration);
  }

  void move() {
    if ((random(10 / difficulty) == 0) || (type > 9)) {
        
      changeDirection();

      bool typeIs128 = (type == 128),
           typeIs129 = (type == 129);
      
      if (typeIs128 && x > MIN_ENEMY_SHIP_X) {
        if (inGameFrame % 4 == 0) {
          x--;
        }
      } else if (typeIs129 && x > 110) {
        if (inGameFrame % 3 == 0) {
          x--;
        }
      } else if ((type == 130) && x > 69) {
        if (inGameFrame % 5 == 0) {
          x--;
        }
      } else if ((type <= 9)
        || ((typeIs128 || typeIs129) && (random(3) == 0))) {
          
        byte newX = x + (isMovingLeft() ? -1 : 1);
        byte newY = y + (isMovingDown() ? -1 : 1);

        if (((type <= 9) && (newX >= MIN_ENEMY_SHIP_X) && (newX <= MAX_ENEMY_SHIP_X))
          || ((type == 129) && (newX >= 97) && (newX <= 109))) {
          x = newX;
        }

        bool newShipYisGreater = newY >= MIN_PLAYER_Y;
        if (((type <= 9) && newShipYisGreater && (newY <= MAX_PLAYER_Y))
          || (((type == 128) || (type == 129)) && newShipYisGreater && (newY <= (MAX_PLAYER_Y + 16 - height)))) {
          y = newY;
        }
      }
    }
  }

  void draw() {
    if (type != 129) {
      drawBitmap(x, y, bitmap, 0);  
    } else {
      if (inGameFrame % 13 == 0) {
        animFrame++;
      }
      drawBitmap(x, y, bitmap, animFrame);
      if (animFrame > 2) {
        animFrame = 0;
      }
    }
  }

  void updateDeathSequence() {
    
//    playTone(dying * 4, 10);
    if (dying < 30) {
      dying++;
      
//      arduboy.drawCircle(x + (width / 2), y + (height / 2), dying, 1);
      explode(x + width / 2,y + height / 2,dying);
//        arduboy.drawCircle(_x - randOp , _y - randOp, random(3, 10), 1);
//        arduboy.drawCircle(_x + randOp , _y - randOp, random(3, 10), 1);
//        arduboy.drawCircle(_x - randOp , _y + randOp, random(3, 10), 1);
//        arduboy.drawCircle(_x + randOp , _y + randOp, random(3, 10), 1);
//      }
    } else {
      // Fully dead, reset it so it can respawn
      dying = 0;
    }
  }

  void changeDirection() {
  
    if (((type <= 9) && (random(30) == 0))
      || ((type > 9) && (random(50) == 0))) {
      options ^= 1 << 0;
    }
    if (((type <= 9) && (random(10) == 0))
      || ((type == 128) && (random(50) == 0))
      || ((type == 129) && (random(20) == 0))) {
      options ^= 1 << 1;
    }
  }

  void fire(byte bulletIndex, byte currentIteration) {
    if (isAlive() && inGameFrame > 120) {
      byte newY = (y + (height / 2) - 1);
      
      if (type <= 9) {
        if ((bulletIndex == 0)
          && (!bullets[bulletIndex].isVisible())
          && (random(1000 / difficulty) == 0)) {
          bullets[bulletIndex].set(x, newY, false, 1, getBulletSpeed(0.7, currentIteration), false);
        }
      } else if (type == 128) {
        bullets[bulletIndex].set(x, newY, false, 1, getBulletSpeed(0.7, currentIteration), false);
      } else if ((type == 129) && (inGameFrame % 50 == 0) && (random(2) == 0)) {
        if (random(4) == 0) {
          bullets[bulletIndex].set((x - 10), newY, false, 1, getBulletSpeed(0.9, currentIteration), true);
        } else {
          bullets[bulletIndex].set(x, newY, false, 1, getBulletSpeed(0.8, currentIteration), false);
        }
      } else if (type == 130) {
        bullets[bulletIndex].set(x, random(MIN_PLAYER_Y, (MAX_PLAYER_Y + 8)), false, 1, getBulletSpeed(0.6, currentIteration), false);
      } 
    }
    
  }

  float getBulletSpeed(float initialSpeed, byte currentIteration) {
    if (currentIteration > 0 && (initialSpeed + (0.1f * currentIteration) > 1)) {
      return 1.0f;
    }

    return (initialSpeed + (0.1f * (currentIteration - 1)));
  }

  void takeDamage() {
    options |= 1 << 2;
    damageFrame = inGameFrame;
  }

  boolean isAlive() {
    return ((health > 0) && (dying == 0));
  }

//  boolean isDying() {
//    return (dying > 0);
//  }

  boolean isMovingLeft() {
    return (options & (1 << 0));
  }

  boolean isMovingDown() {
    return (options & (1 << 1));
  }

  boolean isTakingDamage() {
    return (options & (1 << 2));
  }
};

#endif

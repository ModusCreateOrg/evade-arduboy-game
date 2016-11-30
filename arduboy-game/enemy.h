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

  void set(byte _x, byte _y, byte _type) {
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
    
    if (type < 5) {
      difficulty = 1;
      bitmap = enemy1;
      health = 25;
    } else if (type < 9) {
      difficulty = 2;
      bitmap = enemy2;
      health = 150;
    } else if (type == 9) {
      bitmap = enemy3;
      health = 500;
    } else if (type == 128) {
      bitmap = boss1;
      health = 1000;
      width = 32;
    } else if (type == 129) {
      bitmap = boss2;
      health = 2000;
      width = 32;
    } else if (type == 130) {
      bitmap = boss3;
      health = 3000;
      width = 59;
      height = 53;
    }

    draw();
  }

  void update(boolean stopSpawningEnemies) {
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
      spawn();
    }
    
    if ((type <= 9)
      || ((type == 128) && (x == MIN_ENEMY_SHIP_X))
      || ((type == 129) && (x <= 110))
      || ((type == 130) && (x == 69))) {
        
      for (byte i = 0; i < MAX_BOSS_BULLETS; i++) {
        if (!bullets[i].isVisible()) {
          fire(i);
        } else {
          bullets[i].update();
        }
      }
    }
  }

  void spawn() {
    byte enemyX = random(MIN_ENEMY_SHIP_X, MAX_ENEMY_SHIP_X);
    byte enemyY = random(MIN_PLAYER_Y, MAX_PLAYER_Y);
    set(enemyX, enemyY, random(10));
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
    
    playTone(dying * 4, 10);
//    arduboy.tunes.tone(200 + (dying * 4), 10);
    if (dying < 65) {
      dying++;
      arduboy.drawCircle(x, y, dying, 1);

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

  void fire(byte bulletIndex) {
    if (isAlive() && inGameFrame > 120) {
      byte newY =  (y + (height / 2) - 1);
      
      if (type <= 9) {
        if ((bulletIndex == 0)
          && (!bullets[bulletIndex].isVisible())
          && (random(1000 / difficulty) == 0)) {
          bullets[bulletIndex].set(x, newY, false, 1, .7, false);
        }
      } else if (type == 128) {
        bullets[bulletIndex].set(x, newY, false, 1, 0.7, false);
      } else if ((type == 129) && (inGameFrame % 50 == 0) && (random(2) == 0)) {
        if (random(4) == 0) {
          bullets[bulletIndex].set((x - 10), newY, false, 1, 0.9, true);
        } else {
          bullets[bulletIndex].set(x, newY, false, 1, 0.8, false);
        }
      } else if (type == 130) {
        bullets[bulletIndex].set(x, random(MIN_PLAYER_Y, (MAX_PLAYER_Y + 8)), false, 1, 0.6, false);
      } 
    }
    
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

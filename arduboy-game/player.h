#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 16
#define MAX_GUN_CHARGE 40

struct Player {
  byte x;
  byte y;
  byte frame;
  byte dying;
  byte gunCharge;

  void reset() {
    x = 2;
    y = 32;
    frame = 2;
    dying = 0;
    gunCharge = MAX_GUN_CHARGE;
  }
};

#endif

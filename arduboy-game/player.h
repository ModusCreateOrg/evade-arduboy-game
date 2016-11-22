#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_SIZE 16
#define MAX_GUN_CHARGE 40
#define GUN_SHOT_COST 12

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

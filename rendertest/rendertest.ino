#include "Arduboy.h"
#include "bitmaps.h"

// Define limits that ship movement is bounded by
#define MIN_SHIP_X 2
#define MAX_SHIP_X 112
#define MIN_SHIP_Y 2
#define MAX_SHIP_Y 48

Arduboy arduboy;

void setup() {
  arduboy.beginNoLogo();
  arduboy.setFrameRate(30);
}

void loop() {
  unsigned short shipX = 2, shipY = 32;
  int offset = 64;
  
  while (true) {
    arduboy.clear();
    arduboy.drawBitmap(shipX, shipY, playerShip+offset, 16, 16, 1);
    arduboy.display();

    offset = 0;
    if (arduboy.pressed(UP_BUTTON) && shipY > MIN_SHIP_Y) {
      shipY--;
      offset = 32;
    } else if (arduboy.pressed(DOWN_BUTTON) && shipY < MAX_SHIP_Y) {
      shipY++;
      offset = 64;
    } else if (arduboy.pressed(LEFT_BUTTON) && shipX > MIN_SHIP_X) {
      shipX--;
      offset = 0;
    } else if (arduboy.pressed(RIGHT_BUTTON) && shipX < MAX_SHIP_X) {
      shipX++;
      offset = 0;
    }
  }
}


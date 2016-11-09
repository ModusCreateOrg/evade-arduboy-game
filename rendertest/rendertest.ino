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
  int frame = 2;
  while (true) {

    arduboy.clearDisplay();
//
//    if (arduboy.pressed(RIGHT_BUTTON))
//    {
//      if (shipX < MAX_SHIP_X) shipX++;
//    }
//    if (arduboy.pressed(LEFT_BUTTON))
//    {
//      if (shipX > MIN_SHIP_X) shipX--;
//    }
//    if (arduboy.pressed(UP_BUTTON))
//    {
//      if (shipY > MIN_SHIP_X) shipY--;
//      if (arduboy.everyXFrames(9)) frame--;
//      if (frame < 0) frame = 0;
//    }
//    if (arduboy.pressed(DOWN_BUTTON))
//    {
//      if (shipY < MAX_SHIP_Y) shipY++;
//      if (arduboy.everyXFrames(9)) frame++;
//      if (frame  > 4) frame = 4;
//    }
//
//    if (arduboy.notPressed(UP_BUTTON) && arduboy.notPressed(DOWN_BUTTON))
//    {
//      if (arduboy.everyXFrames(12))
//      {
//        if (frame > 2)
//          frame--;
//        if (frame < 2)
//          frame++;
//      }
//    }

//    draw(shipX, shipY, playerShip, frame);

    draw(24, 4, modus, 0);
    arduboy.display();
  }
}

void draw(int x, int y, const uint8_t *bitmap, uint8_t frame) {
  unsigned int frame_offset;
  uint8_t width = pgm_read_byte(bitmap);
  uint8_t height = pgm_read_byte(++bitmap);

  bitmap++;
  if (frame > 0) {
    frame_offset = (width * ( height / 8 + ( height % 8 == 0 ? 0 : 1)));
    // sprite plus mask uses twice as much space for each frame
    bitmap += frame * frame_offset;
  }
  arduboy.drawBitmap(x, y, bitmap, width, height, 1);
}


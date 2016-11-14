#ifndef STAR_H
#define STAR_H

struct Star {
  float x;
  byte y;
  byte width;
  byte height;
  byte layer;

  void setValues() {
      x = random(250);
      y = random(75);
      width = random(3);
      height = random(3);
  }

};

#endif

#ifndef STAR_H
#define STAR_H

struct Star {
  float x,
        speed;
  byte y;
  byte width;
  byte height;
  byte layer;

  void setValues() {
      x = random(250);
      y = random(75);
      width = random(4);
      height = 1;

      if (width == 0) {
        width = 1;
      }
      // elongated vertically is weird.
      if (height > width) {
        height = width;
      }

      if (width > 1) {
        speed = 1;
      }
      else {
        speed = .25;
      }
  }

};

#endif

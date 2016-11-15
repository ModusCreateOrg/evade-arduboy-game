#ifndef STAR_H
#define STAR_H

struct Star {
  float x;
  float speed;
  
  byte y;
  byte width;
  byte layer;

  void setValues() {
      x = random(250);
      y = random(75);
      width = random(1, 4);

      if (width >= 3) {
        speed = random(75, 95) * 0.01f;
      }
      else if (width >= 2) {
        speed = random(35, 40) * 0.01f;
      }
      else {
        speed = random(15, 25) * 0.01f;
      }
//      serial
  }

};

#endif

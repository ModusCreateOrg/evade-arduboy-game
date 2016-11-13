
struct Star {

  float x;
  int y;
  int width;
  int height;
  int layer;


  void setValues() {
      x = random(250);
      y = random(75);
      width = random(3);
      height = random(3);
  }

};


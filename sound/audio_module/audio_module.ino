#include "Arduboy.h"
Arduboy arduboy;

#include "MusicPlayer.h"



void setup()
{
  arduboy.begin();
  arduboy.setTextSize(1);
  arduboy.setCursor(0,0);
  arduboy.print(" ");
  arduboy.display();
}


int x = 0, 
    y = 0,
    played = 0;


void loop ()
{
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;


  if (arduboy.pressed(UP_BUTTON)) {
    y-=1;
    playTone1();
  } 
  else if (arduboy.pressed(DOWN_BUTTON)) {
    playTone2();

    y+=1;
  } 
  else if (arduboy.pressed(LEFT_BUTTON)) {
    playTone3();
    x-=1;
  } 
  else if (arduboy.pressed(RIGHT_BUTTON)) {
    playTone4();
    x+=1;
  }


  
  if (arduboy.pressed(A_BUTTON)) {
    played = 2;
  } 
  else if (arduboy.pressed(B_BUTTON)) {
    played = 1;
  }
  
  arduboy.clear();
  arduboy.setCursor(x,y);
  
  if (played == 0) {
    arduboy.print("Press B to play...");
    stopMusic();
  }
  else if (played == 1) {
    arduboy.print("Playing...\nPress A to stop");
    playMusic();
  }
  else if (played == 2) {
    arduboy.print("Stopped...\nPress B to play");
    stopMusic();
  }


  arduboy.display();


  // play the tune if we aren't already
//  if (!arduboy.tunes.playing())
//    arduboy.tunes.playScore(stage1Music);
}

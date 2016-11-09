#include "Arduboy.h"
#include "MusicPlayer.h"
#include "Stage1Music.h"


void playMusic() {
    Arduboy ab;

   
    if (!ab.tunes.playing()) {
        ab.tunes.playScore(stage1Music);
    }
    
}

void stopMusic() {
    Arduboy ab;

    if (ab.tunes.playing()) {
        ab.tunes.stopScore();
    }
}

void playTone1() {
  Arduboy ab;
  ab.tunes.tone(987, 120);
  delay(120); 
  ab.tunes.tone(1318, 400);
}

void playTone2() {
  Arduboy ab;

  ab.tunes.tone(1318, 120);
  delay(120); 
  ab.tunes.tone(987, 400);

}


void playTone3() {
  Arduboy ab;
  ab.tunes.tone(1500, 120);
}

void playTone4() {
  Arduboy ab;
  ab.tunes.tone(900, 120);
}

void playTone5() {
  Arduboy ab;
  ab.tunes.tone(987, 120);
}


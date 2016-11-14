#include "Arduboy.h"
#include "MusicPlayer.h"
#include "Music.h"


int currentSong = -1;

void stopMusic() {
    Arduboy ab;

    if (ab.tunes.playing()) {
        ab.tunes.stopScore();
    }
}
//22,262 bytes



void playMusic(int song) {
    Arduboy ab;
    
    if (!ab.tunes.playing() && currentSong != song) {
      stopMusic();

      unsigned char *music;
      switch(song) {
        case 0 :
           song = titleMusic;
        break;
        case 1 :
          song = stage1Music;
        break;
        case 4 :
           song = bossMusic;
        break;
        case 5 :
          song = gameOverMusic;
        break;
      }
      ab.tunes.playScore(music);
    }
}   




// SFX (experimental)
void playTone1() {
  Arduboy ab;
//  ab.tunes.tone(1318, 400);
  ab.tunes.tone(800, 50);
//  delay(120); 
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


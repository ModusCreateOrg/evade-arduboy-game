#include "Arduboy.h"
#include "MusicPlayer.h"
#include "Music.h"


int currentSong = 0;

void stopMusic() {
    Arduboy ab;

    if (ab.tunes.playing()) {
        ab.tunes.stopScore();
    }
}

void playMusic(int song) {
    Arduboy ab;
    
    if (!ab.tunes.playing() && currentSong != song) {
      stopMusic();

      unsigned char *music;
      switch(song) {
        case 0 :
           music = titleMusic;
        break;
        case 1 :
          music = stage1Music;
        break;
        case 4 :
           music = bossMusic;
        break;
        case 5 :
          music = gameOverMusic;
        break;
      }
      ab.tunes.playScore(music);
    }
}   

// SFX (experimental)
void sfx(byte tone) {
  Arduboy ab;

  switch(tone) {
    case 1:
      ab.tunes.tone(800, 50);
    break;
    case 2:
      ab.tunes.tone(1318, 120);
    break;
    case 3:
      ab.tunes.tone(987, 400);
    break;
    case 4:
      ab.tunes.tone(800, 50);
    break;
  }
}

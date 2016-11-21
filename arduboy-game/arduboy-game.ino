/*
   arduboy-game, Modus Create 2016
*/

#include "Arduboy.h"
#include "globals.h"
#include "messagecatalog.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "boss.h"
#include "bitmaps.h"
#include "Music.h"
#include <stddef.h>
#include <inttypes.h>
#include <Arduino.h>
#include <avr/pgmspace.h>

#define DEBOUNCE_DELAY 100
#define MAX_LIVES 4
#define NUM_STARS 15
#define NOT_NEW_HI_SCORE 5

// TODO highScoreTable should be replaced with table in EEPROM
char *highScoreTable = "AAA000400BBB000300CCC000200DDD000100";

unsigned long inGameAButtonLastPress, inGameBButtonLastPress, inGameLastDeath, score;
byte livesRemaining = MAX_LIVES;

float starX[NUM_STARS];
float starSpeed[NUM_STARS];
byte starY[NUM_STARS];
byte starWidth[NUM_STARS];

bool soundOn = true;     
byte currentSong = 255;

// Bullet array
Bullet playerBullets[MAX_PLAYER_BULLETS];

// Enemies array
Enemy enemies[MAX_ENEMIES];

// Boss
Boss boss;

bool isBossAlive;

// General purpose text buffer for string concatenation and read from progmem
char textBuf[23];

Player spaceShip;

//void stopMusic() {
//
//    if (arduboy.tunes.playing()) {
//        arduboy.tunes.stopScore();
//    }
//}

void playMusic(byte song) {
//  Serial.println(currentSong);
    if (soundOn && currentSong != song) {
      arduboy.tunes.stopScore();
    }
    
    unsigned char *music;
      switch(song) {
        case 1 :
           music = titleMusic;
        break;
        case 2 :
//          music = stage1MusicSingleTrack; // IF WE RUN OUT OF SPACE
          music = stage1MusicDoubleTrack;
        break;
        case 3 :
//          music = bossMusicSingleTrack; // IF WE RUN OUT OF SPACE
           music = bossMusicDoubleTrack;
        break;
        case 4 :
          music = gameOverMusic;
        break;
        case 5 : 
          music = mainMusic;
        break;
      }

      currentSong = song;
      if (! arduboy.tunes.playing()) {
        arduboy.tunes.playScore(music);
      }
}   


void printText(char *message, byte x, byte y, byte textSize) {
  arduboy.setCursor(x, y);
  arduboy.setTextSize(textSize);
  arduboy.print(message);
}

void introScreen() {
  arduboy.clear();
  drawBitmap(2, 8, modusLogo, 0);
  arduboy.display();

  arduboy.initRandomSeed();
  
  delay(250);
  playMusic(1);
  delay(2750);
}

byte titleScreen() {
  byte selectedItem = TITLE_PLAY_GAME;
  unsigned short totalDelay = 0;
  long lastDebounceTime = millis();  // the last time the button was pressed
  
  arduboy.clear();
  drawBitmap(4, 14, title, 0);
  drawBitmap(4, 51, titleMenuOptions, 0);
  arduboy.drawRect(2, 48, 26, 12, 1);
  arduboy.display();

  playMusic(5);
  
  while (totalDelay < ATTRACT_MODE_TIMEOUT) {
    unsigned long currentMilliseconds = millis();
    
    if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) {
      if ((currentMilliseconds - lastDebounceTime) > DEBOUNCE_DELAY) {
        break;
      } else {
        lastDebounceTime = currentMilliseconds;
      }
    }

    if (arduboy.pressed(LEFT_BUTTON)) {
      if ((currentMilliseconds - lastDebounceTime) > DEBOUNCE_DELAY) {
        selectedItem = titleMenuLeftButton(selectedItem);
        lastDebounceTime =currentMilliseconds; //set the current time
      }
    }

    if (arduboy.pressed(RIGHT_BUTTON)) {
      if ((currentMilliseconds - lastDebounceTime) > DEBOUNCE_DELAY) {
        selectedItem = titleMenuRightButton(selectedItem);
        lastDebounceTime = currentMilliseconds; //set the current time
      }
    }

    delay(15);
    totalDelay += 15;
  }
  return (totalDelay >= ATTRACT_MODE_TIMEOUT ? TITLE_TIMEOUT : selectedItem);
}

byte titleMenuLeftButton(byte selectedItem) {
  /**
     Handle clicks on the left button
     to navigate through main menu
     items.
  */
  switch (selectedItem) {
    case TITLE_SETTINGS:
      arduboy.drawRect(76, 48, 51, 12, 0);
      arduboy.drawRect(30, 48, 45, 12, 1);
      arduboy.display();
      return TITLE_CREDITS;
      break;

    case TITLE_CREDITS:
      arduboy.drawRect(30, 48, 45, 12, 0);
      arduboy.drawRect(2, 48, 26, 12, 1);
      arduboy.display();
      return  TITLE_PLAY_GAME;
      break;

    default: break;
  }
}

byte titleMenuRightButton(byte selectedItem) {
  /**
     Handle clicks on the right button
     to navigate through main menu
     items.
  */
  switch (selectedItem) {
    case TITLE_PLAY_GAME:
      arduboy.drawRect(2, 48, 26, 12, 0);
      arduboy.drawRect(30, 48, 45, 12, 1);
      arduboy.display();
      return TITLE_CREDITS;
      break;

    case TITLE_CREDITS:
      arduboy.drawRect(30, 48, 45, 12, 0);
      arduboy.drawRect(76, 48, 51, 12, 1);
      arduboy.display();
      return TITLE_SETTINGS;
      break;

    default: break;
  }
}

void highScoreScreen() {
  // TODO, this is placeholder 
  long lastDebounceTime = millis();
  unsigned short totalDelay = 0;
  char hiInitials[4];
  char hiScore[7];

  arduboy.clear();
  printText("HI SCORES", 8, 1, 2);

  for (byte i = 0; i < 4; i++) {
    strncpy(hiInitials, highScoreTable + ((9 * i) * sizeof(char)), 3);
    hiInitials[3] = '\0';
    strncpy(hiScore, highScoreTable + (((9 * i) + 3) * sizeof(char)), 6);
    hiScore[6] = '\0';
    sprintf(textBuf, "%d.  %s  %s", i + 1, hiScore, hiInitials);
    printText(textBuf, 15, 21 + (12 * i), 1);
  }

  arduboy.display();
  while (totalDelay < 4000) {
    unsigned long currentMilliseconds = millis();
    if (arduboy.pressed(UP_BUTTON) || arduboy.pressed(DOWN_BUTTON) || arduboy.pressed(LEFT_BUTTON) || arduboy.pressed(RIGHT_BUTTON) || arduboy.pressed(A_BUTTON)  || arduboy.pressed(B_BUTTON)) {

      if ((currentMilliseconds - lastDebounceTime) > DEBOUNCE_DELAY) {
        totalDelay = 4000;
        lastDebounceTime = currentMilliseconds;
      }
    }
    
    if (totalDelay < 4000) {
      delay(15);
      totalDelay += 15;
    }
  }  
}

void creditsScreen() {
  scrollCredits(4, false);
}

void scrollCredits(byte y, bool quit) {
  /**
     Recursive function for scrolling
     credits up screen
  */
  byte padding = 7;
  byte origY = y;
  arduboy.clear();
  
  for (unsigned short i = 0; i < NUM_CREDITS; i++) {
    if (i == 0) {
      y = y - 4;
    } else {
      y = origY;
    }
    
    strcpy_P(textBuf, (char*)pgm_read_word(&(credits[i])));
    printText(textBuf, 2, y + padding, (i == 0 ? 2 : 1));
    arduboy.display();
    padding = padding + 15;
    if ( i + 1 == NUM_CREDITS && y + padding < 0) {
      quit = true;
    }
  }
  delay(1000);

  if (!quit) {
    scrollCredits(y - 15, quit);
  }
}

void settingsScreen() {
  long lastDebounceTime = millis();  // the last time the button was pressed
  bool exit_settings_menu = false;
  byte selectedItem;

  arduboy.clear();
  printText("SETTINGS", 20, 5, 2);
  printsoundOnOff();
  printText("RESET HIGHSCORE", 20, 37, 1);
  printText("EXIT", 20, 49, 1);
  arduboy.drawRect(17, 22, 35, 13, 1);
  arduboy.display();

  while (!exit_settings_menu) {
    unsigned long currentMilliseconds = millis();

    if (arduboy.pressed(DOWN_BUTTON)) {
      if ((currentMilliseconds - lastDebounceTime) > DEBOUNCE_DELAY) {
        selectedItem = settingMenuDownButton(selectedItem);
        lastDebounceTime = currentMilliseconds; //set the current time
      }
    }

    if (arduboy.pressed(UP_BUTTON)) {
      if ((currentMilliseconds - lastDebounceTime) > DEBOUNCE_DELAY) {
        selectedItem = settingMenuUpButton(selectedItem);
        lastDebounceTime = currentMilliseconds; //set the current time
      }
    }

    if (arduboy.pressed(A_BUTTON)) {
      if ((currentMilliseconds - lastDebounceTime) > DEBOUNCE_DELAY) {
        switch (selectedItem) {
          case SETTINGS_EXIT:
            exit_settings_menu = true;
            break;

          case SETTINGS_SOUND:
            soundOn = !soundOn;
            printsoundOnOff();
            break;
            
          case SETTINGS_RESET_HIGH_SCORE:
            exit_settings_menu = true;
            break;

          default: break;
        }
      }
      lastDebounceTime = currentMilliseconds; //set the current time
    }
  }
}

byte settingMenuDownButton(byte selectedItem) {
  /**
     Handle clicks on the down button
     to navigate through settings menu
     items.
  */
  switch (selectedItem) {

    case SETTINGS_SOUND:
      arduboy.drawRect(17, 22, 35, 13, 0);
      arduboy.drawRect(17, 34, 95, 13, 1);
      arduboy.display();
      return SETTINGS_RESET_HIGH_SCORE;
      break;

    case SETTINGS_RESET_HIGH_SCORE:
      arduboy.drawRect(17, 34, 95, 13, 0);
      arduboy.drawRect(17, 46, 29, 13, 1);
      arduboy.display();
      return SETTINGS_EXIT;
      break;

    default: break;
  }
}

byte settingMenuUpButton(byte selectedItem) {
  /**
     Handle clicks on the up button
     to navigate through settings menu
     items.
  */
  switch (selectedItem) {

    case SETTINGS_EXIT:
      arduboy.drawRect(17, 46, 29, 13, 0);
      arduboy.drawRect(17, 34, 95, 13, 1);
      arduboy.display();
      return SETTINGS_RESET_HIGH_SCORE;
      break;

    case SETTINGS_RESET_HIGH_SCORE:
      arduboy.drawRect(17, 34, 95, 13, 0);
      arduboy.drawRect(17, 22, 35, 13, 1);
      arduboy.display();
      return SETTINGS_SOUND;
      break;

    default: break;
  }
}

void printsoundOnOff() {
  if(soundOn) {
    printText("SOUND  ON ", 20, 25, 1);
    arduboy.drawRect(17, 22, 35, 13, 1);
  } else {
    printText("SOUND  OFF", 20, 25, 1);
    arduboy.drawRect(17, 22, 35, 13, 1);
  }
  arduboy.display();
}

void playGame() {
  inGameFrame = 0;
  inGameAButtonLastPress = 0;
  inGameBButtonLastPress = 0;
  inGameLastDeath = 0;
  score = 0;
  livesRemaining = MAX_LIVES;
  spaceShip.reset();
  arduboy.tunes.stopScore();
  isBossAlive = false;
  boolean spawnedBossOne = false;
  boolean spawnedBossTwo = false;
  
  while (livesRemaining > 0) {
    arduboy.clear();
    inGameFrame++;

    if (inGameFrame % 20 == 0) {
      score++;

      if (spaceShip.gunTemp > 0) {
        // Cool off the gun
        spaceShip.gunTemp--;
      }
    }

    drawGunTemp();

    drawScore();

    drawPlayerShip();

    if (!isBossAlive) {
      boolean spawnedBoss = false;
      byte arduboyWidth = arduboy.width();
      if ((score >= 5000) && (!spawnedBossOne)) {
        boss.set(arduboyWidth + 1, 10, 1);
        spawnedBossOne = true;
        spawnedBoss = true;
      } else if ((score >= 12000) && (!spawnedBossTwo)) {
        boss.set(arduboyWidth + 1, 24, 2);
        spawnedBossTwo = true;
        spawnedBoss = true;
      }

      if (spawnedBoss) {
        isBossAlive = true;
        for (byte i = 0; i < MAX_ENEMIES; i++) {
          enemies[i].health = 0;
        }
      }
    }
    
    if (isBossAlive) {
      boss.update();
    } else {
      updateEnemies();
    }

    if(inGameAButtonLastPress > 80 || inGameBButtonLastPress > 60) {
      for (byte i = 0; i < MAX_PLAYER_BULLETS; i++) {
        playerBullets[i].update();
      }  
    }

    drawStarLayer();
    drawLives();
       
    arduboy.display();

    updateStarFieldVals();

    handlePlayerBullets();
    handleEnemyBullets();
    handleBossBullets();
      
    if (isBossAlive) {
       playMusic(3);
    }
    else {
       playMusic(2);
    }

  }

  arduboy.tunes.stopScore();
}

void drawGunTemp() {
  arduboy.drawRect(40, 1, 40, 5, 1);
  arduboy.fillRect(40, 1, (spaceShip.gunTemp > 40 ? 40 : spaceShip.gunTemp), 5, 1);
}

void drawScore() {
  sprintf(textBuf, "%06d", score);
  printText(textBuf, 0, 0, 1);
}

void drawStarLayer() {
  for (byte i = 0; i < NUM_STARS; i++) {
    arduboy.drawFastHLine(starX[i], starY[i], starWidth[i], 1);
  }
}

void drawLives() {
  for (byte i = 0; i < MAX_LIVES; i++) {
    if (i < livesRemaining) {
      drawBitmap(120 - (i * 10), 0, livesShip , 0);
    }
  }
}

void drawPlayerShip() {
  if (spaceShip.dying == 0) {
    if (arduboy.pressed(RIGHT_BUTTON) && (spaceShip.x < MAX_PLAYER_SHIP_X)) {
      spaceShip.x++;
    }
  
    if (arduboy.pressed(LEFT_BUTTON) && (spaceShip.x > MIN_PLAYER_SHIP_X)) {
      spaceShip.x--;
    }
  
    if (arduboy.pressed(UP_BUTTON)) {
      if (spaceShip.y > MIN_SHIP_Y) {
        spaceShip.y--;
      }
      if (arduboy.everyXFrames(9)) {
        if (spaceShip.frame > 0) {
          spaceShip.frame--;
        }
      }
    }
  
    if (arduboy.pressed(DOWN_BUTTON)) {
      if (spaceShip.y < MAX_SHIP_Y)  {
        spaceShip.y++;
      }
      if (arduboy.everyXFrames(9)) {
        spaceShip.frame++;
      }
      if (spaceShip.frame > 4) {
        spaceShip.frame = 4;
      }
    }
  
    if (arduboy.pressed(A_BUTTON)) {
      if ((inGameFrame > 80) && (inGameAButtonLastPress < (inGameFrame - 75)) && (spaceShip.gunTemp < 25)) {
        inGameAButtonLastPress = inGameFrame;
        // Fire A weapon (single fire) if weapon isn't too hot
        for (byte i = 0; i < MAX_PLAYER_BULLETS; i++) {
          if (!playerBullets[i].isVisible()) {
            playerBullets[i].set(spaceShip.x, (spaceShip.y + 5), true, A_BULLET_DAMAGE, 2.5, false);
            spaceShip.gunTemp += 15;
            break;
          }
        }
      }
    }
  
    // Here to test out other SFX
    if (arduboy.pressed(B_BUTTON)) {
      if (inGameBButtonLastPress < (inGameFrame - 55)) {
        inGameBButtonLastPress = inGameFrame;
        // Fire B weapon (rapid fire)
        for (byte i = 0; i < MAX_PLAYER_BULLETS; i++) {
          if (inGameBButtonLastPress > 80 && !playerBullets[i].isVisible()) {
            playerBullets[i].set(spaceShip.x, (spaceShip.y + 7), true, B_BULLET_DAMAGE, 3, false);
            break;
          }
        }
      }
    }
  
    if (arduboy.notPressed(UP_BUTTON) && arduboy.notPressed(DOWN_BUTTON)) {
      if (arduboy.everyXFrames(12)) {
        if (spaceShip.frame > 2) {
          spaceShip.frame--;
        }
        if (spaceShip.frame < 2) {
          spaceShip.frame++;
        }
      }
    }

    if ((inGameLastDeath > 0) && (inGameLastDeath > (inGameFrame - 450))) {
      if (!(inGameFrame % 3)) {
        // Blink ship
        drawBitmap(spaceShip.x, spaceShip.y, playerShip, spaceShip.frame);
      }
    } else {
      drawBitmap(spaceShip.x, spaceShip.y, playerShip, spaceShip.frame);
    }

    
  } else {
    arduboy.drawCircle(spaceShip.x, spaceShip.y, spaceShip.dying , 1);
    
    int tone = (spaceShip.dying % 2 == 0) ? (400 + spaceShip.dying * 2) : (600 - spaceShip.dying * 2);
    arduboy.tunes.tone(tone, 10);
    if (spaceShip.dying < 65) {
      spaceShip.dying++;
    } else {
      livesRemaining--;
      spaceShip.reset();
    }
  }
}

void updateEnemies() {
  for (byte i = 0; i < MAX_ENEMIES; i++) {
    enemies[i].update();
  }
}

void handleEnemyBullets() {
  for (byte i = 0; i < MAX_ENEMIES; i++) {
    if ((enemies[i].bullet.isVisible()) && (enemies[i].bullet.isHittingObject(spaceShip.x, spaceShip.y, PLAYER_WIDTH, PLAYER_HEIGHT))) {
      // Hit Player
      enemies[i].bullet.hide();

      // Doesn't count if player recently died
      if (inGameLastDeath < (inGameFrame - 450)) {
        inGameLastDeath = inGameFrame;
        spaceShip.dying = 1;
      }
    }
  }
}

void handleBossBullets() {
  if (isBossAlive) {
    for (byte i = 0; i < MAX_BOSS_BULLETS; i++) {
      if ((boss.bullets[i].isVisible()) && (boss.bullets[i].isHittingObject(spaceShip.x, spaceShip.y, PLAYER_WIDTH, PLAYER_HEIGHT))) {
        // Hit Player
        boss.bullets[i].hide();
  
        // Doesn't count if player recently died
        if (inGameLastDeath < (inGameFrame - 450)) {
          inGameLastDeath = inGameFrame;
          spaceShip.dying = 1;
        }
      }
    }
  }
}

void handlePlayerBullets() {
  for (byte i = 0; i < MAX_PLAYER_BULLETS; i++) {
    if (playerBullets[i].isVisible()) {
      if (isBossAlive) {
        if ((boss.health > 0) && (playerBullets[i].isHittingObject(boss.x, boss.y, boss.width, boss.height))) {
          // Hit Boss
          playerBullets[i].hide();
          boss.health -= playerBullets[i].damage;
          score += playerBullets[i].damage;
          
          if (boss.health <= 0) {
            // Killed Boss
            isBossAlive = false;
            for (byte j = 0; j < MAX_BOSS_BULLETS; j++) {
              boss.bullets[j].hide();
            }
            score += 500;
          }
        }
      } else {
        for (byte j = 0; j < MAX_ENEMIES; j++) {
          if ((enemies[j].health > 0) && (playerBullets[i].isHittingObject(enemies[j].x, enemies[j].y, enemies[j].width, enemies[j].height))) {
            // Hit Enemy
            playerBullets[i].hide();
            score += playerBullets[i].damage;
            
            if (playerBullets[i].damage > enemies[j].health) {
              enemies[j].health = 0;
              enemies[j].dying = 1;
              score += 100;
            } else {
              enemies[j].health -= playerBullets[i].damage;
              enemies[j].takeDamage();
            }
          }
        }
      }
    }
  }
}

void gameOverScreen() {
  arduboy.clear();
  drawBitmap(0, 8, gameOver, 0);
  arduboy.display();

  delay(100);
  // play game over tune

  playMusic(4);
  
  delay(3000);
  arduboy.tunes.stopScore();
  delay(2000);
}

void drawHighScoreEntryCursor(byte pos) {
  arduboy.fillRect(44, 62, 10, 2, (pos == 0 ? 1 : 0));
  arduboy.fillRect(56, 62, 10, 2, (pos == 1 ? 1 : 0));
  arduboy.fillRect(68, 62, 10, 2, (pos == 2 ? 1 : 0));
}

byte isNewHighScore() {
  char hiScore[7];
  hiScore[6] = '\0';
  for (byte i = 0; i < 4; i++) {
    strncpy(hiScore, highScoreTable + (((9 * i) + 3) * sizeof(char)), 6);
    if (score > strtol(hiScore, NULL, 10)) {
       return i;
    }
  }
  return NOT_NEW_HI_SCORE;
}

void newHighScoreScreen(byte newHiPos) {
  long lastDebounceTime = millis();
  bool allDone = false;
  unsigned short currPos = 0;
  byte currInitials[] = { 65, 65, 65};
  
  arduboy.clear();
  printText("NEW HI!", 24, 1, 2);
  sprintf(textBuf, "%06d", score);
  printText(textBuf, 28, 22, 2);
  sprintf(textBuf, "%c%c%c", currInitials[0], currInitials[1], currInitials[2]);
  printText(textBuf, 44, 45, 2);
  drawHighScoreEntryCursor(currPos);

  arduboy.display();

  while(! allDone) {
    unsigned long currentMilliseconds = millis();
    if (arduboy.pressed(A_BUTTON)) {
      if ((currentMilliseconds - lastDebounceTime) > DEBOUNCE_DELAY) {
        // Advance right or finish entering initials
        if (currPos < 2) {
          currPos++;
          drawHighScoreEntryCursor(currPos);
          arduboy.display();
        } else {
          allDone = true;
        }
        lastDebounceTime = currentMilliseconds;
      }
    }
   
    if (arduboy.pressed(LEFT_BUTTON)) {
      if ((currentMilliseconds - lastDebounceTime) > DEBOUNCE_DELAY) {
        // Only do something if we are not already on leftmost one
        if (currPos > 0) {
          currPos--;
          drawHighScoreEntryCursor(currPos);
          arduboy.display();
        }
        lastDebounceTime = currentMilliseconds;
      }    
    }
  
    if (arduboy.pressed(RIGHT_BUTTON)) {
      if ((currentMilliseconds - lastDebounceTime) > DEBOUNCE_DELAY) {
        // Only do something if we are not already on rightmost one
        if (currPos < 2) {
          currPos++;
          drawHighScoreEntryCursor(currPos);
          arduboy.display();
        }
        lastDebounceTime = currentMilliseconds;
      }
    }

    if (arduboy.pressed(UP_BUTTON)) {
       if ((currentMilliseconds - lastDebounceTime) > DEBOUNCE_DELAY) {
          if (currInitials[currPos] == 96) {
            currInitials[currPos] = 32;
          } else {
            currInitials[currPos]++;
          }
          sprintf(textBuf, "%c%c%c", currInitials[0], currInitials[1], currInitials[2]);
          printText(textBuf, 44, 45, 2);
          arduboy.display();
          lastDebounceTime = currentMilliseconds;
       }
    }

    if (arduboy.pressed(DOWN_BUTTON)) {
       if ((currentMilliseconds - lastDebounceTime) > DEBOUNCE_DELAY) {
          if (currInitials[currPos] == 32) {
            currInitials[currPos] = 96;
          } else {
            currInitials[currPos]--;
          }
          sprintf(textBuf, "%c%c%c", currInitials[0], currInitials[1], currInitials[2]);
          printText(textBuf, 44, 45, 2);
          arduboy.display();
          lastDebounceTime = currentMilliseconds;
       }     
    }
  }

  // Store the new high score, newHiPos == 0 is highest score
  sprintf(textBuf, "%c%c%c%06d", currInitials[0], currInitials[1], currInitials[2], score);

  if (newHiPos < 3) {
    // shuffle existing results around
    for (currPos = 26; currPos > (9 * newHiPos); currPos--) {
      highScoreTable[currPos + 9] = highScoreTable[currPos];
    }
  }

  // then copy new result into correct place
  for (currPos = 0; currPos < 9; currPos++) {
    highScoreTable[currPos + ((9 * newHiPos) * sizeof(char))] = textBuf[currPos];
  }
}

void createStarFieldVals() {
  for (byte i = 0; i < NUM_STARS; i++) {
      setStarValuesForIndex(i);
  } 
}

void setStarValuesForIndex(byte i) {
  starX[i] = random(250);
  starY[i] = random(75);
  starWidth[i] = random(1, 4);

  if (starWidth[i] >= 3) {
//    starSpeed[i] = random(75, 95) * 0.01f;
    starSpeed[i] = random(30, 40) * 0.01f;
  }
  else if (starWidth[i] >= 2) {
//    starSpeed[i] = random(35, 40) * 0.01f;   
    starSpeed[i] = random(18, 25) * 0.01f;
  }
  else {
//    starSpeed[i] = random(15, 25) * 0.01f;
    starSpeed[i] = random(7, 12) * 0.01f;
  }
}

void updateStarFieldVals() {
  for (byte i = 0; i < NUM_STARS; i++) {
    if (starX[i] < -1) {
      setStarValuesForIndex(i);
      starX[i] = 128 + random(20);
      starY[i] = random(10, 64);
      
    } 
    else {
      starX[i] -= starSpeed[i];
    }
  }
}

boolean shouldPlayAButtonTone() {
  return (inGameAButtonLastPress > (inGameFrame - 20));
}

boolean shouldPlayBButtonTone() {
  return (inGameBButtonLastPress > (inGameFrame - 50));
}

// Initialization runs once only
void setup() {
  arduboy.beginNoLogo();
  introScreen();
  spaceShip.reset();
  createStarFieldVals();
}

// Main program loop
void loop() {
  byte result, newHiScorePos;

  result = titleScreen();

  switch (result) {
    case TITLE_CREDITS:
      creditsScreen();
      break;
    case TITLE_PLAY_GAME:
      playGame();
      gameOverScreen();
      // TODO high score should be checked against a set of high scores
      // in the EEPROM
      newHiScorePos = isNewHighScore();
      if (newHiScorePos != NOT_NEW_HI_SCORE) {
        newHighScoreScreen(newHiScorePos);
      }

      highScoreScreen();
      break;
    case TITLE_SETTINGS:
      settingsScreen();
      break;
    case TITLE_TIMEOUT:
      // No button pressed on title, alternate with high score
      highScoreScreen();
      break;
  }
}

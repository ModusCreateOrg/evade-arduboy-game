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
#include <avr/pgmspace.h>

#define DEBOUNCE_DELAY 100
#define MAX_LIVES 4
#define NUM_STARS 15
// TODO highScore should be replaced with table in EEPROM
unsigned long inGameFrame, inGameAButtonLastPress, inGameBButtonLastPress, score, highScore = 0;
byte livesRemaining = MAX_LIVES;

float starX[NUM_STARS];
float starSpeed[NUM_STARS];
byte starY[NUM_STARS];
byte starWidth[NUM_STARS];

bool musicOn = true;     
byte currentSong = 0;

// Bullet arrays
Bullet playerBullets[MAX_PLAYER_BULLETS];
Bullet enemyBullets[MAX_ENEMIES];

// Enemies array
Enemy enemies[MAX_ENEMIES];

// General purpose text buffer for string concatenation and read from progmem
char textBuf[25];

Player spaceShip;

void stopMusic() {
    Arduboy ab;

    if (ab.tunes.playing()) {
        ab.tunes.stopScore();
    }
}

void playMusic(byte song) {
    if (!arduboy.tunes.playing() && currentSong != song) {
      stopMusic();

      unsigned char *music;
      switch(song) {
        case 1 :
           music = titleMusic;
        break;
        case 2 :
          music = stage1Music;
        break;
        case 3 :
           music = bossMusic;
        break;
        case 4 :
          music = gameOverMusic;
        break;
        case 5 : 
          music = mainMusic;
        break;
      }
      
      arduboy.tunes.playScore(music);
    }
}   

// SFX (experimental)
void sfx(byte tone) {
  switch(tone) {
    case 1:
      arduboy.tunes.tone(820, 10);
    break;
    case 2:
      arduboy.tunes.tone(750, 50);
    break;
    case 3:
      arduboy.tunes.tone(987, 400);
    break;
    case 4:
      arduboy.tunes.tone(800, 50);
    break;
  }
}

void printText(char *message, byte x, byte y, byte textSize) {
  arduboy.setCursor(x, y);
  arduboy.setTextSize(textSize);
  arduboy.print(message);
}

void introScreen() {
  arduboy.clear();
  draw(2, 8, modusLogo, 0);
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
  draw(0, 0, title, 0);
  arduboy.drawRect(2, 47, 26, 13, 1);
  arduboy.display();

  playMusic(5);
  
  while (totalDelay < ATTRACT_MODE_TIMEOUT) {
    if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) {
      if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        break;
      } else {
        lastDebounceTime = millis();
      }
    }

    if (arduboy.pressed(LEFT_BUTTON)) {
      if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        selectedItem = titleMenuLeftButton(selectedItem);
        lastDebounceTime = millis(); //set the current time
      }
    }

    if (arduboy.pressed(RIGHT_BUTTON)) {
      if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        selectedItem = titleMenuRightButton(selectedItem);
        lastDebounceTime = millis(); //set the current time
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
      arduboy.drawRect(76, 47, 51, 13, 0);
      arduboy.drawRect(30, 47, 45, 13, 1);
      arduboy.display();
      return TITLE_CREDITS;
      break;

    case TITLE_CREDITS:
      arduboy.drawRect(30, 47, 45, 13, 0);
      arduboy.drawRect(2, 47, 26, 13, 1);
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
      arduboy.drawRect(2, 47, 26, 13, 0);
      arduboy.drawRect(30, 47, 45, 13, 1);
      arduboy.display();
      return TITLE_CREDITS;
      break;

    case TITLE_CREDITS:
      arduboy.drawRect(30, 47, 45, 13, 0);
      arduboy.drawRect(76, 47, 51, 13, 1);
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

  arduboy.clear();
  printText("HI SCORES", 8, 1, 2);
  printText("1.  000000  AAA", 15, 21, 1);
  printText("2.  000000  AAA", 15, 33, 1);
  printText("3.  000000  AAA", 15, 45, 1);
  printText("4.  000000  AAA", 15, 57, 1);
  arduboy.display();
  while (totalDelay < 4000) {
    if (arduboy.pressed(UP_BUTTON) || arduboy.pressed(DOWN_BUTTON) || arduboy.pressed(LEFT_BUTTON) || arduboy.pressed(RIGHT_BUTTON) || arduboy.pressed(A_BUTTON)  || arduboy.pressed(B_BUTTON)) {
      if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        totalDelay = 4000;
        lastDebounceTime = millis();
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

void scrollCredits(int y, bool quit) {
  /**
     Recursive function for scrolling
     credits up screen
  */
  byte padding = 7;
  byte textSize = 1;
  int origY = y;
  arduboy.clear();
  for (unsigned short i = 0; i < NUM_CREDITS; i++) {
    if (i == 0) {
      textSize = 2;
      y = y - 4;
    } else {
      textSize = 1;
      y = origY;
    }
    strcpy_P(textBuf, (char*)pgm_read_word(&(credits[i])));
    printText(textBuf, 2, y + padding, textSize);
    arduboy.display();
    padding = padding + 15;
    if ( i + 1 == NUM_CREDITS && y + padding < 0) {
      quit = true;
    }
  }
  delay(1000);
  y = y - 15;

  if (!quit) {
    scrollCredits(y, quit);
  }
}

void settingsScreen() {
  long lastDebounceTime = millis();  // the last time the button was pressed
  bool exit_settings_menu = false;
  byte selectedItem;

  arduboy.clear();
  printText("SETTINGS", 20, 5, 2);
  printMusicOnOff();
  printText("RESET HIGHSCORE", 20, 37, 1);
  printText("EXIT", 20, 49, 1);
  arduboy.drawRect(17, 22, 35, 13, 1);
  arduboy.display();

  while (!exit_settings_menu) {
    if (arduboy.pressed(DOWN_BUTTON)) {
      if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        selectedItem = settingMenuDownButton(selectedItem);
        lastDebounceTime = millis(); //set the current time
      }
    }

    if (arduboy.pressed(UP_BUTTON)) {
      if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        selectedItem = settingMenuUpButton(selectedItem);
        lastDebounceTime = millis(); //set the current time
      }
    }

    if (arduboy.pressed(A_BUTTON)) {
      if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        switch (selectedItem) {
          case SETTINGS_EXIT:
            exit_settings_menu = true;
            break;

          case SETTINGS_SOUND:
            musicOn = !musicOn;
            printMusicOnOff();
            break;
            
          case SETTINGS_RESET_HIGH_SCORE:
            highScore = 0;
            exit_settings_menu = true;
            break;

          default: break;
        }
      }
      lastDebounceTime = millis(); //set the current time
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

void printMusicOnOff() {
  if(musicOn) {
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
  score = 0;
  livesRemaining = MAX_LIVES;
  spaceShip.reset();

  while (livesRemaining > 0) {  
    arduboy.clear();
    inGameFrame++;

    drawScore();

    drawPlayerShip();
    // testing purposes, need AI
//    drawEnemies();
    drawBosses();
    
    for (byte i = 0; i < MAX_PLAYER_BULLETS; i++) {
      playerBullets[i].draw();
      playerBullets[i].update();
    }
    for (byte i = 0; i < MAX_ENEMIES; i++) {
      enemyBullets[i].draw();
      enemyBullets[i].update();
    }

    drawStarLayer();
    drawLives();
    arduboy.display();
    updateStarFieldVals();

    handlePlayerBullets();
    handleEnemyBullets();

    // Play stage1 music
    playMusic(2);
    if (shouldPlayAButtonTone()) {
      sfx(1);
    }

    if (shouldPlayBButtonTone()) {
      sfx(2);
    }
  }

  stopMusic();
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
    draw(120 - (i * 10), 0, (i < livesRemaining ? heart : unfilledHeart) , 0);
  }
}

void drawPlayerShip() {
  if (arduboy.pressed(RIGHT_BUTTON) && (spaceShip.x < MAX_PLAYER_SHIP_X)) {
    spaceShip.x++;
  }

  if (arduboy.pressed(LEFT_BUTTON) && (spaceShip.x > MIN_PLAYER_SHIP_X)) {
    spaceShip.x--;
  }

  if (arduboy.pressed(UP_BUTTON)) {
    if (spaceShip.y > MIN_PLAYER_SHIP_X) {
      spaceShip.y--;
    }
    if (arduboy.everyXFrames(9)) {
      spaceShip.frame--;
    }
    if (spaceShip.frame < 0) {
      spaceShip.frame = 0;
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
    if (inGameAButtonLastPress < (inGameFrame - 75)) {
      inGameAButtonLastPress = inGameFrame;
      // Fire A weapon (single fire)
      for (byte i = 0; i < MAX_PLAYER_BULLETS; i++) {
        if (!playerBullets[i].isVisible()) {
          playerBullets[i].set(spaceShip.x, (spaceShip.y + (16 / 2) - 1), true, 100);
          break;
        }
      }
    }
  }

  // Here to test out other SFX
  if (arduboy.pressed(B_BUTTON)) {
    if (inGameBButtonLastPress < (inGameFrame - 15)) {
      inGameBButtonLastPress = inGameFrame;
      // Fire B weapon (rapid fire)
      for (byte i = 0; i < MAX_PLAYER_BULLETS; i++) {
        if (!playerBullets[i].isVisible()) {
          playerBullets[i].set(spaceShip.x, (spaceShip.y + (16 / 2) - 1), true, 5);
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

  draw(spaceShip.x, spaceShip.y, playerShip, spaceShip.frame);
}

void drawEnemies() {
  for (byte i = 0; i < MAX_ENEMIES; i++) {
    if ((enemies[i].health <= 0) && (random(1000) == 0)) {
      byte enemyX = random(MIN_ENEMY_SHIP_X, MAX_ENEMY_SHIP_X);
      byte enemyY = random(MIN_SHIP_Y, MAX_SHIP_Y);
      enemies[i].set(enemyX, enemyY, (random(3) + 1));
    }
    
    if (enemies[i].health > 0) {
      enemies[i].move();
      draw(enemies[i].x, enemies[i].y, enemies[i].bitmap, 0);
      
      if ((!enemyBullets[i].isVisible()) && (random(1000) == 0)) {
        enemyBullets[i].set(enemies[i].x, (enemies[i].y + (16 / 2) - 1), false, 1);
      }
    }
  }
}

<<<<<<< Updated upstream
void draw(byte x, byte y, const uint8_t *bitmap, uint8_t frame) {
=======
void drawEnemy(int x, int y, int type) {
  Enemy enemy;
  enemy.set(x, y, type);
  draw(enemy.x, enemy.y, enemy.bitmap, 0);
}


void drawBosses() {
  drawBoss(69, 6, 1);
  drawBoss(96, 24, 2); 
}

void drawBoss(int x, int y, int type) {
  Boss boss;
  boss.set(x, y, type);
  draw(boss.x, boss.y, boss.bitmap, 0);
  
}
void draw(int x, int y, const uint8_t *bitmap, uint8_t frame) {
>>>>>>> Stashed changes
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

void handleEnemyBullets() {
  for (byte i = 0; i < MAX_ENEMIES; i++) {
    if ((enemyBullets[i].isVisible()) &&
        (enemyBullets[i].posX >= spaceShip.x) &&
        (enemyBullets[i].posX <= (spaceShip.x + 16)) &&
        (enemyBullets[i].posY >= spaceShip.y) &&
        (enemyBullets[i].posY <= (spaceShip.y + 16))) {
          // Hit Player
          enemyBullets[i].hide();
          livesRemaining--;
        }
  }
}

void handlePlayerBullets() {
  for (byte i = 0; i < MAX_PLAYER_BULLETS; i++) {
    if (playerBullets[i].isVisible()) {
      for (byte j = 0; j < MAX_ENEMIES; j++) {
        if ((enemies[j].health > 0) &&
            (playerBullets[i].posX >= enemies[j].x) &&
            (playerBullets[i].posX <= (enemies[j].x + 16)) &&
            (playerBullets[i].posY >= enemies[j].y) &&
            (playerBullets[i].posY <= (enemies[j].y + 16))) {
              // Hit Enemy
              playerBullets[i].hide();
              enemies[j].health -= playerBullets[i].damage;

              if (enemies[j].health <= 0) {
                score += 100;
              }
            }
      }
    }
  }
}

void gameOverScreen() {
  // TODO, this is placeholder
  arduboy.clear();
  draw(0, 0, gameOver, 0);
  arduboy.display();

  delay(100);
  // play game over tune
  playMusic(4);
  delay(3000);
  stopMusic();
  delay(2000);
}

void drawHighScoreEntryCursor(byte pos) {
  arduboy.fillRect(44, 62, 10, 2, (pos == 0 ? 1 : 0));
  arduboy.fillRect(56, 62, 10, 2, (pos == 1 ? 1 : 0));
  arduboy.fillRect(68, 62, 10, 2, (pos == 2 ? 1 : 0));
}

void newHighScoreScreen() {
  long lastDebounceTime = millis();
  bool allDone = false;
  byte currPos = 0;
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
    if (arduboy.pressed(A_BUTTON)) {
      if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        // Advance right or finish entering initials
        if (currPos < 2) {
          currPos++;
          drawHighScoreEntryCursor(currPos);
          arduboy.display();
        } else {
          allDone = true;
        }
        lastDebounceTime = millis();
      }
    }
   
    if (arduboy.pressed(LEFT_BUTTON)) {
      if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        // Only do something if we are not already on leftmost one
        if (currPos > 0) {
          currPos--;
          drawHighScoreEntryCursor(currPos);
          arduboy.display();
        }
        lastDebounceTime = millis();
      }    
    }
  
    if (arduboy.pressed(RIGHT_BUTTON)) {
      if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        // Only do something if we are not already on rightmost one
        if (currPos < 2) {
          currPos++;
          drawHighScoreEntryCursor(currPos);
          arduboy.display();
        }
        lastDebounceTime = millis();
      }
    }

    if (arduboy.pressed(UP_BUTTON)) {
       if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
          if (currInitials[currPos] == 96) {
            currInitials[currPos] = 32;
          } else {
            currInitials[currPos]++;
          }
          sprintf(textBuf, "%c%c%c", currInitials[0], currInitials[1], currInitials[2]);
          printText(textBuf, 44, 45, 2);
          arduboy.display();
          lastDebounceTime = millis();
       }
    }

    if (arduboy.pressed(DOWN_BUTTON)) {
       if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
          if (currInitials[currPos] == 32) {
            currInitials[currPos] = 96;
          } else {
            currInitials[currPos]--;
          }
          sprintf(textBuf, "%c%c%c", currInitials[0], currInitials[1], currInitials[2]);
          printText(textBuf, 44, 45, 2);
          arduboy.display();
          lastDebounceTime = millis();
       }     
    }
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
  spaceShip.set();
  createStarFieldVals();
}

// Main program loop
void loop() {
  byte result;

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
      if (score > highScore) {
        newHighScoreScreen();
        highScore = score;
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

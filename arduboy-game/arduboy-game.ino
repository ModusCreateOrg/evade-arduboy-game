/*
   arduboy-game, Modus Create 2016
*/

#include "Arduboy.h"
#include "globals.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "bitmaps.h"
#include "MusicPlayer.h"
#include "star.h"

#define DEBOUNCE_DELAY 100
#define MAX_LIVES 4

// TODO highScore should be replaced with table in EEPROM
unsigned long score, highScore = 0;
byte livesRemaining = MAX_LIVES;
int numStars = 30;
Star stars[30];

// Placeholders
bool shouldPlayTone1,
     shouldPlayTone2,
     shouldPlayTone3;

// Bullets array - We may need a playerBullets and enemyBullets at some point and a MAX global int for each
Bullet playerBullets[MAX_PLAYER_BULLETS];

// General purpose text buffer for string concatenations etc
char textBuf[15];

Player spaceShip;

void printText(char *message, byte x, byte y, byte textSize) {
  arduboy.setCursor(x, y);
  arduboy.setTextSize(textSize);
  arduboy.print(message);
}

void introScreen() {
  arduboy.clear();
  draw(0, 0, modusLogo, 0);
  arduboy.display();
  delay(250);
  playMusic(0);

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
  arduboy.clear();
  printText("HIGH SCORE ", 5, 15, 1);
  arduboy.print(highScore);
  arduboy.display();
  delay(3000);
}

void creditsScreen() {
  // TODO, this is placeholder
  const char* credits[] = {"CREDITS", "p1", "p2", "p3", "p4", "p5", "p6"};
  unsigned short arrsize = sizeof(credits) / sizeof(int);
  scrollCredits(5, arrsize, credits, false);
  delay(1000);
}

void scrollCredits(int y, unsigned short arrsize, char* credits[], bool quit) {
  /**
     Recursive function for scrolling
     creddits up screen
  */
  byte padding = 7;
  byte textSize = 1;

  arduboy.clear();
  for (unsigned short i = 0; i < arrsize; i++) {
    if (i == 0) {
      textSize = 2;
    } else {
      textSize = 1;
    }
    printText(credits[i], 20, y + padding, textSize);
    arduboy.display();
    padding = padding + 15;
    if ( i + 1 == arrsize && y + padding < 0) {
      quit = true;
    }
  }
  delay(1000);
  y = y - 15;

  if (!quit) {
    scrollCredits(y, arrsize, credits, quit);
  }
}

void settingsScreen() {
  // TODO, this is a placeholder
  long lastDebounceTime = 0;  // the last time the button was pressed
  bool exit_settings_menu = false;
  byte selectedItem;

  arduboy.clear();
  printText("SETTINGS", 20, 5, 2);
  printText("SOUND", 20, 25, 1);
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

void playGame() {
  score = 0;
  livesRemaining = MAX_LIVES;
  spaceShip.reset();

  // This still has artificial game ending mechanisms in it...
  while (livesRemaining > 0) {  
    arduboy.clear();

    drawScore();

    // TODO implement proper scoring
    score++;

    drawPlayerShip();
    drawEnemies();
    for (byte i = 0; i < MAX_PLAYER_BULLETS; i++) {
      playerBullets[i].draw();
      playerBullets[i].update();
    }

    drawStarLayer();
    drawLives();
    arduboy.display();
    updateStarFieldVals();

    // Play stage1 music
    playMusic(1);
    if (shouldPlayTone1) {
      sfx(1);
      shouldPlayTone1 = false;
    }

    if (shouldPlayTone2) {
      sfx(2);
      shouldPlayTone2 = false;
    }

    // TODO Replace dummy code that makes sure user dies
    if (score > 0 && score % 1000 == 0) {
      livesRemaining--;
    }
  }

  stopMusic();
}

void drawScore() {
  sprintf(textBuf, "%06d", score);
  printText(textBuf, 0, 0, 1);
}
void drawStarLayer() {
  for (byte i = 0; i < numStars; i++) {
    //arduboy.drawPixel(stars[i].x, stars[i].y, 1);
    arduboy.drawRect(stars[i].x, stars[i].y, stars[i].width, stars[i].height, 1);
  }
}

void drawLives() {
  for (byte i = 0; i < MAX_LIVES; i++) {
    draw(120 - (i * 10), 0, (i < livesRemaining ? heart : unfilledHeart) , 0);
  }
}

void drawPlayerShip() {
  if (arduboy.pressed(RIGHT_BUTTON) && (spaceShip.x < MAX_SHIP_X)) {
    spaceShip.x++;
  }

  if (arduboy.pressed(LEFT_BUTTON) && (spaceShip.x > MIN_SHIP_X)) {
    spaceShip.x--;
  }

  if (arduboy.pressed(UP_BUTTON)) {
    if (spaceShip.y > MIN_SHIP_X) {
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
    if (spaceShip.frame  > 4) {
      spaceShip.frame = 4;
    }
  }

  if (arduboy.pressed(A_BUTTON)) {
    shouldPlayTone1 = true;

    for (byte i = 0; i < MAX_PLAYER_BULLETS; i++) {
      if (!playerBullets[i].isVisible) {
        playerBullets[i].set(spaceShip.x, spaceShip.y + (spaceShip.height / 2) - 1);
      }
    }
  }

  // Here to test out other SFX
  if (arduboy.pressed(B_BUTTON)) {
    shouldPlayTone2 = true;    
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
  Enemy enemyType1;
  enemyType1.set(100, 16, 1);
  draw(enemyType1.x, enemyType1.y, enemyType1.bitmap, 0);

  Enemy enemyType2;
  enemyType2.set(100, 48, 2);
  draw(enemyType2.x, enemyType2.y, enemyType2.bitmap, 0);

  Enemy enemyType3;
  enemyType3.set(86, 32, 3);
  draw(enemyType3.x, enemyType3.y, enemyType3.bitmap, 0);
}

void draw(int x, int y, const uint8_t *bitmap, uint8_t frame) {
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

void gameOverScreen() {
  // TODO, this is placeholder
  arduboy.clear();
  printText("GAME OVER", 13, 28, 2);
  arduboy.display();

  // play game over tune
  playMusic(5);
  delay(3000);
  stopMusic();
}

void newHighScoreScreen() {
  // TODO, this is placeholder
  arduboy.clear();
  printText("NEW HIGHSCORE", 4, 25, 1);
  arduboy.display();
  delay(3000);
}

void createStarFieldVals() {
  for (byte i = 0; i < numStars; i++) {
     stars[i].setValues();
  } 
}

void updateStarFieldVals() {
  for (byte i = 0; i < numStars; i++) {
    if (stars[i].x < -1) {
      stars[i].x = 128 + random(20);
      stars[i].y = random(100) + 10;
    } 
    else {
        stars[i].x -= stars[i].speed;
    }
  }
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

  // TODO alternate between titleScreen and highScoreScreen on a timer
  // until user pressed a button
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

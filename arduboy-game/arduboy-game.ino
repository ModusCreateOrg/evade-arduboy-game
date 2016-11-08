/*
 * arduboy-game, Modus Create 2016
*/

#include "Arduboy.h"

// Define limits that ship movement is bounded by
#define MIN_SHIP_X 2
#define MAX_SHIP_X 75
#define MIN_SHIP_Y 10
#define MAX_SHIP_Y 57


// Time before title screen flips to high score screen
#define ATTRACT_MODE_TIMEOUT 10000

// Title screen outcomes
#define TITLE_CREDITS 0
#define TITLE_PLAY_GAME 1
#define TITLE_TIMEOUT 2

// Global variables
Arduboy arduboy;
// TODO highScore should be replaced with table in EEPROM
unsigned int score, highScore = 0;
byte livesRemaining = 4;

// Bullets array

// General purpose text buffer for string concatenations etc
char textBuf[15];

void printText(char *message, int x, int y, int textSize) {
  arduboy.setCursor(x, y);
  arduboy.setTextSize(textSize);
  arduboy.print(message); 
}

void introScreen() {
  arduboy.clear();
  printText("INTRO", 10, 20, 2);
  arduboy.display();
  delay(3000);
}

byte titleScreen() {
  byte result = TITLE_PLAY_GAME;
  unsigned short totalDelay = 0;
  
  arduboy.clear();
  printText("TITLE", 25, 20, 2);
  arduboy.drawRect(2, 47, 31, 13, 1);
  printText("PLAY", 5, 50, 1);
  printText("CREDITS", 45, 50, 1);
  arduboy.display();

  while(totalDelay < ATTRACT_MODE_TIMEOUT) {
    if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) {
      break;
    } else if (arduboy.pressed(LEFT_BUTTON)) {
      // Highlight play option
      result = TITLE_PLAY_GAME;
      arduboy.drawRect(42, 47, 48, 13, 0);
      arduboy.drawRect(2, 47, 31, 13, 1);
      arduboy.display();
    } else if (arduboy.pressed(RIGHT_BUTTON)) {
      // Highlight credits option
      result = TITLE_CREDITS;
      arduboy.drawRect(2, 47, 31, 13, 0);
      arduboy.drawRect(42, 47, 48, 13, 1);
      arduboy.display();
    }

    delay(15);
    totalDelay += 15;
  }

  if (totalDelay >= ATTRACT_MODE_TIMEOUT) {
    // User didn't press a button so title screen just timed out
    result = TITLE_TIMEOUT;
  }

  return(result);
}

void highScoreScreen() {
  // TODO, this is placeholder
  arduboy.clear();
  printText("HI SCORES", 3, 15, 2);
  arduboy.display();
  delay(3000);
}

void creditsScreen() {
  // TODO, this is placeholder
  arduboy.clear();
  printText("CREDITS", 20, 25, 2);
  arduboy.display();
  delay(5000);
}

void playGame() {
  // TODO, this is placeholder, should also use livesRemaining
  // to count down user lives
  unsigned short shipX = 2, shipY = 32;
  score = 0;

  // Random test to set score 
  unsigned int randomScore = random(4000, 15000);

  // Loop to simulate a game that ends with score being 
  // close to value of randomScore
  while (score < randomScore) {
    arduboy.clear();
    arduboy.fillRect(shipX, shipY, 6, 4, 1);
    sprintf(textBuf, "SCORE %u", score);
    printText(textBuf, 0, 0, 1);
    score += random(0, 50);
    arduboy.display();

    if (arduboy.pressed(UP_BUTTON) && shipY > MIN_SHIP_Y) {
      shipY--;
    } else if (arduboy.pressed(DOWN_BUTTON) && shipY < MAX_SHIP_Y) {
      shipY++;
    } else if (arduboy.pressed(LEFT_BUTTON) && shipX > MIN_SHIP_X) {
      shipX--;
    } else if (arduboy.pressed(RIGHT_BUTTON) && shipX < MAX_SHIP_X) {
      shipX++;
    }
  }
}

void gameOverScreen() {
  // TODO, this is placeholder
  arduboy.clear();
  printText("GAME OVER", 15, 30, 2);
  arduboy.display();
  delay(3000);
}

void newHighScoreScreen() {
  // TODO, this is placeholder
  arduboy.clear();
  printText("NEW HI", 4, 25, 2);
  arduboy.display();
  delay(3000);
}

// Initialization runs once only
void setup() {
  arduboy.beginNoLogo();
  introScreen();
}


// Main program loop
void loop() {
  byte result;
  
  // TODO alternate between titleScreen and highScoreScreen on a timer
  // until user pressed a button
  result = titleScreen();

  switch(result) {
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
    case TITLE_TIMEOUT:
      // No button pressed on title, alternate with high score  
      highScoreScreen();
      break; 
  }
}

/*   
 *    arduboy-game, Modus Create 2016
*/

#include "Arduboy.h"
#include "globals.h"

bool soundOn = true;     
void playTone(byte tone, byte duration) {
  if (soundOn) {
    arduboy.tunes.tone(200 + tone, duration);
  }
}


#include "messagecatalog.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "bitmaps.h"
#include "Music.h"
#include "letters.h"
#include <stddef.h>
#include <inttypes.h>
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>

#define DEBOUNCE_DELAY 100
#define MAX_LIVES 4
#define NUM_HIGH_SCORES 3
#define NUM_STARS 15
#define NOT_NEW_HI_SCORE 5

#define PLAYER_SIZE 16
#define MAX_GUN_CHARGE 40
#define GUN_SHOT_COST 12

byte playerX,
     playerY,
     playerFrame,
     playerDying,
     playerGunCharge;

void resetPlayer() {
    playerX = MIN_PLAYER_X;
    playerY = 32;
    playerFrame = 2;
    playerDying = 0;
    playerGunCharge = MAX_GUN_CHARGE; 
}

char *alphabet[29];

char highScoreTable[27] = "AAA000300BBB000200CCC000100";

unsigned long inGameAButtonLastPress, inGameBButtonLastPress, inGameLastDeath, score;
byte livesRemaining = MAX_LIVES;

float starX[NUM_STARS];
float starSpeed[NUM_STARS];
byte starY[NUM_STARS];
byte starWidth[NUM_STARS];

byte currentSong = 255;

// Bullet array
Bullet playerBullets[MAX_PLAYER_BULLETS];

// Enemies array
Enemy enemies[MAX_ENEMIES];

// Boss
Enemy boss;

bool isBossAlive;

// General purpose text buffer for string concatenation and read from progmem
char textBuf[23];

void redAlert() {
    arduboy.clear();

    drawChrs(3, 30, playerWon2, 125);
//  drawChrs(0, 50, playerWon3, creditsDelay);
    delay(3000);
    for (byte i = 1; i < 7; i++) {
    byte color = i % 2;
    arduboy.fillRect(0,0,128,64, color);
    display();

    for (byte z = 1; z < 150 ; z++) {
//      arduboy.drawCircle(64,32, z, color);
      playTone(z, 10);
      delay(3);
    } 
  }
}

void display() {
  arduboy.display();
}

void stopMusic(){
  arduboy.tunes.stopScore();
}

void playMusic(byte song) {
    if (! soundOn) {
      return;
    }
   
    if (currentSong != song) {
      stopMusic();
    }
    
    unsigned char *music;
    switch(song) {
      case 1 :
         music = introMusic;
      break;
      case 2 :
//          music = stage1MusicSingleTrack; // IF WE RUN OUT OF SPACE
        music = stage1MusicDoubleTrack;
      break;
      case 3 :
          music = bossMusicSingleTrack; // IF WE RUN OUT OF SPACE
//         music = bossMusicDoubleTrack;
      break;
      case 4 :
        music = gameOverMusic;
      break;
//      case 5 : 
//        music = titleMusic;
//      break;
      case 6 :
        music = youWin;
      break;
    }
    
    currentSong = song;
    if (! arduboy.tunes.playing()) {
      arduboy.tunes.playScore(music);
    }
}   


void explode(byte x, byte y, byte dying) {
  byte rnd = random(1, 15),
       _dying = dying + rnd;
       
  playTone(_dying * 5, 10);
  
  if (inGameFrame % 2 == 0) {
  
  //        byte _x = random(x, x + width), //x + width/2,
  //             _y = random(y, y + height), //y +  height/2,
  //             randOp = dying / 4, //random(1, 10),
  //             randSize = random(1, 5);
         
    arduboy.drawCircle(x, y, dying + rnd, 1);
    arduboy.drawCircle(x, y, _dying - rnd, 1);
    arduboy.drawCircle(x, y, _dying - rnd + 5, 1);
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

//  arduboy.fillRect(0,0, 128, 16,1);
//  arduboy.fillRect(0,16, 10, 64,1);
//  arduboy.fillRect(121,16, 10, 64,1);
//  arduboy.fillRect(10,54, 128, 64,1);
//
//  drawBitmap(10, 16, moduslogo_graffiti_small, 0);
  display();

  arduboy.initRandomSeed();
  
//  delay(250);
  playMusic(1);
  delay(2750);
}

void drawRectAroundPlayMenuOption(byte color) {
  arduboy.drawRect(2, 48, 28, 12, color);
}


byte titleScreen() {
  byte selectedItem = TITLE_PLAY_GAME;
  unsigned long totalDelay = 0;
  long lastDebounceTime = millis();  // the last time the button was pressed

  
  arduboy.clear();
//  arduboy.drawRect(0,0,128,64,1);
  //drawBitmap(4, 14, title, 0);
  drawBitmap(4,  16, title_letter_e, 0);
  drawBitmap(20, 16, title_letter_v, 0);
  drawBitmap(39, 16, title_letter_a, 0);
  drawBitmap(59, 16, title_letter_d, 0);
  drawBitmap(77, 16, title_letter_e, 0);

  drawBitmap(93, 14, title_planet, 0);
  

  drawChrs(4,  51, titleScreenText1, 30);
  drawChrs(34, 51, titleScreenText2, 30);
  drawChrs(79, 51, titleScreenText3, 30);
//  arduboy.drawRect(2, 48, 28, 12, 1);
  drawRectAroundPlayMenuOption(1);
  display();

//  playMusic(5);

  while (totalDelay < ATTRACT_MODE_TIMEOUT) {

    unsigned long currentMilliseconds = millis();
    bool isGreater = (currentMilliseconds - lastDebounceTime) > DEBOUNCE_DELAY;

    /*
     * UP 128
     * DN 16
     * LT 32
     * RT 64
     * A  8
     * B  4
     */
    //playMusic(5);

    if (isGreater) {

      if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) {
        break;
      } 
    
      if (arduboy.pressed(LEFT_BUTTON)) {
        selectedItem = titleMenuLeftButton(selectedItem);
        lastDebounceTime = currentMilliseconds; //set the current time
        delay(250);
      }
    

      if (arduboy.pressed(RIGHT_BUTTON)) {
        selectedItem = titleMenuRightButton(selectedItem);
        lastDebounceTime = currentMilliseconds; //set the current time
        delay(250);
      }
    }
  
    totalDelay ++;
  }
//  playMusic(99);

  
  return (totalDelay >= ATTRACT_MODE_TIMEOUT ? TITLE_TIMEOUT : selectedItem);
}


void drawRectAroundSettingsMenuOption(byte color) {
  arduboy.drawRect(32, 48, 42, 12, color);
}

byte titleMenuLeftButton(byte selectedItem) {
  /**
     Handle clicks on the left button
     to navigate through main menu
     items.
  */
  switch (selectedItem) {
    case TITLE_SETTINGS:
      arduboy.drawRect(77, 48, 50, 12, 0);
//      arduboy.drawRect(32, 48, 42, 12, 1);
      drawRectAroundSettingsMenuOption(1);
      display();
      return TITLE_CREDITS;
      break;

    case TITLE_CREDITS:
      drawRectAroundSettingsMenuOption(0);
//      arduboy.drawRect(32, 48, 42, 12, 0);
//      arduboy.drawRect(2, 48, 28, 12, 1);
      drawRectAroundPlayMenuOption(1);

      display();
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
//      arduboy.drawRect(2, 48, 28, 12, 0);
      drawRectAroundPlayMenuOption(0);

//      arduboy.drawRect(32, 48, 42, 12, 1);
      drawRectAroundSettingsMenuOption(1);
      display();
      return TITLE_CREDITS;
      break;

    case TITLE_CREDITS:
      arduboy.drawRect(32, 48, 42, 12, 0);
      arduboy.drawRect(77, 48, 50, 12, 1);
      display();
      return TITLE_SETTINGS;
      break;

    default: break;
  }
}

void highScoreScreen() {
  unsigned short totalDelay = 0;
  char hiInitials[4];
  char hiScore[7];

  arduboy.clear();
  
  printText("HIGH", 0, 1, 2);
  printText("SCORES", 58, 1, 2);
  
  for (byte i = 0; i < NUM_HIGH_SCORES; i++) {
    strncpy(hiInitials, highScoreTable + ((9 * i) * sizeof(char)), 3);
    hiInitials[3] = '\0';
    strncpy(hiScore, highScoreTable + (((9 * i) + 3) * sizeof(char)), 6);
    hiScore[6] = '\0';
    sprintf(textBuf, "%d.  %s  %s", i + 1, hiScore, hiInitials);
    printText(textBuf, 15, 26 + (12 * i), 1);
  }

  display();

  unsigned long lastDebounceTime = millis();

  while (totalDelay < 4000) {
    unsigned long currentMilliseconds = millis();
    
    if (arduboy.buttonsState() > 0) {

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


void drawChrs(byte cPos, byte yPos, const uint8_t *letters, unsigned long delayTimer) {
  byte curs = cPos,
       strLen = pgm_read_byte(letters);

//  Serial.println("\n-------------\n");
    byte letter; 
  for (byte i = 0; i <  strLen; i++) {
    letter = pgm_read_byte(++letters);

    // Space chr
    if (letter == 255) {
       curs += 5;
    }
    else {
        drawBitmap(curs,yPos, alphabet[letter], 0);
        curs += pgm_read_byte(alphabet[letter]) + 1;
    }
    
    display();

    if (delayTimer) {
      playTone(100, delayTimer - 10);
      delay(delayTimer);
    }
  }  


  if (delayTimer) {
      playTone(800, 15);
      delay(15);
      playTone(1200, 30);
      delay(40);
  }

}

void creditsScreen() {
  
  arduboy.clear();

  byte creditsDelay = 30;
  drawChrs(0, 0, credits0,  creditsDelay);
  drawChrs(0, 10, credits1, creditsDelay);
  drawChrs(0, 19, credits2, creditsDelay);
  drawChrs(0, 28, credits3, creditsDelay);
  drawChrs(0, 37, credits4, creditsDelay);
  drawChrs(0, 46, credits5, creditsDelay);
  drawChrs(0, 55, credits6, creditsDelay);
  arduboy.clear();

  delay(2000);
  
  drawChrs(0, 0, credits0,  0);
  drawChrs(0, 10, credits7, creditsDelay);
  drawChrs(0, 19, credits8, creditsDelay);
  drawChrs(0, 28, credits9, creditsDelay);
  drawChrs(0, 37, credits10, creditsDelay);
  drawChrs(0, 46, credits11, creditsDelay);

  delay(2000);
}

void drawBoxAroundSoundSetting(byte color) {
   arduboy.drawRect(17, 22, 35, 13, color);
}

void settingsScreen() {
  long lastDebounceTime = millis();  // the last time the button was pressed
  bool exit_settings_menu = false;
  bool isRealSoundChange = false;
  byte selectedItem = SETTINGS_SOUND;

  arduboy.clear();
  printText("SETTINGS", 20, 2, 2);
  printsoundOnOff();
  printText("RESET HIGHSCORE", 20, 37, 1);
  printText("EXIT", 20, 49, 1);

  while (!exit_settings_menu) {
    unsigned long currentMilliseconds = millis();
    bool isGreater = (currentMilliseconds - lastDebounceTime) > DEBOUNCE_DELAY;

    if (isGreater) {
      
      if (arduboy.pressed(DOWN_BUTTON)) {
        switch (selectedItem) {
      
          case SETTINGS_SOUND:
            drawBoxAroundSoundSetting(0);
            arduboy.drawRect(17, 34, 95, 13, 1);

            selectedItem = SETTINGS_RESET_HIGH_SCORE;
            break;
          case SETTINGS_RESET_HIGH_SCORE:
            arduboy.drawRect(17, 34, 95, 13, 0);
            arduboy.drawRect(17, 46, 29, 13, 1);
            
            selectedItem = SETTINGS_EXIT;
            break;

          default: break;
        }
                    
        display();
        delay(250);
        lastDebounceTime = currentMilliseconds; //set the current time
      }
  
      if (arduboy.pressed(UP_BUTTON)) {

        if (selectedItem == SETTINGS_EXIT) {
          arduboy.drawRect(17, 46, 29, 13, 0);
          arduboy.drawRect(17, 34, 95, 13, 1);
  
          selectedItem = SETTINGS_RESET_HIGH_SCORE;
        }
        else if (selectedItem == SETTINGS_RESET_HIGH_SCORE) {
          arduboy.drawRect(17, 34, 95, 13, 0);
          drawBoxAroundSoundSetting(1);
          
          selectedItem = SETTINGS_SOUND;
        }
        
        display();
        delay(250);
        lastDebounceTime = currentMilliseconds; //set the current time
      }


 
      if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) {
        switch (selectedItem) {
          case SETTINGS_EXIT:
            exit_settings_menu = true;
            break;

          case SETTINGS_SOUND:
            if (isRealSoundChange) {
              soundOn = !soundOn;
              printsoundOnOff();
            } else {
              isRealSoundChange = true;
            }
            break;

          case SETTINGS_RESET_HIGH_SCORE: 
            resetHighScoreTable();
            exit_settings_menu = true;
            break;
        
          default: break;
        }  

        display();
        delay(250);
        
        lastDebounceTime = currentMilliseconds; //set the current time
      }
      
//      display();
      delay(15);
    }
     
  }
}

void printsoundOnOff() {
  printText(soundOn ? "SOUND ON " : "SOUND OFF", 20, 25, 1);
  drawBoxAroundSoundSetting(1);
}

void playGame() {
  stopMusic();

  inGameFrame = 0;
  inGameAButtonLastPress = 0;
  inGameBButtonLastPress = 0;
  inGameLastDeath = 0;
  score = 0;
  livesRemaining = MAX_LIVES;
  resetPlayer();
  isBossAlive = false;
  byte spawnedBoss = 0;
  byte currentIteration = 0;

  resetEnemies();
  resetBoss();
  resetPlayerBullets();

  redAlert();
  while (livesRemaining > 0) {
    boolean stopSpawningEnemies = false;
    
    arduboy.clear();
    inGameFrame++;

    if (inGameFrame % 20 == 0) {
      score++;

      if (playerGunCharge < MAX_GUN_CHARGE) {
        // Charge up the gun
        playerGunCharge++;
//        playerGunCharge = MAX_GUN_CHARGE; // FOR DEBUG
        
      }
    }

    drawGunTemp();
    drawScore();
    drawPlayerShip();

    boolean enemiesAlive = false;
    for (byte i = 0; i < MAX_ENEMIES; i++) {
      if (enemies[i].isAlive()) {
        enemiesAlive = true;
        break;
      }
    }

    // This logic seems way too nested and can probably be simplified a little. :) -- JG
    if (!isBossAlive) {
      if ((score >= BOSS1_SCORE + (currentIteration * BOSS3_SCORE)) && (spawnedBoss < 1)) {        
        if (isBossAlive = stopSpawningEnemies = !enemiesAlive) {
          boss.set(129, 28, 128, currentIteration);
          spawnedBoss = 1;
//          isBossAlive = true;
        } 
//        else {
//          stopSpawningEnemies = true;
//        }
      } else if ((score >= (BOSS2_SCORE + (currentIteration * BOSS3_SCORE))) && (spawnedBoss < 2)) {
        if (isBossAlive = stopSpawningEnemies = !enemiesAlive) {
          boss.set(129, 24, 129, currentIteration);
          spawnedBoss = 2;
//          isBossAlive = true;
        } 
//        else {
//          stopSpawningEnemies = true;
//        }
      } else if ((score >= (BOSS3_SCORE + (currentIteration * BOSS3_SCORE))) && (spawnedBoss < 3)) {
        if (isBossAlive = stopSpawningEnemies =!enemiesAlive) {
          boss.set(129, 10, 130, currentIteration);
          spawnedBoss = 3;
//          isBossAlive = true;
        } 
//        else {
//          stopSpawningEnemies = true;
//        }
      }
    }
    
    if (isBossAlive) {
      stopSpawningEnemies = !stopSpawningEnemies;
    }

    
    boss.update(! isBossAlive, currentIteration);

    for (byte i = 0; i < MAX_ENEMIES; i++) {
       enemies[i].update(stopSpawningEnemies, currentIteration);
    }

    if(inGameAButtonLastPress > 80 || inGameBButtonLastPress > 60) {
//       for (byte i = 0; i < MAX_ENEMIES; i++) {
//       playerBullets[i].update(); 
//       } 

       playerBullets[0].update(); 
       playerBullets[1].update(); 
       playerBullets[2].update(); 
       playerBullets[3].update();
    }

    drawStarLayer();
    drawLives();
       
    display();

    updateStarFieldVals();

    if (handlePlayerBullets()) {
      playerWinsScreen();

      // Reset which boss spawns for next iteration
      spawnedBoss = 0;

      // Give the player their lives back, end any dying sequence etx
      livesRemaining = MAX_LIVES;
      resetPlayer();
      resetEnemies();
      resetBoss();

      // Set them on the next iteration
      currentIteration = (currentIteration == 254 ? 0 : currentIteration + 1);
    }
    
    handleEnemyBullets();
    handleBossBullets();
      
    playMusic(isBossAlive ? 3 : 2);

  }

  stopMusic();
}

void drawGunTemp() {
  arduboy.drawRect(40, 1, 40, 5, 1);
  arduboy.fillRect(40, 1, (playerGunCharge), 5, 1);
}

void drawScore() {
  sprintf(textBuf, "%06lu", score);
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
  if (playerDying == 0) {
    if (arduboy.pressed(RIGHT_BUTTON) && (playerX < MAX_PLAYER_X)) {
      playerX++;
    }
  
    if (arduboy.pressed(LEFT_BUTTON) && (playerX > MIN_PLAYER_X)) {
      playerX--;
    }

    bool every9Frames = arduboy.everyXFrames(9);
  
    if (arduboy.pressed(UP_BUTTON)) {
      if (playerY > MIN_PLAYER_Y) {
        playerY--;
      }
      if (every9Frames) {
        if (playerFrame > 0) {
          playerFrame--;
        }
      }
    }
  
    if (arduboy.pressed(DOWN_BUTTON)) {
      if (playerY < MAX_PLAYER_Y)  {
        playerY++;
      }
      if (every9Frames) {
        if (playerFrame < 4) {
          playerFrame++;
        }
      }
//      if (playerFrame > 4) {
//        playerFrame = 4;
//      }
    }
  
    if (arduboy.pressed(A_BUTTON)) {
      if ((inGameFrame > 80) && (inGameAButtonLastPress < (inGameFrame - 75)) && (playerGunCharge >= GUN_SHOT_COST)) {
        inGameAButtonLastPress = inGameFrame;
        // Fire A weapon (single fire) if weapon isn't too hot
        for (byte i = 0; i < MAX_PLAYER_BULLETS; i++) {
          if (!playerBullets[i].isVisible()) {
            playerBullets[i].set(playerX + 16, playerY + 4, true, A_BULLET_DAMAGE, 2.5, false);

            if (playerGunCharge > GUN_SHOT_COST) {
              playerGunCharge -= GUN_SHOT_COST;
            } else {
              playerGunCharge = 0;
            }
            
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
        bool isGreater = inGameBButtonLastPress > 80;
        for (byte i = 0; i < MAX_PLAYER_BULLETS; i++) {
          if (isGreater && !playerBullets[i].isVisible()) {
            playerBullets[i].set(playerX + 16, playerY + 7, true, B_BULLET_DAMAGE, 3, false);
            break;
          }
        }
      }
    }
  
    if (arduboy.notPressed(UP_BUTTON) && arduboy.notPressed(DOWN_BUTTON)) {
      if (arduboy.everyXFrames(12)) {
        if (playerFrame > 2) {
          playerFrame--;
        }
        if (playerFrame < 2) {
          playerFrame++;
        }
      }
    }

    if ((inGameLastDeath > 0) && (inGameLastDeath > (inGameFrame - 450))) {
      if (!(inGameFrame % 3)) {
        // Blink ship
        drawBitmap(playerX, playerY, playerShip, playerFrame);
      }
    } else {
      drawBitmap(playerX, playerY, playerShip, playerFrame);
    }

    
  } else {
//    playTone((playerDying % 2 == 0) ? (400 + playerDying * 2) : (600 - playerDying * 2), 10);
    explode(playerX + 8, playerY + 8, playerDying);
//    arduboy.drawCircle(playerX, playerY, playerDying , 1);


    if (playerDying < 50) {
      playerDying++;
    } else {
      livesRemaining--;
      resetPlayer();
    }
  }
}

//void updateEnemies(boolean stopSpawningEnemies) {
//  for (byte i = 0; i < MAX_ENEMIES; i++) {
//    enemies[i].update(stopSpawningEnemies);
//  }
//}

void handleEnemyBullets() {
  for (byte i = 0; i < MAX_ENEMIES; i++) {
    if ((enemies[i].bullets[0].isVisible()) && (enemies[i].bullets[0].isHittingObject(playerX, playerY, PLAYER_SIZE, PLAYER_SIZE))) {
      // Hit Player
      enemies[i].bullets[0].hide();

      // Doesn't count if player recently died
      if (inGameLastDeath < (inGameFrame - 450)) {
        inGameLastDeath = inGameFrame;
        playerDying = 1;
      }
    }
  }
}

void handleBossBullets() {
  if (isBossAlive) {
    for (byte i = 0; i < MAX_BOSS_BULLETS; i++) {
      if (boss.bullets[i].isVisible() && boss.bullets[i].isHittingObject(playerX, playerY, PLAYER_SIZE, PLAYER_SIZE)) {
        // Hit Player
        boss.bullets[i].hide();
  
        // Doesn't count if player recently died
        if (inGameLastDeath < (inGameFrame - 450)) {
          inGameLastDeath = inGameFrame;
          playerDying = 1;
        }
      }
    }
  }
}

boolean handlePlayerBullets() {
  for (byte i = 0; i < MAX_PLAYER_BULLETS; i++) {
    if (playerBullets[i].isVisible()) {
      if (isBossAlive) {
        if ((boss.health > 0) && (playerBullets[i].isHittingObject(boss.x, boss.y, boss.width, boss.height))) {
          // Hit Boss
          playerBullets[i].hide();
          boss.health -= playerBullets[i].damage;
          score += playerBullets[i].damage;

          boss.takeDamage();
          if (boss.health <= 0) {
            boss.dying = 1;
            score += 500;

            if (boss.type == 130) {
              isBossAlive = false;
              return true;
            }
          }
          
        }
      } else {
        for (byte j = 0; j < MAX_ENEMIES; j++) {
          
          if ((enemies[j].health > 0) && (playerBullets[i].isHittingObject(enemies[j].x, enemies[j].y, 16, 16))) {
            // Hit Enemy
            playerBullets[i].hide();
            score += playerBullets[i].damage;
            
            if (playerBullets[i].damage >= enemies[j].health) {
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

  if (boss.dying > 1) {
    isBossAlive = false;
  }

  return false;
}

void playerWinsScreen() {
//  playMusic(6);

  byte creditsDelay = 40;
//  stopMusic();
//  resetBoss();
  resetEnemies();
  arduboy.clear();

  drawChrs(18, 25, playerWon0,  creditsDelay);
  drawChrs(3, 40, playerWon1, creditsDelay);
  playMusic(6);
  delay(2500);



  redAlert();
}


void gameOverScreen() {
  stopMusic();
  arduboy.clear();
  
//  drawBitmap(0, 23, gameOver, 0);
//  arduboy.drawFastHLine(0, 63, 128, 1);
  playMusic(4);
  
  printText("GAME", 40, 15, 2);
  display();
  delay(1000);

  printText("OVER", 40, 35, 2);
  display();

  delay(4500);
}

void drawHighScoreEntryCursor(byte pos) {
  arduboy.fillRect(44, 62, 10, 2, (pos == 0 ? 1 : 0));
  arduboy.fillRect(56, 62, 10, 2, (pos == 1 ? 1 : 0));
  arduboy.fillRect(68, 62, 10, 2, (pos == 2 ? 1 : 0));
}

byte isNewHighScore() {
  char hiScore[7];
  hiScore[6] = '\0';
  for (byte i = 0; i < NUM_HIGH_SCORES; i++) {
    strncpy(hiScore, highScoreTable + ((9 * i) + 3), 6);
    if (score > strtol(hiScore, NULL, 10)) {
       return i;
    }
  }
  return NOT_NEW_HI_SCORE;
}

void newHighScoreScreen(byte newHiPos) {
  long lastDebounceTime = millis();
  bool allDone = false;
  short currPos = 0;
  byte currInitials[] = {65, 65, 65};
  
  arduboy.clear();
  printText("NEW HI!", 24, 1, 2);
  sprintf(textBuf, "%06lu", score);
  printText(textBuf, 28, 22, 2);
  sprintf(textBuf, "%c%c%c", currInitials[0], currInitials[1], currInitials[2]);
  printText(textBuf, 44, 45, 2);
  drawHighScoreEntryCursor(currPos);

  display();

  while(! allDone) {
    unsigned long currentMilliseconds = millis();
    const bool isGreater = (currentMilliseconds - lastDebounceTime) > DEBOUNCE_DELAY;

    if (isGreater) {
      if (arduboy.pressed(A_BUTTON)) {
          // Advance right or finish entering initials
          if (currPos < 2) {
            currPos++;
            drawHighScoreEntryCursor(currPos);
          } 
          else {
            allDone = true;
          }
          lastDebounceTime = currentMilliseconds;
      }
       
      if (arduboy.pressed(LEFT_BUTTON)) {
        // Only do something if we are not already on leftmost one
        if (currPos > 0) {
          currPos--;
          drawHighScoreEntryCursor(currPos);
        }
        lastDebounceTime = currentMilliseconds;
      }    
  
      if (arduboy.pressed(RIGHT_BUTTON)) {
        // Only do something if we are not already on rightmost one
        if (currPos < 2) {
          currPos++;
          drawHighScoreEntryCursor(currPos);
        }
        lastDebounceTime = currentMilliseconds;
      }

      if (arduboy.pressed(UP_BUTTON)) {
          if (currInitials[currPos] == 96) {
            currInitials[currPos] = 32;
          } else {
            currInitials[currPos]++;
          }
          
          sprintf(textBuf, "%c%c%c", currInitials[0], currInitials[1], currInitials[2]);
          printText(textBuf, 44, 45, 2);
          lastDebounceTime = currentMilliseconds;
       }

       if (arduboy.pressed(DOWN_BUTTON)) {
          if (currInitials[currPos] == 32) {
            currInitials[currPos] = 96;
          } else {
            currInitials[currPos]--;
          }
          sprintf(textBuf, "%c%c%c", currInitials[0], currInitials[1], currInitials[2]);
          printText(textBuf, 44, 45, 2);
          lastDebounceTime = currentMilliseconds;
       }    
       display();
 
    }
  }

  // Store the new high score, newHiPos == 0 is highest score
  sprintf(textBuf, "%c%c%c%06lu", currInitials[0], currInitials[1], currInitials[2], score);

  if (newHiPos < 2) {
    // shuffle existing results around

    for (currPos = 17; currPos >= (9 * newHiPos); currPos--) {
      highScoreTable[currPos + 9] = highScoreTable[currPos];
    }
  }

  // then copy new result into correct place
  for (currPos = 0; currPos < 9; currPos++) {
    highScoreTable[currPos + (9 * newHiPos)] = textBuf[currPos];
  }

  persistHighScoreTable(false);
}

void persistHighScoreTable(boolean firstTime) {  
  for (byte i = 0; i < 27; i++) {
    EEPROM.write(i + 2, highScoreTable[i]);
    if (firstTime) {
      // Also write to the reset location starting at 29
      EEPROM.write(i + 29, highScoreTable[i]);
    }
  }
}

void resetHighScoreTable() {
  // Read it back into memory and overwrite what's in EEPROM
  // with data frem reset location
  
  for (byte i = 0; i < 27; i++) {
    highScoreTable[i] = EEPROM.read(i + 29);
  }

  persistHighScoreTable(false);
}

void createStarFieldVals() {
  for (byte i = 0; i < NUM_STARS; i++) {
      setStarValuesForIndex(i);
  } 
}

void resetEnemies() {
  for (byte i = 0; i < MAX_ENEMIES; i++) {
    enemies[i].health = 0; 
    enemies[i].bullets[0].hide();
  }
}

void resetBoss() {
  boss.health = 0;
  boss.dying = 0;
  for (byte i = 0; i < MAX_BOSS_BULLETS; i++) {
    boss.bullets[i].hide();
  }
}

void resetPlayerBullets() {
  for (byte i = 0; i < MAX_PLAYER_BULLETS; i++) {
    playerBullets[i].hide();
  }
}

void setStarValuesForIndex(byte i) {
  starX[i] = random(250);
  starY[i] = random(11, 64);
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
      starY[i] = random(11, 64);
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

  // Uncomment to test high score reset on device that
  // previously tested EEPROM high scores before high 
  // score reset code was added.  Then remove!
  //EEPROM.write(0, 0);
  //EEPROM.write(1, 0);
  
  // Check for hi scores in EEPROM
  if (EEPROM.read(0) == 254) {
    // Scores were in EEPROM
    for (byte i = 0; i < 27; i++) {
      highScoreTable[i] = EEPROM.read(i + 2);
    }
  } else {
    persistHighScoreTable(true);

    // And write initial signature
    EEPROM.write(0, 254);
  }
 
  introScreen();
  resetPlayer();
  createStarFieldVals();

  alphabet[0] = A;
  alphabet[1] = B;
  alphabet[2] = C;
  alphabet[3] = D;
  alphabet[4] = E;
  alphabet[5] = F;
  alphabet[6] = G;
  alphabet[7] = H;
  alphabet[8] = I;
  alphabet[9] = J;
  alphabet[10] = K;
  alphabet[11] = L;
  alphabet[12] = M;
  alphabet[13] = N;
  alphabet[14] = O;
  alphabet[15] = P;
  alphabet[16] = Q;
  alphabet[17] = R;
  alphabet[18] = S;
  alphabet[19] = T;
  alphabet[20] = U;
  alphabet[21] = V;
  alphabet[22] = W;
  alphabet[23] = X;
  alphabet[24] = Y;
  alphabet[25] = Z;
  alphabet[26] = colon;
  alphabet[27] = period;
  alphabet[28] = exclamation;
}

// Main program loop
void loop() {
  byte result, newHiScorePos;
//  playerWinsScreen(); // FOR DEBUG
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

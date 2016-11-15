/*
 * globals
*/
#ifndef GLOBALS_H
#define GLOBALS_H

#include "Arduboy.h"

// Define limits that ship movement is bounded by
#define MIN_SHIP_Y 2
#define MAX_SHIP_Y 48

// Define limits that player ship movement is bounded by
#define MIN_PLAYER_SHIP_X 2
#define MAX_PLAYER_SHIP_X 75

// Define limits that enemy ship movement is bounded by
#define MIN_ENEMY_SHIP_X 92
#define MAX_ENEMY_SHIP_X 112

// Time before title screen flips to high score screen
#define ATTRACT_MODE_TIMEOUT 10000

// Title screen outcomes
#define TITLE_CREDITS 0
#define TITLE_PLAY_GAME 1
#define TITLE_SETTINGS 2
#define TITLE_TIMEOUT 3

// Settings screen outcomes
#define SETTINGS_SOUND 0
#define SETTINGS_RESET_HIGH_SCORE 1
#define SETTINGS_EXIT 2

#define MAX_PLAYER_BULLETS 8
#define MAX_ENEMIES 3

#define STAR_COUNT 30

Arduboy arduboy;

#endif

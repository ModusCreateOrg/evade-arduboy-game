/*
 * globals
*/
#ifndef GLOBALS_H
#define GLOBALS_H

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

Arduboy arduboy;

#endif
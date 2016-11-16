#ifndef MESSAGECATALOG_H
#define MESSAGECATALOG_H

// High Score screen

PROGMEM const char highScoreTitle[] = { "HI SCORES" };

// New High Score screen

PROGMEM const char newHighScoreTitle[] = { "NEW HIGH SCORE" };

// Settings screen

PROGMEM const char settingsTitle[] = { "SETTINGS" };
PROGMEM const char settingsResetHighScore[] = { "RESET HIGH SCORE" };
PROGMEM const char settingsExit[] = { "EXIT" };
PROGMEM const char settingsSoundOn[] = { "SOUND  ON " };

// Credits screen

#define NUM_CREDITS 12

PROGMEM const char credits0[] = "CREDITS";
PROGMEM const char credits1[] = "Captain: Jay Garcia";
PROGMEM const char credits2[] = "Medic: Simon Prickett";
PROGMEM const char credits3[] = "Lt: Stan Bershadskiy"; 
PROGMEM const char credits4[] = "Helmsman: Andrew Owen"; 
PROGMEM const char credits5[] = "Hologram: Andy Dennis"; 
PROGMEM const char credits6[] = "Gunner: Timothy Eagan"; 
PROGMEM const char credits7[] = "Tech: Drew Griffith"; 
PROGMEM const char credits8[] = "Loadmaster: JD Jones"; 
PROGMEM const char credits9[] = "Purser: Jon Van Dalen"; 
PROGMEM const char credits10[] = "Mechanic: Lucas Still"; 
PROGMEM const char credits11[] = "Comms: Matt McCants";

PROGMEM const char * const credits[] = {
  credits0, credits1, credits2, credits3, credits4, credits5, credits6, credits7, credits8, credits9, credits10, credits11
};

#endif

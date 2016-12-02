#ifndef MESSAGECATALOG_H
#define MESSAGECATALOG_H

// Credits screen

/*  JAVASCRIPT To generate this stuff below: (Paste in JSFIddle.net)
var map = {
    'A' : 0,
    'B' : 1,
    'C' : 2,
    'D' : 3,
    'E' : 4,
    'F' : 5,
    'G' : 6,
    'H' : 7,
    'I' : 8,
    'J' : 9,
    'K' : 10,
    'L' : 11,
    'M' : 12,
    'N' : 13,
    'O' : 14,
    'P' : 15,
    'Q' : 16,
    'R' : 17,
    'S' : 18,
    'T' : 19,
    'U' : 20,
    'V' : 21,
    'W' : 22,
    'X' : 23,
    'Y' : 24,
    'Z' : 25,
    ':' : 26,
    '.' : 27,
    ' ' : 255
}

var credits = [
     "CREDITS",
     "Jay Garcia",
     "Simon Prickett",
     "Stan Bershadskiy", 
     "Andrew Owen", 
     "Andy Dennis", 
     "Tim Eagan", 
     "Drew Griffith", 
     "JD Jones", 
     "Jon Van Dalen", 
     "Lucas Still", 
     "Matt McCants",
     "Play",
     "Settings",
     "Credits"
]

var x = 0;

var endStr = '\n\n';

credits.forEach(function(creditStr, index) {
  creditStr = creditStr.toUpperCase();
    
    var header = `\n\n//${creditStr}\nPROGMEM const uint8_t credits${index}[] = {\n`,
      outStr = '';
    creditStr = creditStr.split('');
    
    
    creditStr.forEach(function(str) {
      if (outStr.length > 0) {
          outStr += ', '; 
        }
      outStr += map[str];
    });
    
    outStr = creditStr.length +',// Number characters\n' + outStr + '\n};';
    
    //console.log(header, outStr)
    endStr += (header + outStr);
    
})

console.log(endStr);
 */


//YOU SURVIVED BUT
PROGMEM const uint8_t playerWon0[] = {
  16, // Number characters
  24, 14, 20, 255, 18, 20, 17, 21, 8, 21, 4, 3, 255, 1, 20, 19
};

//THE MISSION CONTINUES.
PROGMEM const uint8_t playerWon1[] = {
  22,
  19, 7, 4, 255, 12, 8, 18, 18, 8, 14, 13, 255, 2, 14, 13, 19, 8, 13, 20, 4, 18, 27
};

//ENEMIES APPROACHING!
PROGMEM const uint8_t playerWon2[] = {
  19,
  4, 13, 4, 12, 8, 4, 18, 255, 0, 15, 15, 17, 14, 0, 2, 7, 8, 13, 6, //28
};

//ALIENS HAVE INVADED.
PROGMEM const uint8_t preIntro0[] = {
  20,
  0, 11, 8, 4, 13, 18, 255, 7, 0, 21, 4, 255, 8, 13, 21, 0, 3, 4, 3, 27
};

//THEY ARE DESTROYING
PROGMEM const uint8_t preIntro1[] = {
  19,
  19, 7, 4, 24, 255, 0, 17, 4, 255, 3, 4, 18, 19, 17, 14, 24, 8, 13, 6
};

//EVERYTHING.
PROGMEM const uint8_t preIntro2[] = {
  11,
  4, 21, 4, 17, 24, 19, 7, 8, 13, 6, 27
};

//YOUR ONLY OPTION...
PROGMEM const uint8_t preIntro3[] = {
  19,
  24, 14, 20, 17, 255, 14, 13, 11, 24, 255, 14, 15, 19, 8, 14, 13, 27, 27, 27
};

//CREDITS:
PROGMEM const uint8_t credits0[] = {
  8,
  2, 17, 4, 3, 8, 19, 18, 26
};

//J. GARCIA
PROGMEM const uint8_t credits1[] = {
  9,
  9, 27, 255, 6, 0, 17, 2, 8, 0
};

//S. PRICKETT
PROGMEM const uint8_t credits2[] = {
  11,
  18, 27, 255, 15, 17, 8, 2, 10, 4, 19, 19
};

//S. BERSHADSKIY
PROGMEM const uint8_t credits3[] = {
  14,
  18, 27, 255, 1, 4, 17, 18, 7, 0, 3, 18, 10, 8, 24
};

//A. OWEN
PROGMEM const uint8_t credits4[] = {
  7,
  0, 27, 255, 14, 22, 4, 13
};

//A. DENNIS
PROGMEM const uint8_t credits5[] = {
  9,
  0, 27, 255, 3, 4, 13, 13, 8, 18
};

//T. EAGAN
PROGMEM const uint8_t credits6[] = {
  8,
  19, 27, 255, 4, 0, 6, 0, 13
};

//D. GRIFFITH
PROGMEM const uint8_t credits7[] = {
  11,
  3, 27, 255, 6, 17, 8, 5, 5, 8, 19, 7
};

//JD JONES
PROGMEM const uint8_t credits8[] = {
  8,
  9, 3, 255, 9, 14, 13, 4, 18
};

//J. VAN DALEN
PROGMEM const uint8_t credits9[] = {
  12,
  9, 27, 255, 21, 0, 13, 255, 3, 0, 11, 4, 13
};

//L. STILL
PROGMEM const uint8_t credits10[] = {
  8,
  11, 27, 255, 18, 19, 8, 11, 11
};

//M. MCCANTS
PROGMEM const uint8_t credits11[] = {
  10,
  12, 27, 255, 12, 2, 2, 0, 13, 19, 18
};

//PLAY
PROGMEM const uint8_t titleScreenText1[] = {
  4,
  15, 11, 0, 24
};

//CREDITS
PROGMEM const uint8_t titleScreenText2[] = {
  7,
  2, 17, 4, 3, 8, 19, 18
};

//SETTINGS
PROGMEM const uint8_t titleScreenText3[] = {
  8,
  18, 4, 19, 19, 8, 13, 6, 18
};

#endif

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
     "CREDITS:",
     "Jay Garcia",
     "Simon Prickett",
     "Stan Bershadskiy", 
     "Andrew Owen", 
     "Andrew Dennis", 
     "Timothy Eagan", 
     "Drew Griffith", 
     "JD Jones", 
     "Jonathan Van Dalen", 
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
16,// Number characters
24, 14, 20, 255, 18, 20, 17, 21, 8, 21, 4, 3, 255, 1, 20, 19
};

//THE MISSION CONTINUES.
PROGMEM const uint8_t playerWon1[] = {
22,// Number characters
19, 7, 4, 255, 12, 8, 18, 18, 8, 14, 13, 255, 2, 14, 13, 19, 8, 13, 20, 4, 18, 27
};

//ENEMIES APPROACHING!
PROGMEM const uint8_t playerWon2[] = {
20,// Number characters
4, 13, 4, 12, 8, 4, 18, 255, 0, 15, 15, 17, 14, 0, 2, 7, 8, 13, 6, 28
};

//PREPARE FOR BATTLE!
PROGMEM const uint8_t playerWon3[] = {
19,// Number characters
15, 17, 4, 15, 0, 17, 4, 255, 5, 14, 17, 255, 1, 0, 19, 19, 11, 4, 28
};


//CREDITS:
PROGMEM const uint8_t credits0[] = {
8,// Number characters
2, 17, 4, 3, 8, 19, 18, 26
};

//JAY GARCIA
PROGMEM const uint8_t credits1[] = {
10,// Number characters
9, 0, 24, 255, 6, 0, 17, 2, 8, 0
};

//SIMON PRICKETT
PROGMEM const uint8_t credits2[] = {
14,// Number characters
18, 8, 12, 14, 13, 255, 15, 17, 8, 2, 10, 4, 19, 19
};

//STAN BERSHADSKIY
PROGMEM const uint8_t credits3[] = {
16,// Number characters
18, 19, 0, 13, 255, 1, 4, 17, 18, 7, 0, 3, 18, 10, 8, 24
};

//ANDREW OWEN
PROGMEM const uint8_t credits4[] = {
11,// Number characters
0, 13, 3, 17, 4, 22, 255, 14, 22, 4, 13
};

//ANDREW DENNIS
PROGMEM const uint8_t credits5[] = {
13,// Number characters
0, 13, 3, 17, 4, 22, 255, 3, 4, 13, 13, 8, 18
};

//TIMOTHY EAGAN
PROGMEM const uint8_t credits6[] = {
13,// Number characters
19, 8, 12, 14, 19, 7, 24, 255, 4, 0, 6, 0, 13
};

//DREW GRIFFITH
PROGMEM const uint8_t credits7[] = {
13,// Number characters
3, 17, 4, 22, 255, 6, 17, 8, 5, 5, 8, 19, 7
};

//JD JONES
PROGMEM const uint8_t credits8[] = {
8,// Number characters
9, 3, 255, 9, 14, 13, 4, 18
};

//JONATHAN VAN DALEN
PROGMEM const uint8_t credits9[] = {
18,// Number characters
9, 14, 13, 0, 19, 7, 0, 13, 255, 21, 0, 13, 255, 3, 0, 11, 4, 13
};

//LUCAS STILL
PROGMEM const uint8_t credits10[] = {
11,// Number characters
11, 20, 2, 0, 18, 255, 18, 19, 8, 11, 11
};

//MATT MCCANTS
PROGMEM const uint8_t credits11[] = {
12,// Number characters
12, 0, 19, 19, 255, 12, 2, 2, 0, 13, 19, 18
};



//PLAY
PROGMEM const uint8_t titleScreenText1[] = {
4,// Number characters
15, 11, 0, 24
};

//CREDITS
PROGMEM const uint8_t titleScreenText2[] = {
7,// Number characters
2, 17, 4, 3, 8, 19, 18
};



//SETTINGS
PROGMEM const uint8_t titleScreenText3[] = {
8,// Number characters
18, 4, 19, 19, 8, 13, 6, 18
};

#endif

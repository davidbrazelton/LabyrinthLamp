/* Program to automatically generate maze puzzles
 *
 * For use with Adafruit Metro M4 Express Airlift or Metro M4 and tricolor e-Paper Display Shield
 * 
 * Adafruit invests time and resources providing this open source code.
 * Please support Adafruit and open source hardware by purchasing
 * products from Adafruit.com!
 * 
 * Written by Dan Cogliano for Adafruit Industries
 * Copyright (c) 2019 Adafruit Industries MIT License, all text must be preserved
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <FastLED.h>
#include "AiEsp32RotaryEncoder.h"
#include "Arduino.h"
#include <Preferences.h>

// https://macetech.github.io/FastLED-XY-Map-Generator/
// Params for width and height
const uint8_t kMatrixWidth = 16;  //16
const uint8_t kMatrixHeight = 40; //32
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[NUM_LEDS];

//const uint8_t XYTable[] = {
//     7,   6,   5,   4,   3,   2,   1,   0,
//     8,   9,  10,  11,  12,  13,  14,  15,
//    23,  22,  21,  20,  19,  18,  17,  16,
//    24,  25,  26,  27,  28,  29,  30,  31,
//    39,  38,  37,  36,  35,  34,  33,  32,
//    40,  41,  42,  43,  44,  45,  46,  47,
//    55,  54,  53,  52,  51,  50,  49,  48,
//    56,  57,  58,  59,  60,  61,  62,  63
//  };
//  const uint8_t XYTable[] = {
//     7,   6,   5,   4,   3,   2,   1,   0,
//     8,   9,  10,  11,  12,  13,  14,  15,
//    23,  22,  21,  20,  19,  18,  17,  16,
//    24,  25,  26,  27,  28,  29,  30,  31,
//    39,  38,  37,  36,  35,  34,  33,  32,
//    40,  41,  42,  43,  44,  45,  46,  47,
//    55,  54,  53,  52,  51,  50,  49,  48,
//    56,  57,  58,  59,  60,  61,  62,  63,
//    71,  70,  69,  68,  67,  66,  65,  64,
//    72,  73,  74,  75,  76,  77,  78,  79,
//    87,  86,  85,  84,  83,  82,  81,  80,
//    88,  89,  90,  91,  92,  93,  94,  95,
//   103, 102, 101, 100,  99,  98,  97,  96,
//   104, 105, 106, 107, 108, 109, 110, 111,
//   119, 118, 117, 116, 115, 114, 113, 112,
//   120, 121, 122, 123, 124, 125, 126, 127,
//   135, 134, 133, 132, 131, 130, 129, 128,
//   136, 137, 138, 139, 140, 141, 142, 143,
//   151, 150, 149, 148, 147, 146, 145, 144,
//   152, 153, 154, 155, 156, 157, 158, 159,
//   167, 166, 165, 164, 163, 162, 161, 160,
//   168, 169, 170, 171, 172, 173, 174, 175,
//   183, 182, 181, 180, 179, 178, 177, 176,
//   184, 185, 186, 187, 188, 189, 190, 191,
//   199, 198, 197, 196, 195, 194, 193, 192,
//   200, 201, 202, 203, 204, 205, 206, 207,
//   215, 214, 213, 212, 211, 210, 209, 208,
//   216, 217, 218, 219, 220, 221, 222, 223,
//   231, 230, 229, 228, 227, 226, 225, 224,
//   232, 233, 234, 235, 236, 237, 238, 239,
//   247, 246, 245, 244, 243, 242, 241, 240,
//   248, 249, 250, 251, 252, 253, 254, 255
//  };
//  const uint16_t XYTable[] = {
//    511,510,509,508,507,506,505,504,7,6,5,4,3,2,1,0,
//    496,497,498,499,500,501,502,503,8,9,10,11,12,13,14,15,
//    495,494,493,492,491,490,489,488,23,22,21,20,19,18,17,16,
//    480,481,482,483,484,485,486,487,24,25,26,27,28,29,30,31,
//    479,478,477,476,475,474,473,472,39,38,37,36,35,34,33,32,
//    464,465,466,467,468,469,470,471,40,41,42,43,44,45,46,47,
//    463,462,461,460,459,458,457,456,55,54,53,52,51,50,49,48,
//    448,449,450,451,452,453,454,455,56,57,58,59,60,61,62,63,
//    447,446,445,444,443,442,441,440,71,70,69,68,67,66,65,64,
//    432,433,434,435,436,437,438,439,72,73,74,75,76,77,78,79,
//    431,430,429,428,427,426,425,424,87,86,85,84,83,82,81,80,
//    416,417,418,419,420,421,422,423,88,89,90,91,92,93,94,95,
//    415,414,413,412,411,410,409,408,103,102,101,100,99,98,97,96,
//    400,401,402,403,404,405,406,407,104,105,106,107,108,109,110,111,
//    399,398,397,396,395,394,393,392,119,118,117,116,115,114,113,112,
//    384,385,386,387,388,389,390,391,120,121,122,123,124,125,126,127,
//    383,382,381,380,379,378,377,376,135,134,133,132,131,130,129,128,
//    368,369,370,371,372,373,374,375,136,137,138,139,140,141,142,143,
//    367,366,365,364,363,362,361,360,151,150,149,148,147,146,145,144,
//    352,353,354,355,356,357,358,359,152,153,154,155,156,157,158,159,
//    351,350,349,348,347,346,345,344,167,166,165,164,163,162,161,160,
//    336,337,338,339,340,341,342,343,168,169,170,171,172,173,174,175,
//    335,334,333,332,331,330,329,328,183,182,181,180,179,178,177,176,
//    320,321,322,323,324,325,326,327,184,185,186,187,188,189,190,191,
//    319,318,317,316,315,314,313,312,199,198,197,196,195,194,193,192,
//    304,305,306,307,308,309,310,311,200,201,202,203,204,205,206,207,
//    303,302,301,300,299,298,297,296,215,214,213,212,211,210,209,208,
//    288,289,290,291,292,293,294,295,216,217,218,219,220,221,222,223,
//    287,286,285,284,283,282,281,280,231,230,229,228,227,226,225,224,
//    272,273,274,275,276,277,278,279,232,233,234,235,236,237,238,239,
//    271,270,269,268,267,266,265,264,247,246,245,244,243,242,241,240,
//    256,257,258,259,260,261,262,263,248,249,250,251,252,253,254,255
//};
const uint16_t XYTable[] = {
  319,318,317,316,315,314,313,312,327,326,325,324,323,322,321,320,
  304,305,306,307,308,309,310,311,328,329,330,331,332,333,334,335,
  303,302,301,300,299,298,297,296,343,342,341,340,339,338,337,336,
  288,289,290,291,292,293,294,295,344,345,346,347,348,349,350,351,
  287,286,285,284,283,282,281,280,359,358,357,356,355,354,353,352,
  272,273,274,275,276,277,278,279,360,361,362,363,364,365,366,367,
  271,270,269,268,267,266,265,264,375,374,373,372,371,370,369,368,
  256,257,258,259,260,261,262,263,376,377,378,379,380,381,382,383,
  255,254,253,252,251,250,249,248,391,390,389,388,387,386,385,384,
  240,241,242,243,244,245,246,247,392,393,394,395,396,397,398,399,
  239,238,237,236,235,234,233,232,407,406,405,404,403,402,401,400,
  224,225,226,227,228,229,230,231,408,409,410,411,412,413,414,415,
  223,222,221,220,219,218,217,216,423,422,421,420,419,418,417,416,
  208,209,210,211,212,213,214,215,424,425,426,427,428,429,430,431,
  207,206,205,204,203,202,201,200,439,438,437,436,435,434,433,432,
  192,193,194,195,196,197,198,199,440,441,442,443,444,445,446,447,
  191,190,189,188,187,186,185,184,455,454,453,452,451,450,449,448,
  176,177,178,179,180,181,182,183,456,457,458,459,460,461,462,463,
  175,174,173,172,171,170,169,168,471,470,469,468,467,466,465,464,
  160,161,162,163,164,165,166,167,472,473,474,475,476,477,478,479,
  159,158,157,156,155,154,153,152,487,486,485,484,483,482,481,480,
  144,145,146,147,148,149,150,151,488,489,490,491,492,493,494,495,
  143,142,141,140,139,138,137,136,503,502,501,500,499,498,497,496,
  128,129,130,131,132,133,134,135,504,505,506,507,508,509,510,511,
  127,126,125,124,123,122,121,120,519,518,517,516,515,514,513,512,
  112,113,114,115,116,117,118,119,520,521,522,523,524,525,526,527,
  111,110,109,108,107,106,105,104,535,534,533,532,531,530,529,528,
  96,97,98,99,100,101,102,103,536,537,538,539,540,541,542,543,
  95,94,93,92,91,90,89,88,551,550,549,548,547,546,545,544,
  80,81,82,83,84,85,86,87,552,553,554,555,556,557,558,559,
  79,78,77,76,75,74,73,72,567,566,565,564,563,562,561,560,
  64,65,66,67,68,69,70,71,568,569,570,571,572,573,574,575,
  63,62,61,60,59,58,57,56,583,582,581,580,579,578,577,576,
  48,49,50,51,52,53,54,55,584,585,586,587,588,589,590,591,
  47,46,45,44,43,42,41,40,599,598,597,596,595,594,593,592,
  32,33,34,35,36,37,38,39,600,601,602,603,604,605,606,607,
  31,30,29,28,27,26,25,24,615,614,613,612,611,610,609,608,
  16,17,18,19,20,21,22,23,616,617,618,619,620,621,622,623,
  15,14,13,12,11,10,9,8,631,630,629,628,627,626,625,624,
  0,1,2,3,4,5,6,7,632,633,634,635,636,637,638,639
};

#define LED_PIN 16
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define LDR_PIN 36
#define MAZE_SERIAL_OUTPUT 0
#define MOTION_PIN 27

Preferences preferences;

int BRIGHTNESS = 8;
int BRIGHT_MODE = 0; // 0 = Manual, 1 = Auto Dim (with range adjust)
int SOLVESPEED = 100;
int SOLUTIONHOLD = 5000;
CRGB SOLUTIONCOLOR = CRGB::Green;
int SOLUTIONBRIGHTNESS_OFFSET = 0;
CRGB SOLVECOLOR = CRGB::Red;
int SOLVEBRIGHTNESS_OFFSET = 0;
CRGB WALLCOLOR = CRGB::White;
int WALLBRIGHTNESS_OFFSET =0;

//TaskHandle_t TaskHandle_generateMaze;
//TaskHandle_t TaskHandle_save;

hw_timer_t * timer = NULL;
bool doSave = false;

hw_timer_t * timerPause = NULL;
bool doPauseMaze = false;
bool doResumeMaze = false;
int pauseSeconds = 0;
int SECONDS_UNTIL_SLEEP = 300;

#define MEM_MAX    20000
 
// print character to use when printing to terminal
#define BLOCK_CHAR 'X'
 
#define BOTTOM  0x01
#define RIGHT 0x02
#define TRUE  1
#define FALSE 0
 
// convert block # to x, y coordinate
//#define GETX(item,width) (item % width)
//#define GETY(item,width) (item / width)
//#define COORD(item,width)(String("[") + String(GETX(item,width)) + "," + String(GETY(item,width)) + String("]"))
uint16_t GETX(uint16_t item, int width) {
  //Serial.print("GETX(");Serial.print(item);Serial.print(",");Serial.print(width);Serial.println();
  return item % width;
}
uint16_t GETY(uint16_t item, int width) {
  //Serial.print("GETY(");Serial.print(item);Serial.print(",");Serial.print(width);Serial.println();
  return item / width;
}
String COORD(uint16_t item, int width) {
  return String("[") + String(GETX(item,width)) + "," + String(GETY(item,width)) + String("]");
}

#define ROTARY_ENCODER_A_PIN 32
#define ROTARY_ENCODER_B_PIN 21
#define ROTARY_ENCODER_BUTTON_PIN 25
#define ROTARY_ENCODER_VCC_PIN -1 /* 27 put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder */

//depending on your encoder - try 1,2 or 4 to get expected behaviour
//#define ROTARY_ENCODER_STEPS 1
//#define ROTARY_ENCODER_STEPS 2
#define ROTARY_ENCODER_STEPS 4

//instead of changing here, rather change numbers above
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
 
/* global variables defined here */
char *maze=NULL;              // pointer to maze wall data
uint16_t *mazepath=NULL;      // pointer to maze path data
//uint16_t *mazesolution=NULL;  // pointer to maze solution
uint16_t *mazesolution=(uint16_t *)malloc(sizeof(uint16_t) * 1000);  // pointer to maze solution
uint16_t solutioncount = 0;
 
int sizex=8, sizey=20;     /* maze size */
//int sizex=4, sizey=4;     /* maze size */
int cellsize = 1; // larger the cellsize, easier the puzzle, in pixels
int lwidth = 1; // maze wall width, in pixels

// Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR detectsMovement() {
  Serial.println("MOTION DETECTED!!!");
  doResumeMaze = true;
}

void IRAM_ATTR onTimerPause() {
  pauseSeconds++;
  if(pauseSeconds*10 > SECONDS_UNTIL_SLEEP) {
    pauseSeconds = 0;
    doPauseMaze = true;
  }
  
}

void PauseMaze() {
  doPauseMaze = false;
  Serial.println("Pause Maze");
  timerAlarmDisable(timerPause);
  //vTaskSuspend(TaskHandle_generateMaze);
  //vTaskSuspend(TaskHandle_save);
  //screen off
  FastLED.setBrightness(0);
  FastLED.show();
}
void ResumeMaze() {
  doResumeMaze = false;
  Serial.println("Resume Maze");
  pauseSeconds = 0;
  timerWrite(timerPause, 0); // reset countdown
  timerAlarmEnable(timerPause); // enable (in case it was not)
  //screen on
  FastLED.setBrightness(BRIGHTNESS*17);
  FastLED.show();
}

void IRAM_ATTR onTimer() {
  doSave = true;
}
void StartSaveTimer() {
  timerWrite(timer, 0); // reset countdown
  timerAlarmEnable(timer); // enable (in case it was not)
}
void Save() {
  doSave=false;
  preferences.begin("maze", false);
  preferences.putInt("BRIGHTNESS", BRIGHTNESS);
  preferences.putInt("BRIGHT_MODE", BRIGHT_MODE);
  preferences.end();
  Serial.print("BRIGHTNESS saved:");Serial.print(BRIGHTNESS);Serial.println();
  Serial.print("BRIGHT_MODE saved:");Serial.print(BRIGHT_MODE);Serial.println();
  timerAlarmDisable(timer);
}

void rotary_onButtonClick()
{
  static unsigned long lastTimePressed = 0;
  //ignore multiple press in that time milliseconds
  if (millis() - lastTimePressed < 500)
  {
    return;
  }
  lastTimePressed = millis();
  Serial.print("button pressed at ");
  Serial.println(millis());

  if(BRIGHT_MODE==0) {
    BRIGHT_MODE = 1;
  } else {
    BRIGHT_MODE = 0;
  }

  StartSaveTimer();
}

void rotary_loop()
{
  //dont print anything unless value changed
  if (!rotaryEncoder.encoderChanged())
  {
    return;
  }

  BRIGHTNESS = rotaryEncoder.readEncoder();

  if(BRIGHT_MODE==0) {
    // manual
    int ledvalue = BRIGHTNESS*17;
    Serial.print("BRIGHTNESS: EncoderValue=");Serial.print(BRIGHTNESS);Serial.print(", LEDValue=");Serial.print(ledvalue);Serial.println();
  
    FastLED.setBrightness(ledvalue);
    FastLED.show();
    
  } else if(BRIGHT_MODE ==1) {
    // auto (with range)

    //do nothing special. BRIGHTNESS (top range) already adjusted and saved above.
    Serial.println("BRIGHTNESS: Auto");
  }

  StartSaveTimer();
}

/*
   init_maze(cs, lw) initializes maze memory.
   cs - cell size in pixels (smaller cell = harder maze)
   lw - line width in pixels
   example: init_maze(7,2) will use 7 pixel boxes with 2 pixel maze wall width
     and 3 pixel solution line width (cs - lw - 2)
   Maze size is based on cs value and ePaper screen size
*/
void init_maze(int cs, int lw)
{
  // use max size allowable
  cellsize = cs;
  lwidth = lw;
  Serial.println("maze size: " + String(sizex) + " x " + String(sizey));
  if(sizex < 1 || sizey < 1 || (long)sizex*sizey > MEM_MAX)
  {
    Serial.println("Invalid values entered for maze size\n");
    Serial.println("Maze must be at least 4 x 4\n");
    exit(EXIT_FAILURE);
  }
  long incr;
  for(incr=0; incr < (sizex/1) *  (sizey/1); incr++)
  {
    if(incr==0)
      maze[incr]=0;
    else if(incr < sizex)
      maze[incr]=BOTTOM;
    else if((incr%sizex)==0)
      maze[incr]=RIGHT;
    else
      maze[incr]=BOTTOM|RIGHT;
    mazepath[incr]=incr;
  }
}

int getPreviousDirection(uint16_t pos, uint16_t prevpos)
{
  int x1, y1, x2, y2, dir;
  x1 = GETX(pos,sizex);
  y1 = GETY(pos,sizex);  
  x2 = GETX(prevpos,sizex);
  y2 = GETY(prevpos,sizex);

  dir = -1; //self or error

  if(x1 > x2) {
    dir = 1; //prev is west
  }

  if(x1 < x2) {
    dir = 3; //prev is east
  }

  if(y1 > y2) {
    dir = 0; //prev is north
  }

  if(y1 < y2) {
    dir = 2; //prev is south
  }
  
  if(MAZE_SERIAL_OUTPUT == 1) {Serial.println( "getPreviousDirection: PREV " + COORD(prevpos,sizex) + " is " + String(dir) + " of POS " + COORD(pos,sizex));}
  return dir;
}

uint16_t coordToLedAddress(uint16_t posx, uint16_t posy, int dir) {
  uint16_t address = (posy*sizex*2*2)+(posx*2); // (row * how many boxes wide * double x-direction * double y-direction) + double x;
  uint16_t ledAddress;
  //dir
  //-1=this coord
  //0=North
  //1=west
  //2=south
  //3=east
  
  switch(dir) {
    case 0:
      address = address - (sizex*2);
      break;
    case 1:
      address = address - 1;
      break;
    case 2:
      address = address + (sizex*2);
      break;
    case 3:
      address = address + 1;
      break;
  }

  ledAddress = XYTable[address];
  
  if(MAZE_SERIAL_OUTPUT == 1) {Serial.print("  XYTable[");Serial.print(address);Serial.print("]=");Serial.print(ledAddress);Serial.println();}
  return ledAddress;
}

/*
 * solve_r() - recursive solve routine, called by solve()
 */
bool solve_r(uint16_t finish, uint16_t pos, uint16_t prevpos, uint8_t dir)
{
  if(MAZE_SERIAL_OUTPUT == 1) {Serial.print("solve_r(finish:");Serial.print(finish);Serial.print(",pos:");Serial.print(pos);Serial.print(",prevpos:");Serial.print(prevpos);Serial.print(",dir:");Serial.print(dir);Serial.println();}
  
  if(pos == prevpos)
  {
    //if(MAZE_SERIAL_OUTPUT == 1) {
    Serial.println("same square, backing up");
    //}
    solutioncount--;
    // same square, need to back up from here
    return false;
  }
  mazesolution[solutioncount++] = pos;
  if(pos == finish)
  {
    if(MAZE_SERIAL_OUTPUT == 1) {Serial.println("Solved in " + String(solutioncount) + " moves");}
    return true;
  }
  uint16_t posx, posy, newx, newy, newpos, prevx, prevy;  
  String sDir = "";
  /*
  * directions:
  * 0: north
  * 1: west
  * 2: south
  * 3: east
  */
  switch(dir)
  {
    case 0: // north
      sDir = "north";
      break;
    case 1: // west
      sDir = "west";
      break;
    case 2: // south
      sDir = "south";
      break;
    case 3: // east
      sDir = "east";
      break;
  }
  
  posy = GETY(pos, sizex);
  posx = GETX(pos, sizex);
  if(MAZE_SERIAL_OUTPUT == 1) {Serial.println("trying square " + COORD(pos, sizex) + " at direction " + sDir);}

  int ledAddress = coordToLedAddress(posx, posy, -1);
  
  int prevDir = getPreviousDirection(pos, prevpos);
  int ledAddress2 = coordToLedAddress(posx, posy, prevDir);
  vTaskDelay(SOLVESPEED);
  leds[ledAddress2] = SOLVECOLOR;
  leds[ledAddress2].fadeToBlackBy(BRIGHTNESS+SOLVEBRIGHTNESS_OFFSET);
  FastLED.show();
  
  vTaskDelay(SOLVESPEED);
  leds[ledAddress] = SOLVECOLOR;
  leds[ledAddress].fadeToBlackBy(BRIGHTNESS+SOLVEBRIGHTNESS_OFFSET);
  FastLED.show();
      
  switch(dir)
  {
    case 0: // north
      newx = posx;
      newy = posy - 1;
      newpos = newy * sizex + newx;

      if((maze[newpos] & BOTTOM) == 0)
      {
        return solve_r(finish, newpos, pos, 3);
      }
      break;
    case 1: // west
      newx = posx - 1;
      newy = posy;
      newpos = newy * sizex + newx;
      
      if((maze[newpos] & RIGHT) == 0)
      {
        return solve_r(finish, newpos, pos, 0);
      }
      break;
    case 2: // south
      newx = posx;
      newy = posy + 1;
      newpos = newy * sizex + newx;

      if((maze[pos] & BOTTOM) == 0)
      {
        return solve_r(finish, newpos, pos, 1);
      }
      break;
    case 3: // east 
      newx = posx + 1;
      newy = posy;
      newpos = newy * sizex + newx;
      
      if((maze[pos] & RIGHT) == 0)
      {
        return solve_r(finish, newpos, pos, 2);
      }
      break;
  }
  dir = (dir + 1) %4;
  if(MAZE_SERIAL_OUTPUT == 1) {Serial.println("next direction " + sDir);}
  solutioncount--;
  
  return solve_r(finish, pos, prevpos, dir);
}
 
/*
 * solve() - solve the maze
 */
void solve()
{
  uint16_t start = 0;
  uint16_t finish = 0;
 
  for(int i = 1; i < sizex; i++)
  {
    if((maze[i] & BOTTOM) == 0)
    {
      start = i + sizex;// start at row below
      break;
    }
  }
  for(int i = (sizey-1)*sizex + 1; i < (sizey*sizex); i++)
  {
    if((maze[i] & BOTTOM) == 0)
    {
      finish = i;
      break;
    }
  }
  solutioncount = 0;
  if(MAZE_SERIAL_OUTPUT == 1) {Serial.print("maze start: ");Serial.print(start);Serial.print(", finish: ");Serial.print(finish);Serial.println();}
  if(MAZE_SERIAL_OUTPUT == 1) {Serial.println("maze start: " + COORD(start, sizex) + ", finish: " + COORD(finish, sizex));}
  mazesolution[solutioncount++] = start;
  solve_r(finish, start, start - sizex, 1);
  
  // remove dead end moves
  int solutionpos = 0;
  while(solutionpos < solutioncount)
  {
    //Serial.println("solution pos: " + String(solutionpos) + COORD(mazesolution[solutionpos], sizex) 
    //  + ", solution count: " + String(solutioncount));
    for(int i = solutioncount - 1; i > solutionpos; i--)
    {
      if(mazesolution[solutionpos] == mazesolution[i])
      {
        // remove dead end paths
        //Serial.println("removing " + String(i - solutionpos) + " duplicate path items");
        for(int j = 0; j < (solutioncount - solutionpos); j++)
        {          
          mazesolution[solutionpos + j] = mazesolution[i + j];
        }
        solutioncount -= i - solutionpos;
      }
    }
    solutionpos++;
  }
  if(MAZE_SERIAL_OUTPUT == 1) {Serial.println("Solution reduced to " + String(solutioncount) + " moves");}
 
  // print out solution
  int prevpos = start - sizex;//artificially move up a row to create direction from north
  for(int i = 0; i < solutioncount; i++)
  {
    if(MAZE_SERIAL_OUTPUT == 1) {Serial.println(String(i) + ": " + COORD(mazesolution[i], sizex));}
    uint16_t posy, posx, prevposx, prevposy;
    posy = GETY(mazesolution[i], sizex);
    posx = GETX(mazesolution[i], sizex);
    int ledAddress = coordToLedAddress(posx, posy, -1);
    leds[ledAddress] = SOLUTIONCOLOR;
    leds[ledAddress].fadeToBlackBy(BRIGHTNESS+SOLUTIONBRIGHTNESS_OFFSET);
    
    int prevDir = getPreviousDirection(mazesolution[i], prevpos);
    //prevposx = GETX(prevpos, sizex);
    //prevposy = GETY(prevpos, sizex);
    //Serial.println("prevposx: " + String(prevposx) + ", prevposy: " + String(prevposy) + ", prevDir: " + String(prevDir));
    int ledAddress2 = coordToLedAddress(posx, posy, prevDir);
    leds[ledAddress2] = SOLUTIONCOLOR;
    leds[ledAddress2].fadeToBlackBy(BRIGHTNESS+SOLUTIONBRIGHTNESS_OFFSET);
        
    prevpos = mazesolution[i];
  }
  uint16_t posyfinish, posxfinish;
  posyfinish = GETY(finish, sizex);
  posxfinish = GETX(finish, sizex);
  int ledAddressFinish = coordToLedAddress(posxfinish, posyfinish+1, 0);
  leds[ledAddressFinish] = SOLUTIONCOLOR;
  leds[ledAddressFinish].fadeToBlackBy(BRIGHTNESS+SOLUTIONBRIGHTNESS_OFFSET);
  
  FastLED.show();
  vTaskDelay(SOLUTIONHOLD);

}
 
void print_block_maze()
{
  int incrx, incry, incr2;
  int disprow, dispaddress1, dispaddress2, displed1, displed2;
  char buff[sizex];
  FastLED.clear();
  FastLED.show();
  //each row
  for(incry=0; incry < sizey; incry++)
  {
    // left and right box
    for(incr2=0; incr2 < 2; incr2++)
    {
      //each column
      for(incrx=0; incrx < sizex; incrx++)
      {
        // left and right box
        switch(incr2)
        {
        case 0:
          //right box
          strcpy(buff,"00");
          if(incry > 0)                                           // NOT first row
          {
            if((*(maze+incry*sizex+incrx)&RIGHT)!=0)              //  IS Right Wall
              buff[1]=BLOCK_CHAR;
              //buff[1]='R';                                        //    ::Block Right
          }

          disprow=incry*2;
          dispaddress2=disprow*sizex*2+incrx*2+1;
          displed2=XYTable[dispaddress2];
          
          Serial.print(buff);
          if(buff[1]==BLOCK_CHAR) {
            leds[displed2] = WALLCOLOR;  
            leds[displed2].fadeToBlackBy(BRIGHTNESS+WALLBRIGHTNESS_OFFSET);
          }
          //Serial.print(", incry=");Serial.print(incry);Serial.print(", incrx=");Serial.print(incrx);
          //Serial.print(", disprow=");Serial.print(disprow);Serial.print(", dispaddress2="); Serial.print(dispaddress2);Serial.print(", displed2=");Serial.print(displed2);Serial.print(", value=");Serial.println(buff[1]);
                    
          break;
        case 1:
          //left box
          strcpy(buff,"00");
          
          if((*(maze+incry*sizex+incrx)&BOTTOM)!=0 && incrx > 0)  // BOTTOM Wall and NOT first column
            buff[0]=BLOCK_CHAR;
            //buff[0]='B';                                          // ::Block Left
          
          if
          (
            (
              (incry < (sizey-1))
              && ((*(maze+(incry+1)*sizex+incrx)&RIGHT)!=0)
            )                                                     // NOT last row and below is RIGHT Wall
            || (buff[0]==BLOCK_CHAR)                              // or left box is already block
            || (incrx==0)                                         // or IS first column
            || ((*(maze+incry*sizex+incrx)&RIGHT)!=0)             // or RIGHT WALL
            || (                                                  // or
                (incrx < (sizex-1))
                && ((*(maze+incry*sizex+incrx+1)&&BOTTOM)!=0)
               )                                                  // NOT last column and NEXT is BOTTOM wall 
          )
            buff[1]=BLOCK_CHAR;
            //buff[1]='X';                                          // ::Block Right

          disprow=incry*2+1;
          dispaddress1=disprow*sizex*2+incrx*2;
          displed1=XYTable[dispaddress1];
          dispaddress2=disprow*sizex*2+incrx*2+1;
          displed2=XYTable[dispaddress2];
          
          Serial.print(buff);
          if(buff[0]==BLOCK_CHAR) {
            leds[displed1] = WALLCOLOR;
            leds[displed1].fadeToBlackBy(BRIGHTNESS+WALLBRIGHTNESS_OFFSET);
          }
          //Serial.print(", incry=");Serial.print(incry+1);Serial.print(", incrx=");Serial.print(incrx);
          //Serial.print(", disprow=");Serial.print(disprow);Serial.print(", dispaddress1="); Serial.print(dispaddress1);Serial.print(", displed1=");Serial.print(displed1);Serial.print(", value=");Serial.println(buff[0]);
          if(buff[1]==BLOCK_CHAR) {
            leds[displed2] = WALLCOLOR;
            leds[displed2].fadeToBlackBy(BRIGHTNESS+WALLBRIGHTNESS_OFFSET);
          }
          //Serial.print(", incry=");Serial.print(incry+1);Serial.print(", incrx=");Serial.print(incrx);
          //Serial.print(", disprow=");Serial.print(disprow);Serial.print(", dispaddress2="); Serial.print(dispaddress2);Serial.print(", displed2=");Serial.print(displed2);Serial.print(", value=");Serial.println(buff[1]);
          
          break;
        } // right and left
      } //column
      Serial.println();
    } // right and left
  } //row
  FastLED.show();
}
 
/*
   cell_join() joins two cells together, effectively breaking down a wall within
   the maze.
*/
void cell_join(int cell1, int cell2)
{
  int incr,val;
  
  val=*(mazepath+cell2);
  /* set mazepath value */
  //for(incr=0; incr < sizex*sizey; incr++)
  for(incr = sizex*sizey-1; incr >= 0; incr--)
    if(*(mazepath+incr)==val)
      *(mazepath+incr)=*(mazepath+cell1);
  /* set graphics */
  if(cell1+1 == cell2) /* open right wall */
    *(maze+cell1)=*(maze+cell1)&~RIGHT;
  else /* open bottom wall */
    *(maze+cell1)=*(maze+cell1)&~BOTTOM;
}
 
/*
   connect() attempts to connect two squares together, returning
   FALSE if the attempt failed
*/
int connect(int cell)
{
  int incr;
  int cellcheck[2]; /* adjacent cell attempts */
  /* check if cell is a border, if so, return false */
  if((cell < sizex) /* top line */
  ||((cell%sizex)==0)) /* left line */
    return(FALSE);
  /* determine order of cell attempts */
  cellcheck[0]=random(2);
  cellcheck[1]=(cellcheck[0]+1)%2;
  /* check cells to see if can be connected */
  for(incr=0; incr < 2; incr++)
  {
    if((GETX(cell,sizex)==(sizex-1))&&(cellcheck[incr]==0))
      continue; // do not attempt to open right edge of maze
    //if((cell > (sizex*(sizey-1)))&&(cellcheck[incr]==1))
    if((GETY(cell,sizex)==(sizey-1))&&(cellcheck[incr]==1))
      continue; // do not attempt to open bottom edge of maze
    if(*(mazepath+cell)!=*(mazepath+cell+1+cellcheck[incr]*(sizex-1)))
    {
      cell_join(cell,cell+1+cellcheck[incr]*(sizex-1));
      return(TRUE);
    }
  }
  return(FALSE);
}
 
/*
   generate() is the function that generates a random maze.  It calls connect()
   (which, in turn, calls cell_join()) to generate the maze.
*/
void generateMaze_task(void * parameters)
{
  for(;;)
  {
    init_maze(sizex,sizey);
    
    int cell,checkcell,incr,complete;
    do
    {
      complete=TRUE;
      /* pick a random cell */
      cell=sizex + random(sizex*(sizey-1));
      /* find the next cell that can be connected */
      for(incr=0; incr < sizex*sizey; incr++)
      {
        checkcell=(incr+cell)%(sizex*sizey);
        if((checkcell < sizex)||((checkcell%sizex)==0))
          continue;
        if(connect(checkcell))
        {
          complete=FALSE;
          break;
        }
      }
    }
    while(!complete);
    
    /* break walls for start and end of maze, near center */
    cell=sizex/4+(long)random(sizex/2);
    *(maze+cell)=*(maze+cell)&~BOTTOM;
    cell=sizex/4+(long)random(sizex/2)+sizex*(sizey-1);
    *(maze+cell)=*(maze+cell)&~BOTTOM;
  
    print_block_maze();
    
    solve();
  }

  // never called, but good practice
  vTaskDelete(NULL);
}

void readLight_task(void * parameters) {

  for(;;) {

    //sunny room, lights on = 720
    //sunny room, lights off = 1830
    //finger = 2400
    //black = 4095

    if(BRIGHT_MODE==1) {
      int ldrValue = analogRead(LDR_PIN);
      Serial.print("ldr: ");Serial.print(ldrValue);Serial.println();
      //map(value, fromLow, fromHigh, toLow, toHigh)
  
      int brightness = map(ldrValue,4096,0,1,BRIGHTNESS) / 1; //convert to int
      int ledvalue = brightness*17;
      Serial.print("BRIGHTNESS: LightValue=");Serial.print(ldrValue);Serial.print(", brightnessLevel=");Serial.print(brightness);Serial.print(", LEDValue=");Serial.print(ledvalue);Serial.println();
    
      FastLED.setBrightness(ledvalue);
      FastLED.show();  
    }
        
    vTaskDelay(1000);
  }
  vTaskDelete(NULL);
}

/*
 * error() display error message and blink red neopixel
 */
void error(const char *err)
{
  Serial.println(err);
  while(1)
  {
    FastLED.clear();
    FastLED.show();
    leds[0] = CRGB::Red; 
    FastLED.show();
    delay(400);

    leds[0] = CRGB::Black; 
    FastLED.show();
    delay(100);      
  }
}

void startupLEDsTest() {
  // startup blink test to confirm LEDs are working.
  FastLED.setBrightness(32);
  fill_solid(leds, NUM_LEDS, CRGB(255,0,0));  // fill red
  FastLED.show();
  delay(500);
  fill_solid(leds, NUM_LEDS, CRGB(0,255,0));  // fill green
  FastLED.show();
  delay(500);
  fill_solid(leds, NUM_LEDS, CRGB(0,0,255));  // fill blue
  FastLED.show();
  delay(500);
  FastLED.clear();
  FastLED.show();
  
  for(int i = 0;i < NUM_LEDS - 1; i++)
  {
    leds[i] = CRGB::White;
    FastLED.show();
    
  }
  FastLED.clear();
  FastLED.show();
  
  FastLED.setBrightness(BRIGHTNESS);
} //end_startupLEDsTest

void setup() {
  Serial.begin(115200);
  //while(!Serial);
  delay(1000);
  Serial.println(
    "XXXXXXX XXXXXXXXXXXXXXXXXXXXX\n"
    "X  Maze Generation Program  X\n"
    "X  Written by Dan Cogliano  X\n"
    "X  For Adafruit Industries  X\n"
    "XXXXXXXXXXXXXXXXXXXXX XXXXXXX\n\n");

  randomSeed(analogRead(0));

  pinMode(LED_PIN,OUTPUT);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS*17);
  FastLED.clear();
  FastLED.show();
  Serial.println("FastLED initialized.");
//  startupLEDsTest();
//  Serial.println("FastLED tested.");
 
  int w = sizex;//gfx.width();
  int h = sizey;//gfx.height();
  // assuming cell size of 1 pixels is the smallest supported maze
  if((maze=(char *)malloc((w/1) *  (h/1) * sizeof(char)))==NULL)
  {
    error("Not enough memory available for maze\n");
  }
  if((mazepath=(uint16_t *)malloc((w/1) *  (h)/1 * sizeof(uint16_t)))==NULL)
  {
    error("Not enough memory available for maze\n");
  }
  if((mazesolution=(uint16_t *)malloc((w/1) *  (h)/1 * sizeof(uint16_t)))==NULL)
  {
    error("Not enough memory available for maze\n");
  }

  // Configure the Prescaler at 80 the quarter of the ESP32 is cadence at 80Mhz
  // 80000000 / 80 = 1000000 tics / seconde
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 3000000, true);

  // Configure the Prescaler at 80 the quarter of the ESP32 is cadence at 80Mhz
  // 80000000 / 80 = 1000000 tics / seconde
  timerPause = timerBegin(0, 80, true);  //tick every 1 second
  timerAttachInterrupt(timerPause, &onTimerPause, true);
  timerAlarmWrite(timerPause, 10000000, true); //10 seconds for test
  timerAlarmEnable(timerPause);

  // Open Preferences with my-app namespace. Each application module, library, etc
  // has to use a namespace name to prevent key name collisions. We will open storage in
  // RW-mode (second parameter has to be false).
  // Note: Namespace name is limited to 15 chars.
  preferences.begin("maze", false);
  BRIGHTNESS = preferences.getInt("BRIGHTNESS", 8);
  if(BRIGHTNESS>15) {
    BRIGHTNESS=8;
    preferences.putInt("BRIGHTNESS", BRIGHTNESS);
  }

  BRIGHT_MODE = preferences.getInt("BRIGHT_MODE", 0);
  if(BRIGHT_MODE!=0 || BRIGHT_MODE!=1) {
    BRIGHT_MODE=0;
    preferences.putInt("BRIGHT_MODE", BRIGHT_MODE);
  }
  preferences.end();
  
  //we must initialize rotary encoder
  rotaryEncoder.begin();
  rotaryEncoder.setup(
    [] { rotaryEncoder.readEncoder_ISR(); },
    [] { rotary_onButtonClick(); });
  //set boundaries and if values should cycle or not
  //in this example we will set possible values between 0 and 1000;
  bool circleValues = false;
  rotaryEncoder.setBoundaries(0, 15, circleValues); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  /*Rotary acceleration introduced 25.2.2021.
   * in case range to select is huge, for example - select a value between 0 and 1000 and we want 785
   * without accelerateion you need long time to get to that number
   * Using acceleration, faster you turn, faster will the value raise.
   * For fine tuning slow down.
   */
  rotaryEncoder.disableAcceleration(); //acceleration is now enabled by default - disable if you dont need it
  //rotaryEncoder.setAcceleration(250); //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration
  rotaryEncoder.setEncoderValue(BRIGHTNESS);

  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(MOTION_PIN, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(MOTION_PIN), detectsMovement, RISING);
  
  xTaskCreate(
    generateMaze_task,  // Function that should be called
    "generateMaze task",   // Name of the task (for debugging)
    50000,       // Stack size (bytes)
    NULL,       // Parameter to pass
    1,          // Task priority
    NULL// Task handle
  );
  
  xTaskCreate(
    readLight_task,  // Function that should be called
    "read light task",   // Name of the task (for debugging)
    1000,       // Stack size (bytes)
    NULL,       // Parameter to pass
    1,          // Task priority
    NULL// Task handle
  );
}
 
void loop() {
  rotary_loop();

  if(doSave)
    Save();

  if(doPauseMaze)
    PauseMaze();

   if(doResumeMaze)
    ResumeMaze();
}

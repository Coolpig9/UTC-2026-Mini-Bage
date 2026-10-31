#include <EEPROM.h>
/*
This file handles blink mode and all its funcs
pattern list:
  0. blink all
  1. OnOff
  2. Snake
  3. Reverse Snake
  4. Snake -> Reverse Snake
  5. Center 
  6. Reverse Center
  7. OnOff -> Reverse Snake

*/

//pattern definitions
//-1==on -2==off -3==delay //-4=blinkOn, -5 == blinkOff
#define ON -1
#define OFF -2
#define SLE -3
#define BON -4
#define BOFF -5

const int8_t allBlink[11] = {11,ON,0,1,2,SLE,OFF,0,1,2,SLE};
const int8_t snake[5] = {5,BON,0,1,2};
const int8_t revSnake[5] = {5,BON,2,1,0};
const int8_t onOff[16] = {16,ON,0,SLE,1,SLE,2,SLE,OFF,0,SLE,1,SLE,2,SLE}; 
const int8_t center[13] = {13,ON,0,2,SLE,1,SLE,OFF,0,2,SLE,1,SLE};
const int8_t revCenter[13] = {13,ON,1,SLE,0,2,OFF,SLE,1,SLE,0,2,SLE};
int8_t selectedPattern = EEPROM.read(patternPost);

int delayStage = EEPROM.read(delayStagePost);
//Menus
void blinkLoop(){
  int8_t button = -1;
  
  while(button != 1){
    playPattern(selectedPattern,(delayStage%7+1)*100);
    button = lisenForUser(true);
    delay(100);
    switch(button){
      case 0:
        //delaySelector();
        delayStage++;
        EEPROM.update(delayStagePost, delayStage);
        break;
      case 2:
        patternSelector();
        break;
    }

  }
  
}
//delayValue
int scanInputFor(int ms){
  for(int i=0;i<ms;i++){
    int button = lisenForUser(true);
    if(!button== -1){
      return button;
    }
    delay(1);
  }
  return -1;
}
/*
void delaySelector(){
  int button = -1;
  while(true){
    setOnOff(true,1);
    button = scanInputFor(delayValue);
    setOnOff(false,1);
    if(button == -1){
      button = scanInputFor(delayValue);
    }
    switch(button){
      case 0:
        delayValue -= 50;
        break;
      case 1:
        return;
      case 2:
        delayValue += 50;
        break;
      case -1:
        debugBlink(0,1000);
        break;
    }

  }
}
*/
void patternSelector(){
  int button = -1;
  while(button != 1){
    playPattern(selectedPattern,250);
    button = lisenForUser(false);
    switch(button){
      case 0:
        selectedPattern--;
        break;
      case 2:
        selectedPattern++;
        break;
    }
    delay(200);
    
  }
  EEPROM.update(patternPost, selectedPattern);
}

//patterns
int patternDelay = 500;
void playPattern(uint8_t patID,int ms){
  patID = patID%8;
  patternDelay = ms;
  switch(patID){
    case 0:
      patternPlayer(allBlink);
      break;
    case 1:
      patternPlayer(onOff);
      
      break;
    case 2:
      patternPlayer(snake);
      
      break;
    case 3:
      patternPlayer(revSnake);
      break;
    case 4:
      patternPlayer(snake);
      delay(ms);
      patternPlayer(revSnake);
      break;
    case 5:
      patternPlayer(center);
      break;
    case 6:
      patternPlayer(revCenter);
      break;
    case 7:
      patternPlayer(onOff);
      patternPlayer(revSnake);
      break;
  }
}
//plays pattern Arrays!
void patternPlayer(int8_t pattern[]){
  //-1==on -2==off -3==sleep -4=blinkOn, -5 == blinkOff
  bool state = true;
  bool doBlink = false;
  for(int i=1; i<pattern[0]; i++){
    
    int8_t p = pattern[i];
    switch(p){
      case -1:
        state = true;
        continue;
      case -2:
        state = false;
        continue;
      case -3:
        delay(patternDelay);
        continue;
      case -4:
        doBlink = true;
        continue;
      case -5:
        doBlink = false;
        continue;
    }
    if(doBlink){
      setOnOff(true, p);
      delay(patternDelay);
      setOnOff(false, p); 
    } else {
      setOnOff(state, p);
    }
  }
}


#include <EEPROM.h>
#include <avr/io.h>
//pin defintions

const int8_t  ledPins[3] = {0,1,2};
const int8_t  dbPins[2] = {4,5};
const int8_t  buzzerPin = 3;

//eprom locations
#define userNameLoc  0x0010
#define valeStartLoc 0x0020
#define contactNumLoc 0x0040

#define highScorePost (0x00 + valeStartLoc)
#define hiAttemptsPost (0x01 + valeStartLoc)
#define hiSeedPost (0x02 + valeStartLoc)

#define lastScorePost (0x04 + valeStartLoc)
#define laAttemptsPost (0x05 + valeStartLoc)
#define laSeedPost (0x06 + valeStartLoc)

#define patternPost (0x08 + valeStartLoc)
#define delayStagePost (0x09 + valeStartLoc)




char userName[16] = "UDOT TEMP"; //name max chars is 15
//Game funcs
void setup() {
  CLKPR = (1 << CLKPCE);
  CLKPR = (1 << CLKPS2);
  epromSetup();

  pinMode(ledPins[0], INPUT_PULLUP);
  pinMode(ledPins[1], INPUT_PULLUP);
  if(digitalRead(ledPins[0]) == LOW && digitalRead(ledPins[2]) == LOW){
    playPattern(0,200);
  }
  
  pinMode(dbPins[0], OUTPUT);
  pinMode(dbPins[1], OUTPUT);
  playPattern(0,250);
}

//Main
void loop() {
  int button = lisenForUser(false);
  delay(300);
  switch(button){
    case 0:
      makeMap();
      playPattern(0,200);
      gameLoop();
      break;
    case 1:
      blinkScore((int)EEPROM.read(highScorePost));
      break;
    case 2:
      blinkLoop();
      break;
  }
}

//basic funcs
int inputDelay(int ms){ // input inturps delay
  unsigned long start = millis();
  for(int i=0;i<ms;i++){
    delay(1);
    int button = lisenForUser(true);
    pinMode(ledPins[1], OUTPUT);
    if(button != -1){
      return button;
    }
  }
  return -1;
}
int lisenForUser(bool passive){
  int cPin = -1;
  while(true){
    cPin = (cPin+1)%3;
    pinMode(ledPins[cPin], INPUT_PULLUP);
    if(digitalRead(ledPins[cPin]) == LOW){
      delay(100);

      digitalWrite(dbPins[0],HIGH);
      while(digitalRead(ledPins[cPin]) == LOW){}
      digitalWrite(dbPins[0],LOW);
      return cPin;
    }
    if (passive && cPin == 2) return -1;
  }
  return cPin;
}

void setOnOff(bool onOff, int pin){
  if(onOff){
    pinMode(ledPins[pin], OUTPUT);
    digitalWrite(ledPins[pin], LOW);
  }else{
    pinMode(ledPins[pin], INPUT_PULLUP);
  } 
}

void blink(int pin,int ms,int times){
  for(int i=0;i<times;i++){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(ms/2);
    pinMode(pin, INPUT_PULLUP);
    delay(ms/2);
  }
}

void debugBlink(int dbPin,int ms){
  digitalWrite(dbPins[dbPin], HIGH);
  delay(ms/2);
  digitalWrite(dbPins[dbPin], LOW);
  delay(ms/2);
}

void epromSetup(){
  if((int)EEPROM.read(0) != 0x4B){
    const char sig[5] = "KIPS";
    EEPROM.put(0, sig);
  }
  
  if((int)EEPROM.read(0x00F0) != 0x55){
    const char udotSig[16] = "UDOT CONF 2026";
    EEPROM.put(0x00F0, udotSig);
  }
  //defult data
  char userNameBuffer[16];
  EEPROM.get(userNameLoc, userNameBuffer);
  if(userNameBuffer != userName){
    EEPROM.put(userNameLoc, userName);
  }
  if((int)EEPROM.read(highScorePost) == 0xFF){
    EEPROM.update(highScorePost, 0);
  }
  if((int)EEPROM.read(laAttemptsPost) == 0xFF){
    EEPROM.update(laAttemptsPost, 0);
  }
  if((int)EEPROM.read(patternPost) == 0xFF){
    EEPROM.update(patternPost, 0);
  }
  if((int)EEPROM.read(delayStagePost) == 0xFF){
    EEPROM.update(delayStagePost, 4);
  }

}


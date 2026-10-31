//This file handles all the simon code and all its funcs
#include <EEPROM.h>

const uint8_t compMovesLen = 200;
int8_t  compMoves[compMovesLen] = {};
int move = 1;
int highScore = 0;
int seed = 0;
int attempts = 0;
void gameLoop(){
  bool gameState = true;
  highScore = (int)EEPROM.read(highScorePost);
  attempts = (int)EEPROM.read(laAttemptsPost);
  while(gameState){
    blinkTo(move);
    //check user input agansit
    for(int i=0;i<(move);i++){
      int8_t userIn = lisenForUser(false);
      delay(100);
      //blink(ledPins[compMoves[i%100]],100,1);
      if(userIn != compMoves[i%compMovesLen]){
        gameState = gameOver();
        break;
      }
    }
    delay(300);
    move++;
  }
  return;
}
bool gameOver(){
  
  playPattern(0,100);
  playPattern(0,100);
  playPattern(0,100);
  delay(500);
  blinkScore(move);
  
  //update Eprom Mem
  attempts++;

  if(move>highScore){
    highScore = move;
    EEPROM.update(highScorePost,move);
    EEPROM.update(hiAttemptsPost,attempts);
    EEPROM.update(hiSeedPost,seed);
  }
  
  EEPROM.update(lastScorePost,move);
  EEPROM.update(laAttemptsPost,attempts);
  EEPROM.update(laSeedPost,seed);

  bool continueLoop = true;
  while(true){
    int button = lisenForUser(false);
    delay(300);
    if (button == 2){
      blinkScore(move);
    }else if(button == 0){
      continueLoop = true;
      break;
    } else if(button == 1){
      continueLoop = false;
      break;
    }
  }

  makeMap();
  move = 0;
  playPattern(0,100);
  delay(100);
  return continueLoop;
}

void makeMap(){
  seed = millis();
  randomSeed(seed);
  for(int i=0;i<compMovesLen;i++) compMoves[i] = random(0,3);
}

//input funcs


//outputs funcs
void blinkScore(int score){
  delay(200);
  blink(ledPins[0],250,(int(score/100) % 10));
  blink(ledPins[1],250,(int(score/10) % 10));
  blink(ledPins[2],250,(score % 10));
}
//blinks to number in moves
int blinkTo(int to){// returns wrap count
  for(int i=0;i<to;i++){
    if((i%100) == 0) digitalWrite(dbPins[1],HIGH);
    blink(ledPins[compMoves[i%100]],125,1);
    digitalWrite(dbPins[1],LOW);
  }
  return (int(to/100)*100); 
}
//This never got finsihed and was scraped

uint32_t defultTimer = 10000;
void passLoop(){
  passMenu();
  uint32_t timer = defultTimer;
  while(timer > 0){
    int ran = random(0,3);
    setOnOff(true,ran);
    delay(100);
    while(timer > 0){
      timer--;
      int button = lisenForUser(true);
      if(button == ran){
        break;
      }else if(button != -1){
        playPattern(0,500);
        playPattern(0,500);
        playPattern(0,500);
        timer -= 1500;
        break;
      }
      delay(1);
    }
    while(timer > 0){
      timer--;
      if(lisenForUser(true) != -1){
        playPattern(0,500);
        break;
      }
      delay(1);
    }
  }
  for(int i=0;i<10;i++){
    playPattern(0,100);
  }
  
}

int passMenu(){
  bool setupEnd = false;
  while(!setupEnd){
    switch(lisenForUser(true)){
      case 0:
        blink(ledPins[1],300,1);
        defultTimer -= 1000;
        break;
      case 1:
        playPattern(0,500);
        setupEnd = true;
        break;
      case 2:
        blink(ledPins[1],300,1);
        defultTimer += 1000;
        break;
    }
  }
}
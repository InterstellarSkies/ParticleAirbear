
#include "Airbear2.h"


void airbear::hatch(){
  Serial.begin(115200);
  memoryStatus();
  delay(5000);

  while(!vs1053.begin()){

  }

  SD.begin(CARDCS);    // initialise the SD card
  vs1053.setVolume(5,5);  // Set volume for left, right channels. lower numbers == louder volume!
  vs1053.useInterrupt(DREQ);  // DREQ int
  vs1053.playFullFile("Sounds/orbstart.mp3");



  neopixelbegin();
  setBrightness(MatrixBrightness);

  pinMode(heartpin,INPUT);
  pinMode(circlepin,INPUT);
  pinMode(crosspin,INPUT);
  pinMode(squarepin,INPUT);
  connection();

  tail4 = PositionRandomizer(random(7));
  tail3 = PositionRandomizer(tail4);
  tail2 = PositionRandomizer(tail3);
  tail1 = PositionRandomizer(tail2);
  head = PositionRandomizer(tail1);
  memoryStatus();
}

int airbear::NewMod(int dividend, int divisor){//modulo function that works with negative values (-5%7 = 2)
   int ret = dividend % divisor;
   if(ret < 0){
     ret+=divisor;
   }
   return ret;
}

void airbear::listen(){
  connection();
  heart();
  cross();
  circle();
  cancelbutton();
  spirit(10,10,10);
  loop();
}

void airbear::cross(){
  if(digitalRead(crosspin)==1){
    vs1053.playFullFile("Sounds/click.mp3");
    Serial.println("working4");
    delay(1000);
    refresh();
  }
}//end function

void airbear::circle(){
  if(digitalRead(circlepin)==1){
  Serial.println("working3");
  vs1053.playFullFile("Sounds/close.mp3");
  char* req = (char*)malloc(strlen("cloud")+8);// TODO change to 'new' function (memory alloc)
  strcpy(req,"sd?*cloud,4");
  Serial.println(req);
  latch(req); // Ask server for the file data
  MqttBlock(7000);//TODO add a mqtt block statement
  free(req);
  delay(1000);
  refresh();
}
}

void airbear::cancelbutton(){
  if(digitalRead(squarepin)==1){
    vs1053.playFullFile("Sounds/click.mp3");
    Serial.println("working2");
    SDbytes("pdec/fire1");
    delay(500);
    refresh();
  }


}
/*
bool airbear::Scrolling(int *ScrollingIndex){
  if(digitalRead(crosspin)){
    *ScrollingIndex = NewMod(*ScrollingIndex-1,10);
    vs1053.playFullFile("Sounds/click.mp3");

  }//endif
  if(digitalRead(circlepin)){
    *ScrollingIndex  = (*ScrollingIndex+1)%10;
    vs1053.playFullFile("Sounds/click.mp3");
  }//endif
  if(digitalRead(squarepin)){
    Serial.println("breaking");
    refresh();
    vs1053.playFullFile("Sounds/click.mp3");
    memoryStatus();
    return false;
  }//endif
  return true;
}// end function
*/
void airbear::inbox(){
  memoryStatus();
  char **msgarray = new char * [10];
  RetreiveMessages(msgarray);

  // rember to wrtie cleanup for the dynamic messages
  memoryStatus();


  //begin scrolling through last 10 messages
  int scroller=0;
  while (true){
    fetchNsketch(msgarray[scroller]);
    if(digitalRead(crosspin)){
      scroller = NewMod(scroller-1,10);
      vs1053.playFullFile("Sounds/click.mp3");

    }//endif
    if(digitalRead(circlepin)){
      scroller  = (scroller+1)%10;
      vs1053.playFullFile("Sounds/click.mp3");
    }//endif
    if(digitalRead(squarepin)){
      Serial.println("breaking");
      refresh();
      vs1053.playFullFile("Sounds/click.mp3");
      memoryStatus();
      return ;
    }//endif

  }
  RetreivalCleanup(msgarray); // clean up all heap data created during RetreiveMessages()
  memoryStatus();
}//end inbox function

void airbear::heart(){
  if(digitalRead(heartpin)==1){
    vs1053.playFullFile("Sounds/confirm.mp3");
    latch("hrtp");
    MqttBlock(5000);//Force mqtt to wait until received message TODO add timeout
    inbox();
  }//end if
}//endfunction
uint8_t airbear::PositionRandomizer(uint8_t n){
  //The light trail sometimes chooses to revert to previous position, add memory of previous movement
  n %= 64;
  uint8_t x = n%8 ;
  uint8_t y = ((n-x)/8);
   switch(random(7)){
     case(0):
       x = NewMod(x-1,8);
       y = NewMod(y-1,8);
       break;
     case(1):
       x = NewMod(x,8);
       y = NewMod(y-1,8);
       break;
     case(2):
       x = NewMod(x+1,8);
       y = NewMod(y-1,8);
       break;
     case(3):
       x = NewMod(x+1,8);
       y = NewMod(y,8);
       break;
     case(4):
       x = NewMod(x+1,8);
       y = NewMod(y+1,8);
       break;
     case(5):
       x = NewMod(x,8);
       y = NewMod(y+1,8);
       break;
     case(6):
       x = NewMod(x-1,8);
       y = NewMod(y+1,8);
       break;
     case(7):
       x = NewMod(x-1,8);
       y = NewMod(y,8);
      break;
     }//end the switch statement
     return x+(y*8);
}//end function
void airbear::spirit(uint8_t r,uint8_t g,uint8_t b){

  //TODO take in struct parameter for properties?, add notifications

  refresh();
  tail4 = tail3;
  tail3 = tail2;
  tail2 = tail1;
  tail1 = head;
  head = PositionRandomizer(head);
  setNPixel(tail4,r,g,b);
  setNPixel(tail3,r,g,b);
  setNPixel(tail2,r,g,b);
  setNPixel(tail1,r,g,b);
  setNPixel(head,0,10,10);
  delay(100);

}

void airbear::memoryStatus(){
  uint32_t freemem = System.freeMemory();
  Serial.print("free memory: ");
  Serial.println(freemem);
}

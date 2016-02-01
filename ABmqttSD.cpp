#include "ABmqttSD.h"


File myFile;
bool MqttCalled = false; // TODO change to bitflag
void callback(char* topic, byte* payload, unsigned int length){

  //debug 
  for(int w = 0 ; w<length ; w++){
    Serial.print((char)payload[w]);
  }


  MqttCalled = true;

  if((int)payload[0]==37 && (int)payload[1]==124){//this identifier sequence initiates list of sms checki ascii
    myFile = SD.open("test",FILE_WRITE);
    if(!myFile){ Serial.println("wFile fail"); return; }
    myFile.seek(0);//overwrite the entire file.
    myFile.write(payload,length);
    myFile.print("\0");
    myFile.close();
    Serial.println("write done");
  }//end if


  if((int)payload[0]==33 && (int)payload[1]==38 && (int)payload[2]==36 ){
    //convert byte array into cstring
    char payloadString[length];
    for(int i = 0;i < length ; i++){ //pretty costly especially if the payload is really large.
      payloadString[i] = (char)payload[i];
    }
    int sdwriteRGB[64];
    strtok(payloadString,"$");
    char* filename = strtok(NULL,",");
    char *number = strtok(NULL,"@");

    for(int i = 0 ; i < 64 ; i++){
      int r  = atoi(strtok(NULL,","));
      int g  = atoi(strtok(NULL,","));
      int b  = atoi(strtok(NULL,","));
      sdwriteRGB[i] = ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
      Serial.println(r);
      Serial.print(g);
      Serial.print(b);
    }


    /*

    int indexRGB=0;
    char sdfilename[30]; // buffer to hold filename
    memset(sdfilename, 0, sizeof sdfilename);//zero out the buffer, without it, random
    int DataStartindex=0;
    //maybe try strtok?

    for(int i = 0 ; i < 30 ; i++){// forloop searches for start of RGB data (the #30 is the max length of filename)
      if((int)payload[i+3]==64 && (int)payload[i+4]==47){//looking for @ (int 64) and / (int 47)
        DataStartindex = i+5;
        break;
      }
      sdfilename[i] = (char)payload[i+3];// copying filename into sdfilename cstring
    }// endforloop
    uint8_t number=0;
    uint8_t multiplier=0;
    for(int i = DataStartindex ; i<length ; i++ ){
      Particle.process();
      signed int digit = (int)payload[i];
      if(digit==47){
        sdwriteRGB[indexRGB] = number;
        indexRGB++;
        number=0;
        multiplier=0;
      }

      else{
      number = (number*multiplier)+(digit-48);
      multiplier =10;
      }
    }//end forloop-i

    //myFile = SD.open(filename,FILE_WRITE);


    for(int i = 0 ; i < 64*3 ; i++){
      Serial.println(sdwriteRGB[i]);
    }//endforloop
    */

  }//end if
}//end function

MQTT client("m10.cloudmqtt.com",16233,callback);

void BearMqttSD::latch(char* mqttmsg){//gotta figure out var type that can be publish
  connection();
  client.publish("protobear/sig",mqttmsg);
  client.loop();
} // simple function

bool BearMqttSD::connection() {
  if (client.isConnected()) {
    client.subscribe("protobear/com");
    return (true);
  }//endif
  else {
    if(!client.isConnected()){
      for(int i =0;i<30;i++){
      client.connect("arduinoClient","fnhnuaqc","uHKb4wF1tRKe");
      if(client.isConnected()){return true;}
      }
      return false;
    }//endforloop
  }//endelse
}//end func



bool BearMqttSD::loop(){
  client.loop();
  Particle.process();
  //Code below checks if message was received
  if(MqttCalled==true){
    MqttCalled = false;
    return true;
  }
  else{
    return false;
  }
}

bool BearMqttSD::MqttBlock(int timeDelay){
  int In = millis();
  int Out = millis();
  while(!loop()){
    Out = millis();
    if(Out-In > timeDelay){Serial.println("break mqttloop");return false;}
  }
  return true;
}



void BearMqttSD::SDbytes(char path[]){

  myFile = SD.open(path,FILE_READ);
  if(!myFile){
    Serial.println("not found - requesting");
    char* req = (char*)malloc(strlen(path)+6);// TODO change to 'new' function (memory alloc)
    strcpy(req,"sd?*");
    strcpy(req,path);
    //latch(req); // Ask server for the file data
    //TODO add a mqtt block statement
    free(req);
    return;
  }
  int array[64];
  uint8_t sdbytes[3];
  for(int i=0 ; i <64 ; i++){
    myFile.read(sdbytes,3);
    array[i] = Color(sdbytes[0],sdbytes[1],sdbytes[2]);
  }//end forloop

  uint8_t bufferindex=0;
  for(int y = 0 ; y < 8 ; y++){
    for (int x = 0 ; x < 8; x++) {
      drawXYPixel(x,y,array[bufferindex]);
      bufferindex++;
    }
  }
  show();

  myFile.close(); // close sd file


}// end function

bool BearMqttSD::fetchNsketch(char* pixelart,uint8_t frames){
  for(int i = 1 ; i <= frames ; i++){
  char* w = ("/pdec/");
  char* path =(char*)malloc(strlen(path)+strlen(pixelart)+3+1);
  char framenumber[2];
  strcpy(path, w);
  strcat(path, pixelart);
  //if(0<=i<10){
    //strcat(path,"0");
  //}
  strcat(path,itoa(i,framenumber,10));
  SDbytes(path);
  free(path);
  int start = millis();
  int end = 0;
  }
  }
bool BearMqttSD::fetchNsketch(char *pixelart){
  Serial.println(pixelart);
  char temp[strlen(pixelart)];
  for(int i = 0 ; i < strlen(pixelart) ; i++){
    temp[i] = pixelart[i];
  }
  char* filename = strtok(temp,",");
  char *framesString = strtok(NULL,",");
  int framesInteger = atoi(framesString);
  Serial.print("file:");
  Serial.println(filename);
  Serial.print("frame:");
  Serial.println(framesInteger);
  fetchNsketch(filename,framesInteger);


}//end function


void BearMqttSD::RetreiveMessages(char *msgarray[]){

      Serial.println("retreive msg");

      myFile = SD.open("TEST",FILE_READ);
      if(myFile){
        // array to hold cstring values which are parameters for SDbytes()
        char buffer[400]; //
        uint16_t index=0;
        memset(buffer, 0, sizeof buffer);//zero out the buffer, without it, random
        myFile.seek(SMSLISTSTART);

        while(myFile.available()){
          buffer[index] = (char)myFile.read();
          index++;
        }//end while loop

        Serial.println(buffer);
        char * temp = strtok(buffer,"/");
        Serial.println(temp);
        char * message = new char[strlen(temp)];
        strcpy(message,temp);

        msgarray[0] = message;
        uint8_t counter = 1;

        while(temp!=NULL){
          temp = strtok(NULL,"/");
          Serial.println(temp);
          char * tokenmessage = new char[strlen(temp)];
          strcpy(tokenmessage,temp);
          msgarray[counter] = tokenmessage;
          counter++;
        }//end while

        counter--;//BAD! temporary fix
        do{ // if the msgarray is not filled up, add filler
          msgarray[counter] = "cloud,11";
          counter++;
        }while(counter<10);

        myFile.close();
        Serial.println("retreive done");

    }
    else{
      Serial.println("failed");
    }
}

void BearMqttSD::RetreivalCleanup(char *msgarray[]){
  for(int i = 0 ; i <1 ; i++){
  delete [] msgarray[i]; //delete columns
  }
  delete [] msgarray; //delete array
}

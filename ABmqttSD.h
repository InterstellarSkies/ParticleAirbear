#ifndef __2AirMQTT_H_INCLUDED__   // if x.h hasn't been included yet...
#define __2AirMQTT_H_INCLUDED__   //   #define this so the compiler knows it has been included

#include "MQTT.h"
#include "SD.h"
#include "ABneopixel.h"

#define SMSLISTSTART 2

class BearMqttSD : public BearNeopixel{
public:
  bool connection();
  void latch(char* mqttmsg);
  bool loop();
  void SDbytes(char path[]);// dependency on neopixel write - create instance of SD
  bool fetchNsketch(char* pixelart,uint8_t frames);
  bool fetchNsketch(char *pixelart);
  void RetreiveMessages(char *msgrray[]);
  bool MqttBlock(int timeDelay);

private:

protected:
  void RetreivalCleanup(char *msgrray[]);
};




#endif

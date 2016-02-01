
#ifndef __Airbear2_H_INCLUDED__
#define __Airbear2_H_INCLUDED__


#include "ABmqttSD.h"
#include "Adafruit_VS1053.h"

#define BREAKOUT_RESET  D4      // VS1053 reset pin (output)
#define BREAKOUT_CS     A0    // VS1053 chip select pin (output)
#define BREAKOUT_DCS    A1     // VS1053 Data/command select pin (output)
#define CARDCS A2 ///
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ	D3    // VS1053 Data request, ideally an Interrupt pin

#define heartpin D5
#define squarepin D0
#define crosspin D6
#define circlepin D2





SYSTEM_THREAD(ENABLED);

class airbear: public BearMqttSD{
public:
  //airbear();
  void hatch();
  void listen();
  void spirit(uint8_t r,uint8_t g,uint8_t b);
  uint8_t MatrixBrightness = 100;

private:
  Adafruit_VS1053_FilePlayer vs1053= Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  int NewMod(int dividend, int divisor);
  void memoryStatus(); // debugging function to check space between stack and heap

    uint8_t head;
    uint8_t tail1;
    uint8_t tail2;
    uint8_t tail3;
    uint8_t tail4;

  void cross();
  void circle();
  void inbox();
  void heart();
  void cancelbutton();
  uint8_t PositionRandomizer(uint8_t n);
  bool Scrolling(int *ScrollingIndex);


};

#endif



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AirBearNeopixel_H_INCLUDED__   
#define __AirBearNeopixel_H_INCLUDED__ 


#include "neopixel.h"
#define PIXEL_PIN D1
#define PIXEL_COUNT 64
#define PIXEL_TYPE WS2812B



class BearNeopixel{
  public:
    void neopixelbegin();
    void drawXYPixel(uint8_t x,uint8_t y, int color);
    void setXYPixelColor( uint8_t x, uint8_t y,uint8_t r,uint8_t g,uint8_t b, int8_t cbrightness);
    int Color(uint8_t r,uint8_t g, uint8_t b);
    uint8_t setBrightness(uint8_t brightness);
    void refresh();
    void idle(int GhostColor[],bool notification);
    void show();
    void setNPixel(uint8_t n ,uint8_t r,uint8_t g,uint8_t b);
    uint32_t Wheel(byte WheelPos);
    int heart(uint8_t showing,uint8_t brightness);
  private:
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
    void fadeout(int prebrightness, int timer);

    /*TODO
      USE DMA
      Interpolation (with 24 bit color?)
      Interpolation LUT
      Gamma Adjustment for bear panel (make all pixelart more red)
      Ordered dithering using 3 channel Bayer matrix
      Temporal Dithering for brightness
      Extra: Fire animation
             Twinkling stars using fading and random x,y generator
             other cool animations
    */

};
#endif

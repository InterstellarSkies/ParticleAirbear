

/* Includes ------------------------------------------------------------------*/
#include "ABneopixel.h"


/*

generate a new gamma tailored to purple fabric  on server
const uint8_t gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

/* this is for the flexible strip in airbear
void stripdraw(uint8_t x,uint8_t y,int color){ // small function to map strip to matrix
  uint8_t stripnumber = y-1+(x*8);
  strip.setPixelColor(stripnumber,color)
}*/

void BearNeopixel::neopixelbegin(){
  strip.begin();
  strip.show();
  strip.setBrightness(100);

}
void BearNeopixel::drawXYPixel(uint8_t x,uint8_t y,int color){ // small function to map strip to strip
  uint8_t stripnumber = x+(y*8);
  strip.setPixelColor(stripnumber,color);
  strip.show();
}

int BearNeopixel::Color(uint8_t r, uint8_t g, uint8_t b){
  return (strip.Color(r,g,b));
}
/*
static uint32_t expandColor(uint16_t color) {
  return ((uint32_t)gamma[color >> 11 ] << 16 | (uint32_t)gamma[(color >> 5) & 0x3F] <<  8 |
                    gamma[ color       & 0x1F]);
}*/
void BearNeopixel::setXYPixelColor( uint8_t x, uint8_t y,uint8_t r,uint8_t g,uint8_t b, int8_t cbrightness) {
  int  color = strip.Color((cbrightness * r / 255) , (cbrightness * g / 255), (cbrightness * b / 255)); // the pgm_read is exponential gamma correction
  drawXYPixel(x, y, color);
  strip.show();
}




uint32_t BearNeopixel::Wheel(byte WheelPos) { // The Color Wheel which will select a color baseed on the input base
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

uint8_t BearNeopixel::setBrightness(uint8_t brightness){
  strip.setBrightness(brightness);
  return brightness;
}

void BearNeopixel::refresh(){
  strip.clear();
  strip.show();
}

void BearNeopixel::setNPixel(uint8_t n ,uint8_t r,uint8_t g,uint8_t b){
  strip.setPixelColor(n,r,g,b);
  strip.show();
}
void BearNeopixel::show(){
  strip.show();
}

void BearNeopixel::fadeout(int prebrightness, int timer) {
  for (int q = prebrightness ; q >= 0  ; q--) {

    strip.setBrightness(q);
    strip.show();
    delay(timer);
  }
  strip.setBrightness(prebrightness);
}

int BearNeopixel::heart(uint8_t showing,uint8_t brightness) {
  uint8_t heartx[]  = {4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 1, 1, 2, 3, 2};//Outer layer of the heart
  uint8_t hearty[]  = {5, 4, 3, 2, 1, 0, 0, 1, 0, 0, 1, 2, 3, 4, 3};
  
  uint8_t heartx1[] = {4, 5, 6, 6, 5, 4, 3, 2, 2, 3};//Middle layer of the heart
  uint8_t hearty1[] = {4, 3, 2, 1, 1, 2, 1, 1, 2, 3};
  
  uint8_t heartx2[] = {4, 5, 3}; //Core of the heart
  uint8_t hearty2[] = {3, 2, 2};

  if (showing == 1) { //
    strip.setBrightness(brightness);

    for (int j = 0; j < 256; j++) {
      //if (cancelpin()==true){return(1);}
      for (int lol = 0;  lol < 15 ; lol++) {// + 30 ( 2 colors at same time),9 double rainbow , the + 40 added to j causes a faster color change in the ring

        drawXYPixel(heartx[lol], hearty[lol], (Wheel((lol + j  ) & 255)));
        strip.show();
      }

      for (int lol = 0;  lol < 10 ; lol++) {

        drawXYPixel(heartx1[lol], hearty1[lol], Wheel((lol + j) & 255));
        strip.show();
      }

      for (int lol = 0;  lol < 3 ; lol++) {


        drawXYPixel(heartx2[lol], hearty2[lol], Wheel((lol + j) & 255));
        strip.show();
      }
    }
    fadeout(brightness, 20);
    refresh();
    return (showing);
  }
  if (showing == 2) { // Full raibow cycling through heart
    strip.setBrightness(brightness);
    for (int j = 0; j < 256 ; j++) {
      //if(cancelpin()==true){return(2);}
      for (int lol = 0;  lol < 14 ; lol++) {
        int color = Wheel(((lol * 255 / 14) + j) & 255);
        drawXYPixel(heartx[lol], hearty[lol], color);
        strip.show();
      }

    }
    fadeout(brightness, 20);
    refresh();
    return (2);

  }

  if (showing == 3) {// rainbow heart which has 2 colors on the screen at a time
    strip.setBrightness(brightness);
    for (int j = 0; j < 256; j++) {
      //if (cancelpin()==true){return(3);}
      for (int lol = 0;  lol < 15 ; lol++) {
        int color = (Wheel((lol + j + 40  ) & 255));

        drawXYPixel(heartx[lol], hearty[lol], color);
        strip.show();
      }

      for (int lol = 0;  lol < 10 ; lol++) {

        drawXYPixel(heartx1[lol], hearty1[lol], Wheel((lol + j) & 255));
        strip.show();
      }

      for (int lol = 0;  lol < 3 ; lol++) {
        drawXYPixel(heartx2[lol], hearty2[lol], Wheel((lol + j) & 255));
        strip.show();
      }
    }
    fadeout(brightness, 60);
    refresh();
    return (showing);
  }



  if (showing == 4) {// Red Heart
    strip.setBrightness(brightness);
    for (uint8_t lol = 0;  lol < 15 ; lol++) {
      drawXYPixel(heartx[lol], hearty[lol], strip.Color(255,0,0));
      strip.show();
    }

    for (uint8_t lol = 0;  lol < 10 ; lol++) {
      drawXYPixel(heartx1[lol], hearty1[lol], strip.Color(255,0,0));
      strip.show();
    }

    for (uint8_t lol = 0;  lol < 3 ; lol++) {
      //int color = Wheel(((lol * 256 /14) + j) & 255);

      drawXYPixel(heartx2[lol], hearty2[lol], strip.Color(255,0,0));
      strip.show();
    }
    return (showing);
  }
}

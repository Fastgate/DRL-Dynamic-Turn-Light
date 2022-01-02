#include "Arduino.h"
#include <FastLED.h>

#include "FastLED_RGBW.h"

//#include <WS2812Serial.h>
//#define USE_WS2812SERIAL

#define DATA_PIN 0               //LED Strip Signal Connection 
#define DRLSignal 1             //DRL Switch Signal Connection
#define LeftSignal 2            //Left Blinker Signal Connection
#define RightSignal 4           //Right Blinker Signal Connection

#define NUM_LEDS 28             //Total no of LEDs in two LED strips (eg. Use 40 here for two 20 LED strips)

#define BlinkerLEDs NUM_LEDS/2 

int BlinkerSpeed = 30;          //Turn Signal Running LED Speed. Adjust this to match with your vehicle turn signal speed.
int BlinkerOffDelay = 250;      //Turn Signal Off time. Adjust this to match with your vehicle turn signal speed.

int StartupSpeed = 25;
int DRLDetect = 0;

int DRLColour = 3;              //Change LED colour here, 1-Ice Blue, 2-Blue, 3-White, 4-Orange

CRGBW leds[NUM_LEDS];
CRGB *ledsRGB = (CRGB *) &leds[0];
int r,r_dim,g,g_dim,b,b_dim,w,w_dim;


void setup() 
{
FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(NUM_LEDS));
pinMode(DRLSignal, INPUT);
pinMode(LeftSignal, INPUT);
pinMode(RightSignal, INPUT);
}


void loop() 
{
if(DRLDetect == 0)
{
if(digitalRead(DRLSignal)==1) 
{
DRLDetect = 1;
}
}

if(DRLDetect == 2)
{
if(digitalRead(DRLSignal)==0) 
{
DRLDetect = 3;
}
}

if(DRLDetect == 1)
{
DRL_ON();
DRLDetect = 2;  
}

if(DRLDetect == 3)
{
DRL_OFF();
DRLDetect = 0;  
}


if(digitalRead(DRLSignal)==0)
{
if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==0)) //Left Blinker
{
RightOff();
LeftBlinker();
LeftDim();
delay (BlinkerOffDelay);
}

if((digitalRead(RightSignal)==1)&&(digitalRead(LeftSignal)==0)) //Right Blinker
{
LeftOff();
RightBlinker();
RightDim();
delay (BlinkerOffDelay);
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==1)) //Dual Blinker / Hazard
{
MiddleOff();
DualBlinker();
LeftDim();
RightDim();
delay (BlinkerOffDelay);
}
}


if(DRLDetect == 2)
{
if((digitalRead(LeftSignal)==0)&&(digitalRead(RightSignal)==0)) //Park Light
{   
ParkFull();
}
    
if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==0)) //Left Blinker
{
LeftDim();
RightLit(); 
LeftBlinker();
LeftDim();
delay (BlinkerOffDelay);
}

if((digitalRead(RightSignal)==1)&&(digitalRead(LeftSignal)==0)) //Right Blinker
{
RightDim();
LeftLit();
RightBlinker();
RightDim();
delay (BlinkerOffDelay);
}

if((digitalRead(LeftSignal)==1)&&(digitalRead(RightSignal)==1)) //Dual Blinker / Hazard
{
LeftDim();
RightDim();
ParkMiddle();
DualBlinker();
LeftDim();
RightDim();
delay (BlinkerOffDelay);
}
}

switch (DRLColour) 
 {
  case 1:
  g = 255;
  g_dim = 30;
  b = 255;
  b_dim = 30;
  break;
  
  case 2:
  b = 255;
  b_dim = 30;
  break; 

  case 3:
  //r = 255;
  //r_dim = 30;
  //g = 255;
  //g_dim = 30;
  //b = 255;
  //b_dim = 30;
  w = 255;
  w_dim = 30;
  break; 

  case 4:
  r = 255;
  r_dim = 30;
  g = 165;
  g_dim = 20;
  b = 0;
  b_dim = 0;
  break; 
}

}

void DRL_ON()
{
  for (int j = ((NUM_LEDS/2)-1); j >= 0; j--)
  {
    leds[j] = CRGBW(r_dim, g_dim, b_dim, w_dim);
    leds[j+1] = CRGBW(0, 0, 0, 0);
    leds[(NUM_LEDS/2-1)+((NUM_LEDS/2)-j)] = CRGBW(r_dim, g_dim, b_dim, w_dim);
    leds[(NUM_LEDS/2-1)+((NUM_LEDS/2)-j)-1] = CRGBW(0, 0, 0, 0);
    FastLED.show();
    delay (StartupSpeed);    
  }
  
  for (int i = 0; i < (NUM_LEDS/2); i++)
  {
    leds[i] = CRGBW(r_dim, g_dim, b_dim, w_dim);
    leds[(NUM_LEDS-1)-i] = CRGBW(r_dim, g_dim, b_dim, w_dim);
    FastLED.show();
    delay (StartupSpeed);    
  }
  

  for (int j = ((NUM_LEDS/2)-1); j >= 0; j--)
  {
    leds[j] = CRGBW(r, g, b, w);
    leds[(NUM_LEDS/2-1)+((NUM_LEDS/2)-j)] = CRGBW(r, g, b, w);
    FastLED.show();
    delay (StartupSpeed);    
  }
}

void DRL_OFF()
{

  for (int j = 0; j <= ((NUM_LEDS/2)-1); j++)
  {
    leds[j] = CRGBW(r, g, b, w);
    leds[j-1] = CRGBW(r_dim, g_dim, b_dim, w_dim);
    leds[(NUM_LEDS-1)-j] = CRGBW(r, g, b, w);
    leds[(NUM_LEDS)-j] = CRGBW(r_dim, g_dim, b_dim, w_dim);
    FastLED.show();
    delay (StartupSpeed);    
  }

  leds[((NUM_LEDS/2)-1)] = CRGBW(r_dim, g_dim, b_dim, w_dim);
  leds[((NUM_LEDS/2)-1)+1] = CRGBW(r_dim, g_dim, b_dim, w_dim);
  FastLED.show();

    for (int j = ((NUM_LEDS/2)-1); j >= 0; j--)
  {
    leds[j] = CRGBW(r_dim, g_dim, b_dim, w_dim);
    leds[j+1] = CRGBW(0, 0, 0, 0);
    leds[(NUM_LEDS/2-1)+((NUM_LEDS/2)-j)] = CRGBW(r_dim, g_dim, b_dim, w_dim);
    leds[(NUM_LEDS/2-1)+((NUM_LEDS/2)-j)-1] = CRGBW(0, 0, 0, 0);
    FastLED.show();
    delay (StartupSpeed);    
  }
  
  for (int j = 0; j <= ((NUM_LEDS/2)-1); j++)
  {
    leds[j] = CRGBW(r_dim, g_dim, b_dim, w_dim);
    leds[j-1] = CRGBW(0, 0, 0, 0);
    leds[(NUM_LEDS-1)-j] = CRGBW(r_dim, g_dim, b_dim, w_dim);
    leds[(NUM_LEDS)-j] = CRGBW(0, 0, 0, 0);
    FastLED.show();
    delay (StartupSpeed);    
  }

    leds[(NUM_LEDS/2)-1] = CRGBW(0, 0, 0, 0);
    leds[NUM_LEDS/2] = CRGBW(0, 0, 0, 0);
    FastLED.show();
}

void AllOff()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
    FastLED.show();  
}

void LeftOff()
{
  for (int i = 0; i < (NUM_LEDS - BlinkerLEDs); i++)
  {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
    FastLED.show();
}

void RightOff()
{
  for (int i = BlinkerLEDs; i < NUM_LEDS; i++)
  {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
    FastLED.show();
}

void MiddleOff()
{
  for (int i = BlinkerLEDs; i < (NUM_LEDS - BlinkerLEDs); i++)
  {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
    FastLED.show();  
}

void ParkFull()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGBW(r, g, b, w);
  }
    FastLED.show();
}

void ParkMiddle()
{
  for (int i = BlinkerLEDs; i < (NUM_LEDS - BlinkerLEDs); i++)
  {
    leds[i] = CRGBW(60, 0, 0, 0);
  }
    FastLED.show();  
}

void LeftBlinker()
{
  for (int i = (BlinkerLEDs-1); i >= 0; i--)
  {
    leds[i] = CRGBW(255, 165, 0, 0);
    FastLED.show();
    delay (BlinkerSpeed);    
  }
}

void LeftDim()
{
  for (int i = 0; i < BlinkerLEDs; i++)
  {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
    FastLED.show();
}

void LeftLit()
{
  for (int i = 0; i < (NUM_LEDS - BlinkerLEDs); i++)
  {
    leds[i] = CRGBW(r, g, b, w);
  }
    FastLED.show();
}

void LeftFull()
{
  for (int i = 0; i < (NUM_LEDS - BlinkerLEDs); i++)
  {
    leds[i] = CRGBW(r, g, b, w);
  }
    FastLED.show();
}

void RightBlinker()
{
  for (int i = (NUM_LEDS - BlinkerLEDs); i < NUM_LEDS; i++)
  {
    leds[i] = CRGBW(255, 165, 0, 0);
    FastLED.show();
    delay (BlinkerSpeed);
  }
}

void RightDim()
{
   for (int i = (NUM_LEDS - BlinkerLEDs); i < NUM_LEDS; i++)
  {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
    FastLED.show();
}

void RightLit()
{
  for (int i = BlinkerLEDs; i < NUM_LEDS; i++)
  {
    leds[i] = CRGBW(r, g, b, w);
  }
    FastLED.show();
}

void RightFull()
{
  for (int i = BlinkerLEDs; i < NUM_LEDS; i++)
  {
    leds[i] = CRGBW(r, g, b, w);
  }
    FastLED.show();
}

void DualBlinker()
{
  for (int i = (BlinkerLEDs-1); i >= 0; i--)
  {
    leds[i] = CRGBW(255, 165, 0, 0);
    leds[NUM_LEDS-1-i] = CRGBW(255, 165, 0, 0);
    FastLED.show();
    delay (BlinkerSpeed);
  }
}

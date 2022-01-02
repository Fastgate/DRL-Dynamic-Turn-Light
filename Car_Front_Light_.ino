// HAZI TECH
// Program by Hasitha Jayasundara
// Visit my YouTube Channel - http://www.youtube.com/c/HAZITECH?sub_confirmation=1

#include "Arduino.h"
//#include <WS2812Serial.h>
//#define USE_WS2812SERIAL
#include <FastLED.h>
   
#include "FastLED_RGBW.h"

#define DATA_PIN 0               //LED Strip Signal Connection 
#define DRLSignal 1             //DRL Switch Signal Connection
#define LeftSignal 2            //Left Blinker Signal Connection
#define RightSignal 4           //Right Blinker Signal Connection

#define NUM_LEDS 14             //Total no of LEDs in two LED strips (eg. Use 40 here for two 20 LED strips)
#define BRIGHTNESS  155

#define BlinkerLEDsLeft NUM_LEDS -12 
#define BlinkerLEDsRight NUM_LEDS -10 

int BlinkerSpeed = 85;          //Turn Signal Running LED Speed. Adjust this to match with your vehicle turn signal speed.
int BlinkerOffDelay = 250;      //Turn Signal Off time. Adjust this to match with your vehicle turn signal speed.

int StartupSpeed = 25;
int DRLDetect = 0;

int DRLColour = 3;              //Change LED colour here, 1-Ice Blue, 2-Blue, 3-White, 4-Orange, Cool White

//CRGB leds[NUM_LEDS];
//int r,r_dim,g,g_dim,b,b_dim;

// FastLED
//CRGB leds[NUM_LEDS];
 
// FastLED with RGBW
CRGBW leds[NUM_LEDS];
CRGB *ledsRGB = (CRGB *) &leds[0];
int r,r_dim,g,g_dim,b,b_dim,w,w_dim;


void setup() 
{
//FastLED
//FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
 
//FastLED with RGBW
FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(NUM_LEDS));
  
//FastLED.addLeds<SK6812, LED_PIN, GRB>(leds, NUM_LEDS);
FastLED.setBrightness(  BRIGHTNESS );
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

  case 5:
  r = 244;
  r_dim = 30;
  g = 253;
  g_dim = 30;
  b = 255;
  b_dim = 30;
  w = 255;
  w_dim = 30;
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
    leds[(NUM_LEDS/2-1)+((NUM_LEDS/2)-j)-1] = CRGBW(0, 0, 0, 255);
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
    leds[j] = CRGBW(r, g, b, w  );
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
  for (int i = 0; i < (NUM_LEDS - BlinkerLEDsRight); i++)
  {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
    FastLED.show();
}

void RightOff()
{
  for (int i = BlinkerLEDsLeft; i < NUM_LEDS; i++)
  {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
    FastLED.show();
}

void MiddleOff()
{
  for (int i = (BlinkerLEDsLeft + BlinkerLEDsRight); i < (NUM_LEDS - (BlinkerLEDsLeft + BlinkerLEDsRight)); i++)
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
    FastLED.setBrightness(  BRIGHTNESS );
    FastLED.show();
}

void ParkMiddle()
{
  for (int i = (BlinkerLEDsLeft + BlinkerLEDsRight); i < (NUM_LEDS - (BlinkerLEDsLeft + BlinkerLEDsRight)); i++)
  {
    leds[i] = CRGBW(60, 0, 0, 0);
  }
    FastLED.show();  
}

void LeftBlinker()
{
  for (int i = (BlinkerLEDsLeft-1); i >= 0; i--)
  {
    leds[i] = CRGBW(255, 0, 0, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);

    leds[i] = CRGBW(255, 255, 0, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
    leds[i] = CRGBW(255, 255, 255, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
    leds[i] = CRGBW(255, 255, 255, 255);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
     

         
  }
}


void LeftDim()
{
  for (int i = 0; i < BlinkerLEDsLeft; i++)
  {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
    FastLED.show();
}

void LeftLit()
{
  for (int i = 0; i < (NUM_LEDS - BlinkerLEDsLeft); i++)
  {
    leds[i] = CRGBW(r, g, b, w);
  }
    FastLED.show();
}

void LeftFull()
{
  for (int i = 0; i < (NUM_LEDS - BlinkerLEDsLeft); i++)
  {
    leds[i] = CRGBW(r, g, b, w);
  }
  FastLED.setBrightness(  BRIGHTNESS );
    FastLED.show();
}

void RightBlinker()
{
  for (int i = (BlinkerLEDsRight -1); i >= 2; i--)
  {
    leds[i] = CRGBW(255, 0, 0, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);

    leds[i] = CRGBW(255, 255, 0, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
    leds[i] = CRGBW(255, 255, 255, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
    leds[i] = CRGBW(255, 255, 255, 255);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
    
  }
}

void RightDim()
{
   for (int i = 1; i < BlinkerLEDsRight; i++)
   //for (int i = 0; i < BlinkerLEDsLeft; i++)
  {
    leds[i] = CRGBW(0, 0, 0, 0);
  }
    FastLED.show();
}

void RightLit()
{
  for (int i = BlinkerLEDsRight; i < NUM_LEDS; i++)
  {
    leds[i] = CRGBW(r, g, b, w);
  }
    FastLED.show();
}

void RightFull()
{
  for (int i = (BlinkerLEDsLeft + BlinkerLEDsRight) ; i < NUM_LEDS; i++)
  {
    leds[i] = CRGBW(r, g, b, w);
  }
  FastLED.setBrightness(  BRIGHTNESS );
    FastLED.show();
}

void DualBlinker()
{
  for (int i = BlinkerLEDsLeft -1; i >= 0; i--)
  {
    leds[i] = CRGBW(255, 0, 0, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);

    leds[i] = CRGBW(255, 255, 0, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
    leds[i] = CRGBW(255, 255, 255, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
    leds[i] = CRGBW(255, 255, 255, 255);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
  }
  for (int i = BlinkerLEDsRight -1; i >= 2; i--)
  {
    leds[i] = CRGBW(255, 0, 0, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);

    leds[i] = CRGBW(255, 255, 0, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
    leds[i] = CRGBW(255, 255, 255, 0);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
    leds[i] = CRGBW(255, 255, 255, 255);  // SK6812
    FastLED.show();
    delay (BlinkerSpeed);
    
  }
  
}

#include "FastLED.h"

#define DATA_PIN1    8
#define DATA_PIN2    7

#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

// 2 strips with an empty space between em
#define NUM_LEDS1   35
#define SKIP_LEDS   38
#define NUM_LEDS2   104


CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];

CLEDController *controllers[2];

void setup() {
  FastLED.setDither( 0 );
  controllers[0] = &FastLED.addLeds<WS2812, DATA_PIN1, GRB>(leds1, NUM_LEDS1);
  controllers[1] = &FastLED.addLeds<WS2812, DATA_PIN2, GRB>(leds2, NUM_LEDS2);

  memset(leds1, 0, NUM_LEDS1*sizeof(CRGB));
  memset(leds2, 0, NUM_LEDS1*sizeof(CRGB));
  FastLED.show();

  // tryin to fight some artifacts at power-up
  delay(100);
  FastLED.show();
}

DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
  0,     0,  0,  0,
 10,   255,  0,  0,
 50,     0,255,  0,
 80,     0,  0,255,
120,   255,  0,255, 
255,   255,255,255 };

CRGBPalette16 myPal = heatmap_gp;


void loop() { 

  uint32_t t = millis();
  
  if( t < 60000 )
  {
    EVERY_N_MILLISECONDS(33) {
  
      const uint16_t totalPoints = NUM_LEDS1 + SKIP_LEDS + NUM_LEDS2;
      for( int16_t i = 0; i < totalPoints; i++){
  
        // ignore the gap
        if( i >= NUM_LEDS1 && i < NUM_LEDS1 + SKIP_LEDS )
          continue;
  
        // kinda 'wave'
        uint32_t phase = t / 2;
        uint16_t offset = i * 5;
        if( phase < offset ) 
          phase = 0;
        else
          phase -= offset;
  
        CRGB color(255,255,255);
        // don't draw at 0xFF cause FasLED blends between last and first???
        if( phase < 0xFA )
          color = ColorFromPalette(myPal, phase, 255);
  
        //
        if( i < NUM_LEDS1 )
          leds1[i] = color;
        else if( i >= NUM_LEDS1 + SKIP_LEDS ){
          int16_t l2idx = totalPoints - i - 1;
          leds2[l2idx] = color;
        }
      }
      controllers[0]->showLeds(255);
      controllers[1]->showLeds(255);
    }
  
  }
  else
  {
    // even if I failed something, it'll be fully lit after 1 minute
    for( int16_t i = 0; i < NUM_LEDS1; i++){
      leds1[i]=CRGB(255,255,255);
    }
  
    controllers[0]->showLeds(255);
    delay(1000);
    
    for( int16_t i = 0; i < NUM_LEDS2; i++){
      leds2[i]=CRGB(255,255,255);
    }
    controllers[1]->showLeds(255);
    delay(1000);
  }
}

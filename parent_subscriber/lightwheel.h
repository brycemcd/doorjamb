// This code is responsible for setting up
// and managing the functions for the lightwheel (8 x WS2812 5050 RGB LED Ring)
//
// The expectation is that the code will be called from the parent_subscriber.ino
// code 

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define COLORWHEEL_DIN_PIN 12 // GPIO pin 12 = ESP D5
#define LED_OFF_COLOR 0
#define LED_BLUE strip.Color(0, 0, 127)
#define LED_RED strip.Color(127, 0, 0)

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
// Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, COLORWHEEL_DIN_PIN, NEO_GRB + NEO_KHZ800);

// Borrowed from strandtest in the example library
// Demonstration of lights chasing one another
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void blinkAllThelights(uint32_t color) {
  for(uint16_t i=0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, color);
  }
  strip.show();
  
  delay(50);
  for(uint16_t i=0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, LED_OFF_COLOR);
  }
  strip.show();
  delay(50);
}

void setupColorWheel() {
  // Assumes Serial is aleady initialized!
  Serial.println("Initializing Colorwheel");
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.println("Colorwheel init'd");
}

void colorWheelAlert() {
  //theaterChase(strip.Color(127, 0, 0), 50); // Red
  //theaterChase(strip.Color(0, 0, 127), 50); // Blue
  blinkAllThelights(LED_RED);
  blinkAllThelights(LED_BLUE);
}

/*

This code is responsible for controlling the multicolored LED
A majority of this code was inspired from https://learn.adafruit.com/adafruit-arduino-lesson-3-rgb-leds?view=all

*/

#define MULTICOLOR_BLINK_DELAY 100

int redPin = 0; // D3
int greenPin = 4; // D2
int bluePin = 5; // D1

void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

void fullBlue() {
  setColor(0, 0, 255);  // blue
}

void fullGreen() {
  setColor(0, 255, 0);  // green
}

void fullRed() {
  setColor(255, 0, 0);  // red
}

void fullOrange() {
  setColor(255,165,0);
}

void turnOff() {
  // NOTE: black = no power to LEDs = off
  setColor(0,0,0);
}

void multicolorLEDSetup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void mutlicolorLEDMQTTConnecting() {
  for(uint8_t i = 0; i < 10; i++){
    fullOrange();
    delay(MULTICOLOR_BLINK_DELAY);
    turnOff();
    delay(MULTICOLOR_BLINK_DELAY);
  }
}

void multicolorLEDMQTTSuccess() {
  for(uint8_t i = 0; i < 5; i++){
    fullGreen();
    delay(MULTICOLOR_BLINK_DELAY);
    turnOff();
    delay(MULTICOLOR_BLINK_DELAY);
  }
}

void multicolorLEDMQTTFail() {
  for(uint8_t i = 0; i < 5; i++){
    fullRed();
    delay(MULTICOLOR_BLINK_DELAY);
    turnOff();
    delay(MULTICOLOR_BLINK_DELAY);
  }
}

void multicolorLEDMQTTAlert() {
for(uint8_t i = 0; i < 2; i++){
    fullBlue();
    delay(MULTICOLOR_BLINK_DELAY);
    turnOff();
    delay(MULTICOLOR_BLINK_DELAY);
    fullGreen();
    delay(MULTICOLOR_BLINK_DELAY);
    turnOff();
    delay(MULTICOLOR_BLINK_DELAY);
  }
}


/**
 * This example shows how to use the matrix keyboard support that's built into IoAbstraction,
 * it can be used out the box with either a 3x4 or 4x4 keypad, but you can modify it to use
 * any matrix keyboard quite easily.
 * It just sends the characters that are typed on the keyboard to Serial. The keyboard in This
 * example is connected directly to Arduino pins, but could just as easily be connected over
 * a PCF8574, MCP23017 or other IoAbstraction.
 */

#include <Wire.h>
#include <IoAbstraction.h>
#include<TaskManagerIO.h>
#include <KeyboardManagerFG.h>

#include "FastLED.h"
#define DATA_PIN 2
#define LED_TYPE WS2813
#define COLOR_ORDER GRB
#define NUM_LEDS 79
#define BRIGHTNESS 30
#define UPDATES_PER_SECOND 5 
#define FRAMES_PER_SECOND  120

CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
//
// We need to make a keyboard layout that the manager can use. choose one of the below.
// The parameter in brackets is the variable name.

MAKE_KEYBOARD_LAYOUT_3X8(keyLayout)
//
// We need a keyboard manager class too
//
MatrixKeyboardManager keyboard;

// this examples connects the pins directly to an arduino but you could use
// IoExpanders or shift registers instead.
IoAbstractionRef arduinoIo = ioUsingArduino();

//
// We need a class that extends from KeyboardListener. this gets notified when
// there are changes in the keyboard state.
//
class MyKeyboardListener : public KeyboardListener {
public:
    void keyPressed(char key, bool held) override {
        Serial.print("Key ");
        Serial.print(key);
        Serial.print(" is pressed, held = ");
        Serial.println(held);
    }

    void keyReleased(char key) override {
        Serial.print("Released ");
        Serial.println(key);
    }
} myListener;

void setup() {
    while(!Serial);
    Serial.begin(115200);

    keyLayout.setRowPin(0, 10);
    keyLayout.setRowPin(1, 9);
    keyLayout.setRowPin(2, 8);
    keyLayout.setRowPin(3, 7);
    keyLayout.setRowPin(4, 6);
    keyLayout.setRowPin(5, 5);
    keyLayout.setRowPin(6, 4);
    keyLayout.setRowPin(7, 3);
    
    keyLayout.setColPin(0, 13);
    keyLayout.setColPin(1, 12);
    keyLayout.setColPin(2, 11);

    // create the keyboard mapped to arduino pins and with the layout chosen above.
    // it will callback our listener
    keyboard.initialise(arduinoIo, &keyLayout, &myListener);

    // start repeating at 850 millis then repeat every 350ms
    keyboard.setRepeatKeyMillis(850, 350);

    Serial.println("Keyboard is initialised!");

//   LED  Start
{
  delay(3000); // initial delay of a few seconds is recommended
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip); // initializes LED strip
  FastLED.setBrightness(BRIGHTNESS);// global brightness
}
}


// switches off all LEDs
void showProgramCleanUp(long delayTime) {
  for (int i = 0; i < NUM_LEDS; ++i) {
   leds[i] = CRGB::Black;
  }
  FastLED.show();
  delay(delayTime);
}


void loop() {
    // as this indirectly uses taskmanager, we must include this in loop.
    taskManager.runLoop();
      for (int i = 0; i < NUM_LEDS; ++i) {
   leds[i] = CRGB::Blue;
  }
  FastLED.show();
  
}
   
 

/*

Blinky Box Programmable Toy
by Miria Grunick

This is the Teensy code for my Blinky Box project here:
http://blog.grunick.com/blinky-box/

This code was written for the Teensy 3. The button pins will need to change if
you want to use an earlier version of Teensy.

This is free to use and modify!
*/

#include "Encoder.h"
#include "PinChangeInterrupt.h"
#include "effects.h"
#include <FastLED.h>
#include <avr/interrupt.h>
#include <avr/io.h>

// LED constants
#define NUM_LEDS 32
#define LED_DATA_PIN 7
#define BRIGHTNESS 64 // 0-255.

// Button constants
#define RED_PIN 8
#define GREEN_PIN 9
#define BLUE_PIN 10
#define YELLOW_PIN 11
#define BLACK_PIN 12

// Knob constants
#define ENC_1_CLK 2
#define ENC_1_DT 4
#define ENC_2_CLK 3
#define ENC_2_DT 5
#define ENC_STEPS 20

#define WAIT 1000

// Constants used for rainbows
const int NUM_COLORS = 16;
const int rainbow_r[] = {127, 127, 127, 127, 127, 64, 0,   0,
                         0,   0,   0,   20,  40,  83, 127, 127};
const int rainbow_g[] = {0,   20, 40, 83, 127, 127, 127, 127,
                         127, 64, 0,  0,  0,   0,   0,   0};
const int rainbow_b[] = {0,   0,   0,   0,   0,   0,  0,  32,
                         127, 127, 127, 127, 127, 83, 40, 20};

// Array of LEDs
CRGB leds[NUM_LEDS];

// Array of colors.
const CRGB colors[] = {CRGB::White, CRGB::Red, CRGB::Green, CRGB::Blue,
                       CRGB::Yellow};
CRGB color = CRGB::White;

volatile int color_idx = 0;
int old_color_idx = 0;

volatile int pattern = 0;
volatile long enc1PrevState = 0;
volatile long enc2PrevState = 0;

Encoder knob(ENC_1_CLK, ENC_1_DT);
Encoder knob2(ENC_2_CLK, ENC_2_DT);

// Effects iniitalization.
SolidLights *solidLights = new SolidLights();
RainDrop *rainDrop = new RainDrop();
OnOffLights *onOffLights = new OnOffLights();
FadeLights *fadeLights = new FadeLights();
AlternateLights *alternateLights = new AlternateLights();
Blink *blink = new Blink();
Twinkle *twinkle = new Twinkle;
Rainbow *rainbow = new Rainbow();

Effect *effects[] = {solidLights, blink,           rainDrop, onOffLights,
                     fadeLights,  alternateLights, twinkle,  rainbow};
int numPatterns = sizeof(effects) / sizeof(Effect *);

unsigned long wait = 0;
unsigned long prevMillis = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Start setup!");

  pinMode(RED_PIN, INPUT_PULLUP);
  attachPCINT(digitalPinToPCINT(RED_PIN), interruptRed, FALLING);

  pinMode(YELLOW_PIN, INPUT_PULLUP);
  attachPCINT(digitalPinToPCINT(YELLOW_PIN), interruptYellow, FALLING);

  pinMode(GREEN_PIN, INPUT_PULLUP);
  attachPCINT(digitalPinToPCINT(GREEN_PIN), interruptGreen, FALLING);

  pinMode(BLUE_PIN, INPUT_PULLUP);
  attachPCINT(digitalPinToPCINT(BLUE_PIN), interruptBlue, FALLING);

  pinMode(BLACK_PIN, INPUT_PULLUP);
  attachPCINT(digitalPinToPCINT(BLACK_PIN), interruptBlack, FALLING);

  FastLED.addLeds<WS2812B, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::White);
  Serial.print("Number of patterns: ");
  Serial.println(numPatterns);
  Serial.println("Patterns:");
  for (int i = 0; i < numPatterns; i++) {
    Serial.print("\t");
    effects[i]->print();
  }
  Serial.println("End setup!");
}

void interruptRed() { color_idx = 1; }

void interruptGreen() { color_idx = 2; }

void interruptBlue() { color_idx = 3; }

void interruptYellow() { color_idx = 4; }

void interruptBlack() { color_idx = 0; }

void printColor(CRGB c) {
  Serial.print("R");
  Serial.print(c.r);
  Serial.print(" G");
  Serial.print(c.g);
  Serial.print(" B");
  Serial.println(c.b);
}

void loop() {
  // Read knob
  long knobState = knob.read();
  long knob2State = knob2.read();
  if (knobState != enc1PrevState) {
    enc1PrevState = knobState;
    long k = (knobState / 4) % numPatterns;
    Serial.print("k: ");
    Serial.println(k);
    if (k >= 0) {
      pattern = k;
    } else {
      pattern = numPatterns + k;
    }

    Serial.print("Pattern: ");
    Serial.println(pattern);
    Serial.print("Color: ");
    printColor(color);
    effects[pattern]->print();
  }
  if (old_color_idx != color_idx) {
    color = colors[color_idx];
    Serial.print("Color_idx: ");
    Serial.println(color_idx);
    Serial.print("old color: ");
    printColor(colors[old_color_idx]);
    Serial.print("new color: ");
    printColor(colors[color_idx]);
    old_color_idx = color_idx;
  }
  if (knob2State != enc2PrevState) {
    enc2PrevState = knob2State;
    uint32_t hue = (uint8_t)abs(knob2State % 256);
    Serial.print("hue: ");
    Serial.println(hue);
    color.setHue(hue);
    Serial.print("knob color ");
    printColor(color);
    old_color_idx = -1;
    color_idx = -1;
  }
  if (millis() > (prevMillis + wait)) {
    wait = effects[pattern]->show(color, WAIT);
    prevMillis = millis();
  }
}

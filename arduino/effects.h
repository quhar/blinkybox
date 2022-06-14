#ifndef EFFECTS_H

#ifndef NUM_LEDS
#define NUM_LEDS 32
#endif

#include <FastLED.h>

class Effect {
public:
  void clearStrip(bool show);
  void solidLights(CRGB color);
  void setIfPresent(int idx, CRGB color);
  virtual unsigned long show(CRGB color, unsigned long wait) = 0;
  virtual void print() = 0;
  virtual ~Effect(){};
};

class SolidLights : public Effect {
public:
  virtual unsigned long show(CRGB color, unsigned long wait);
  virtual void print();
  virtual ~SolidLights(){};
};

class RainDrop : public Effect {
public:
  RainDrop() { chaseState = 0; }
  virtual unsigned long show(CRGB color, unsigned long wait);
  virtual void print();
  virtual ~RainDrop(){};

private:
  int chaseState;
};

class OnOffLights : public Effect {
public:
  OnOffLights() { isOn = true; }
  virtual unsigned long show(CRGB color, unsigned long wait);
  virtual void print();
  virtual ~OnOffLights(){};

private:
  bool isOn;
};

class FadeLights : public Effect {
public:
  FadeLights() { fadeState = 0; }
  virtual unsigned long show(CRGB color, unsigned long wait);
  virtual void print();
  virtual ~FadeLights(){};

private:
  uint8_t fadeState;
  const uint8_t transitions = 16;
  const uint8_t step = 256 / transitions;
};

class AlternateLights : public Effect {
public:
  AlternateLights() { state = true; }
  virtual unsigned long show(CRGB color, unsigned long wait);
  virtual void print();
  virtual ~AlternateLights(){};

private:
  bool state;
};

class Blink : public Effect {
public:
  Blink() { state = true; }
  virtual unsigned long show(CRGB color, unsigned long wait);
  virtual void print();
  virtual ~Blink(){};

private:
  bool state;
};

class Twinkle : public Effect {
public:
  virtual unsigned long show(CRGB color, unsigned long wait);
  virtual void print();
  virtual ~Twinkle(){};
};

class Rainbow : public Effect {
public:
  Rainbow() { state = 0; }
  virtual unsigned long show(CRGB color, unsigned long wait);
  virtual void print();
  virtual ~Rainbow(){};

private:
  int state;
};

#endif // EFFECTS_H

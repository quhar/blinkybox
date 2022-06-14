void Effect::clearStrip(bool show) {
  for (int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = CRGB::Black;
  }
  if (show)
    FastLED.show();
}

void Effect::solidLights(CRGB color) {
  fill_solid(leds, NUM_LEDS, color);
  FastLED.show();
}

void Effect::setIfPresent(int idx, CRGB color) {
  // Don't write to non-existent pixels
  if (idx >= 0 && idx < NUM_LEDS)
    leds[idx] = color;
}

void SolidLights::print() { Serial.println("SolidLights"); }
unsigned long SolidLights::show(CRGB color, unsigned long wait) {
  solidLights(color);
  return 0;
}

void RainDrop::print() { Serial.println("RainDrop"); }
unsigned long RainDrop::show(CRGB color, unsigned long wait) {
  clearStrip(false);
  int i = chaseState;
  CRGB c;
  setIfPresent(i - 6, CRGB::Black);
  c = color;
  c /= 32;
  setIfPresent(i - 5, c);
  c = color;
  c /= 16;
  setIfPresent(i - 4, c);
  c = color;
  c /= 8;
  setIfPresent(i - 3, c);
  c = color;
  c /= 4;
  setIfPresent(i - 2, c);
  c = color;
  c /= 2;
  setIfPresent(i - 1, c);
  setIfPresent(i, color);
  FastLED.show();

  chaseState = chaseState + 1;
  if (chaseState == NUM_LEDS)
    chaseState = 0;

  return wait / NUM_LEDS;
}

void OnOffLights::print() { Serial.println("OnOffLights"); }
unsigned long OnOffLights::show(CRGB color, unsigned long wait) {
  int i;
  if (isOn) {
    clearStrip(false);
    for (i = 0; i < NUM_LEDS; i++) {
      leds[i] = color;
      FastLED.show();
      delay(wait / NUM_LEDS);
    }
    isOn = false;
    return wait / 2;
  }
  for (i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(wait / NUM_LEDS);
  }
  isOn = true;
  return wait / 2;
}

void FadeLights::print() { Serial.println("FadeLights"); }
unsigned long FadeLights::show(CRGB color, unsigned long wait) {
  CRGB c = color;
  uint8_t factor;

  if (fadeState < transitions) {
    factor = step * fadeState;
  } else if (fadeState == transitions) {
    factor = 255;
  } else {
    factor = 256 - ((fadeState - step) * step);
  }
  c.subtractFromRGB(factor);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = c;
  }
  FastLED.show();
  fadeState++;
  if (fadeState == transitions * 2)
    fadeState = 0;
  return wait / (unsigned long)transitions / 2;
}

void AlternateLights::print() { Serial.println("AlternateLights"); }
unsigned long AlternateLights::show(CRGB color, unsigned long wait) {
  bool initial = state;
  for (int i = 0; i < NUM_LEDS; i++) {
    if (state) {
      leds[i] = color;
    } else {
      leds[i] = CRGB::Black;
    }
    state ^= true;
  }
  FastLED.show();
  state = initial ^ 1;
  return wait / 2;
}

void Blink::print() { Serial.println("Blink"); }
unsigned long Blink::show(CRGB color, unsigned long wait) {
  CRGB c = color;
  if (!state) {
    c = CRGB::Black;
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = c;
  }
  FastLED.show();
  state ^= true;
  return wait / 2;
}

void Twinkle::print() { Serial.println("Twinkle"); }
unsigned long Twinkle::show(CRGB color, unsigned long wait) {
  CRGB c = color;
  clearStrip(false);
  int led = random(0, NUM_LEDS);
  leds[led] = c;
  for (int i = 16; i > 0; i--) {
    leds[led].subtractFromRGB(i * 16);
    FastLED.show();
    delay(2);
  }
  leds[led] = c;
  FastLED.show();
  for (int i = 0; i < 16; i++) {
    leds[led].subtractFromRGB(i * 16);
    FastLED.show();
    delay(5);
  }
  leds[led] = CRGB::Black;
  FastLED.show();
  return wait;
}

void Rainbow::print() { Serial.println("Rainbow"); }
unsigned long Rainbow::show(CRGB color, unsigned long wait) {
  int current = state;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(rainbow_r[current], rainbow_g[current], rainbow_b[current]);
    current++;
    if (current == NUM_COLORS)
      current = 0;
  }
  FastLED.show();
  state = state + 1;
  if (state == NUM_COLORS)
    state = 0;
  return (wait / NUM_COLORS) * 2;
}

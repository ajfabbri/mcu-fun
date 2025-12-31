#include <Arduino.h>
#include <SerialPrintf.h>
#include <RainbowSequence.h>
#include <FastLED.h>
#include "Bouncy.h"

// Color constants
static const unsigned int COLOR_OFFSET = 45;
static const float COLOR_RATE = 0.001;
static const float COLOR_DECAY = 0.90;

// LED constants
static const unsigned int NUM_LEDS = 160;
// GPIO 3?
static const unsigned int LED_PIN = 3;
//static const unsigned int LED_PIN = 9;
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

// Main data structures
CRGB leds[NUM_LEDS];
Rainbow rb(COLOR_OFFSET, COLOR_RATE);
Bouncy bouncy(leds, NUM_LEDS, COLOR_DECAY);

void setup() {
  Serial.begin(115200);
  delay(10);
  ser_printf("Hello!\n");
  delay(1000);
  ser_printf("Rainbow: color offset %d, rate %0.3f.\n",
        COLOR_OFFSET, COLOR_RATE);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(90);
  ser_printf("FastLED type WS2812B, pin %d, num_leds %d\n", LED_PIN, NUM_LEDS);
  delay(1000);
}

void loop() {
  Rgb c = rb.next();
  //for (int i = 0; i < NUM_LEDS; i++) {
    // night time, less blue
    /*
    c.b = c.b / 10;
    c.g = c.g / 2;
    */
    //leds[i] = TO_CRGB(c);
    /*
    leds[i].r = random(255);
    leds[i].g = random(255);
    leds[i].b = random(255);
    */
    /*
    if (i == 0) {
      ser_printf("LED %d -> %d, %d, %d\n", i, c.r, c.g, c.b);
    }
    */
    //delay(10);
    // }
  bouncy.render_color(TO_CRGB(c));
  FastLED.show();
  //delay(2);
}

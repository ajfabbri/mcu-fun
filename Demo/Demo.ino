#include <Arduino.h>
#include <SerialPrintf.h>
#include <RainbowSequence.h>
#include <FastLED.h>
#include "Bouncy.h"
#include "Screen.h"

// Color constants
static const unsigned int COLOR_OFFSET = 45;
static const float COLOR_RATE = 0.10;
static const float COLOR_DECAY = 0.94;

// LED constants
static const unsigned int NUM_LEDS = 160;
static const uint8_t BRIGHTNESS = 255;  // out of 255
// GPIO 3
static const unsigned int LED_PIN = 3;
//static const unsigned int LED_PIN = 9;
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

// Main data structures
CRGB leds[NUM_LEDS];
Rainbow rb(COLOR_OFFSET, COLOR_RATE);
Bouncy bouncy(leds, NUM_LEDS, COLOR_DECAY);
Screen screen;

void update_screen();

void setup() {
  Serial.begin(115200);
  yield();
  ser_printf("Hello!\n");
  ser_printf("Rainbow: color offset %d, rate %0.3f.\n",
        COLOR_OFFSET, COLOR_RATE);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  ser_printf("FastLED type WS2812B, pin %d, num_leds %d\n", LED_PIN, NUM_LEDS);
  yield();
  update_screen();
  yield();
}

static char buf1[32], buf2[32];
void update_screen() {
    unsigned long now = millis();
    snprintf(buf1, sizeof(buf1), " ~ BOUNCY COLORS ~");
    snprintf(buf2, sizeof(buf2), "color offs=%u, rate %.3f", COLOR_OFFSET, COLOR_RATE);
    screen.render_text(buf1, buf2);
    ser_printf("Took %lu msec to update screen\n", millis() - now);
}

CRGB warmer(CRGB in) {
  CRGB out;
  out.r = in.r;
  out.g = in.g;
  out.b = in.b - (in.b / 4);  // reduce blue by 25%
  return out;
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
  bouncy.render_color(warmer(TO_CRGB(c)));
  FastLED.show();
  yield();
  // Updating screen is slow enough that it messes up the bounce simulation
  //update_screen();
}

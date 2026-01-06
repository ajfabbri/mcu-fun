#include <Arduino.h>
#include <SerialPrintf.h>
#include <RainbowSequence.h>
#include <FastLED.h>
#include "Bouncy.h"
#include "Screen.h"
#include "Net.h"

// Color constants
static const unsigned int COLOR_OFFSET = 45;
static const float COLOR_RATE = 0.10;
static const float COLOR_DECAY = 0.94;

// LED constants
static const uint8_t BRIGHTNESS = 255;  // out of 255
// ESP32-S3 setup
static const unsigned int LED_PIN = 9;
static const unsigned int NUM_LEDS = 60;

// ESP32-C6 Dev Kit
//static const unsigned int LED_PIN = 3;  // e.g. 3 = GPIO 3
//static const unsigned int NUM_LEDS = 160;

//#define LED_TYPE WS2812B
//#define COLOR_ORDER GRB
//
#define LED_TYPE WS2811
#define COLOR_ORDER RGB

// Main data structures
CRGB leds[NUM_LEDS];
Rainbow rb(COLOR_OFFSET, COLOR_RATE);
Bouncy bouncy(leds, NUM_LEDS, COLOR_DECAY);
Screen screen;
Net net;

void update_screen();

void setup() {
  Serial.begin(9600);
  for (int i = 20; i > 0; i--) {
    delay(200);
    ser_printf("Countdown %d!\n", i);
  }

  delay(200);
  ser_printf("Hello!\n");
  ser_printf("Rainbow: color offset %d, rate %0.3f.\n",
        COLOR_OFFSET, COLOR_RATE);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  ser_printf("FastLED type WS2812B, pin %d, num_leds %d\n", LED_PIN, NUM_LEDS);
  yield();
  update_screen();
  yield();
  net.init();
  net.start();
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
#if 0
  CRGB out;
  out.r = in.r;
  out.g = in.g;
  out.b = in.b - (in.b / 4);  // reduce blue by 25%
  return out;
#else
  return in;
#endif
}

void process_net() {
    byte buf[512];
    size_t received = 0;
    if (net.poll(buf, sizeof(buf), received)) {
        ser_printf("Received %d bytes\n", received);
        // process packet here
        for (size_t i = 0; i < received; i++) {
            Serial.print((char)buf[i]);
        }
    }
}

enum {
  MODE_RAINBOW,
  MODE_BOUNCY,
  MODE_HOLIDAY,
  MODE_TEST
//} mode = MODE_BOUNCY;
} mode = MODE_TEST;

void loop () {
  switch (mode) {
    case MODE_RAINBOW:
      rb_loop();
      break;
    case MODE_BOUNCY:
      bouncy_loop();
      break;
    case MODE_HOLIDAY:
      test_loop();
      break;
    case MODE_TEST:
      test_loop();
      break;
  }
  yield();
  process_net();
  Serial.println("loop..");
  delay(1);
}

void test_loop() {
    delay(1000);
    yield();
    delay(1000);
    Serial0.println("serial0 test");
    Serial1.println("serial1 test");
}

static const int FADE_IN_COUNT = 20;
static const int FADE_OUT_COUNT = 60;
static const CRGB green = CRGB(0, 255, 0);
static const CRGB red = CRGB(255, 0, 0);

// to get non-const copies to modify
static CRGB grn() {
    return green;
}
static CRGB rd() {
    return red;
}

static int fade_count = -FADE_IN_COUNT;

static bool is_green(CRGB c) {
    return (c.g > c.r) && (c.g > c.b);
}

void holiday_loop() {

    // Fading in..
    if (fade_count <= 0) {
        if (fade_count == -FADE_IN_COUNT) {
            // begin fade in: assign colors to LEDs
            for (int i = 0; i < NUM_LEDS; i++) {
                int coin = random(2);
                if (coin == 0) {
                    leds[i] = green;
                } else {
                    leds[i] = red;
                }
            }
        } else {
            // fade in color
            int amount = -(fade_count * 255) / FADE_IN_COUNT;
            for (int i = 0; i < NUM_LEDS; i++) {
                if (amount > 0) {
                    if (is_green(leds[i])) {
                        leds[i] = grn().fadeToBlackBy(amount);
                    } else {
                        leds[i] = rd().fadeToBlackBy(amount);
                    }
                }
            }
        }
    // Fading out
    } else if (fade_count < FADE_OUT_COUNT) {
        for (int i = 0; i < NUM_LEDS; i++) {
            int amount = (fade_count * 255) / FADE_OUT_COUNT;
            leds[i] = leds[i].fadeToBlackBy(amount);
        }
    } else if (fade_count >= FADE_OUT_COUNT) {
        fade_count = -FADE_IN_COUNT - 1;
    }
    FastLED.show();
    fade_count++;
    delay(100);
}

static const int MID_LED = NUM_LEDS / 2;
static int mid_offset = 0;
static int direction = 1; // 1 = out, -1 in
// odd number of lights use same center led
// even number of lights will start with center of two leds.
static int l_led = MID_LED;
static int r_led = NUM_LEDS - MID_LED;

void rb_loop() {
  // colors move from center out, then from edges back to center.
  // we set color of two leds at a time, chosen based on direction of movement.
  if (direction > 0) {
    // moving out
    mid_offset++;
    if (mid_offset > MID_LED) {
      direction = -1;
      mid_offset = MID_LED;
    }
  }
  if (direction < 0) {
    mid_offset--;
    if (mid_offset < 0) {
      direction = 1;
      mid_offset = 0;
    }
  }

  l_led = MID_LED - mid_offset;
  r_led = MID_LED + mid_offset;

  // update led color values
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i == l_led || i == r_led) {
      Rgb c = rb.next();
      leds[i] = warmer(TO_CRGB(c));
    } else {
      // fade out other leds
      leds[i].r = (leds[i].r * 3) / 10;
      leds[i].g = (leds[i].g * 9) / 10;
      leds[i].b = (leds[i].b * 3) / 10;
    }
  }
  FastLED.show();
  delay(100);
}

void bouncy_loop() {
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

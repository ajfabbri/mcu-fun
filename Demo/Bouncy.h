#ifndef _BOUNCY_H_     // This "ifdef guard" prevents this file
#define _BOUNCY_H_     // from being included more than once.

#include <cstdint>
#include <math.h>
#include <FastLED.h>
#include <crgb.h>


// Tunable constants. TODO make these configurable at runtime?
const float GRAVITY = 9.8;  // m/s^2
const float BOUNCE_ENERGY_LOSS = 0.15;
const float WIND_RESISTANCE = 0.001;
const CRGB BASE_COLOR = CRGB::Red;
const float DECAY_FACTOR = 0.5;
// Z-axis of bounce sim is based on 100 m height.
// Use a value < 1 to model a smaller height.
const float LENGTH_SCALE = 0.1;
const unsigned int STALL_MSEC = 1500;

class Bouncy {
  private:
    // parameters
    CRGB color;

    CRGB* leds;
    unsigned int num_leds;
    float color_decay;

    // state
    unsigned long last_msec;
    // velocity; negative is down, positive is up
    float velocity;
    // in [0.0, 1.0]
    float position;
    unsigned long stalled_since;

    void write_leds(CRGB c);
    CRGB decay_color(CRGB c, float factor);

  public:
    /**
     * Create physics animation LED strip renderer.
     * Takes a pointer to your array of FastLED CRGB values,
     * which it will modify when render() is called.
     * (Could use std::array but I'm avoiding C++)
     */
    Bouncy(CRGB* leds, unsigned int num_leds, float color_decay);

    void render();
    void render_color(CRGB color);
};

#endif

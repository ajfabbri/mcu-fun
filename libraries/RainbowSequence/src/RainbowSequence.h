#ifndef _RAINBOWSEQUENCE_H_     // This "ifdef guard" prevents this file
#define _RAINBOWSEQUENCE_H_     // from being included more than once.

#include <cstdint>
#include <math.h>

// A little "library" for generating rainbow colors


struct Rgb; // defined below..

class Rainbow {
private:
  // parameters
  float color_offset_rad;
  float rate;

  // current base color
  uint8_t r;
  uint8_t g;
  uint8_t b;

  // current x index (for sine function) base, in 1000'ths of a degree.
  // Instead of using an integer with these weird units, we could use a floating point number.
  // I used an unsigned int because they automatically wrap on overflow. floats do not.
  unsigned int cur_x_millidegree;

    /**
     * Initialize a rainbow color sequence generator.
     * `color_offset_deg` is degrees in separation between R, G, and B colors.
     * `rate` determines how fast the colors change.
     */
public:
  Rainbow(unsigned int color_offset_deg, float rate);

  Rgb next();

};

struct Rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

// Helper macro for converting rbg into FastLED CRGB type
#define TO_CRGB(c) CRGB((c).r, (c).g, (c).b)

#endif

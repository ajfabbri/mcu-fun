#import "RainbowSequence.h"

// `static` here means this helper function is only visible in this
// file (can't be imported to others)
static float deg_to_rad(float deg) {
  float r = deg * M_PI / 180.0;
  //ser_printf("deg_to_rad %f -> %f\n", deg, r);
  return r;
}

// Like sin(), but instead of range being [-1, 1], it is [0, 255].
static uint8_t sin_8bit(float x) {
  float val = sin(x) + 1;  // in [0, 2]
  return (uint8_t)(val * 128.0);
}

/**
   Initialize a rainbow color sequence generator.
   `color_offset_deg` is degrees in separation between R, G, and B colors.
   `rate` determines how fast the colors change.
*/
Rainbow::Rainbow(unsigned int color_offset_deg, float rate) {
  color_offset_rad = deg_to_rad(color_offset_deg);
  r = 0;
  g = color_offset_rad;
  b = color_offset_rad * 2;
  cur_x_millidegree = 0;
  this->rate = rate;
}

#include <Arduino.h>
Rgb Rainbow::next() {
  float x_deg = (float)cur_x_millidegree / 1000.0;
  float x_rad = deg_to_rad(x_deg);
  r = sin_8bit(x_rad);
  //ser_printf("rb: deg %f, rad %r, sin_8bit %f\n", x_deg, x_rad, r);
  g = sin_8bit(x_rad + color_offset_rad);
  b = sin_8bit(x_rad + (2 * color_offset_rad));
  float x_increment = rate * 1000.0;
  cur_x_millidegree += (unsigned int)(x_increment);
  return Rgb{r, g, b};
}

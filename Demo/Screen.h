
#ifndef _SCREEN_H_     // This "ifdef guard" prevents this file
#define _SCREEN_H_     // from being included more than once.

#include <U8g2lib.h>


// SD1306 OLED screen
const unsigned int SCREEN_WIDTH = 128;
const unsigned int SCREEN_HEIGHT = 64;
// These are probably defaults, but listing for clarity..
const unsigned int CLOCK_PIN = 7;  // GPIO7 on ESP32-C6 Dev Kit
const unsigned int DATA_PIN = 6;   // GPIO6

class Screen {
  private:
    U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2;
    bool initialized = false;

    void lazy_init();

  public:
    Screen();

    void render_text(const char* line1, const char* line2);

};

#endif

#include <SerialPrintf.h>
#include "Screen.h"

#undef WANT_I2C_SCAN

#include <Wire.h>

// Using defaults. We have ESP32-C6 Dev Kit with DA/CL on GPIO6/7
Screen::Screen() : u8g2(U8G2_R0,
        U8X8_PIN_NONE, // reset
        CLOCK_PIN,
        DATA_PIN) {}

void Screen::lazy_init() {
    if (initialized) {
        return;
    }
    Wire.setClock(1000000); // 1 MHz
    u8g2.begin();
#ifdef WANT_I2C_SCAN
    Wire.begin(CLOCK_PIN, DATA_PIN);
    byte error, addr;
    for (addr = 1; addr < 127; addr++ ) {
        Wire.beginTransmission(addr);
        error = Wire.endTransmission();
        if (error == 0) {
            ser_printf("I2C device found at address 0x%02X\n", addr);
        }
    }
#endif
    initialized = true;
    ser_printf("Screen::lazy_init() finished.\n");
}

void Screen::render_text(const char* line1, const char* line2) {
    lazy_init();
    yield();
    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(0, 12, line1);
        u8g2.drawStr(0, 30, line2);
    } while (u8g2.nextPage());
    yield();
/* full page mode (slow)
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 12, line1);
    u8g2.drawStr(0, 30, line2);
    u8g2.sendBuffer();
    */
}


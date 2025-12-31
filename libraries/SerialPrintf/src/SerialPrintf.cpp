#import "SerialPrintf.h"
#import <Arduino.h>
#import <stdarg.h>

char buffer[128];  // adjust max size as needed
void ser_printf(const char *format, ...) {

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    Serial.print(buffer);
}

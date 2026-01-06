#include <Arduino.h>


static unsigned long count = 0;

static void blink_fast() {
    // blink onboard led for ESP32-S3 DevKit
    if (count % 10 == 0) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(10);
        digitalWrite(LED_BUILTIN, LOW);
    } else {
        delay(100);
    }
    count++;
}

static void blink_slow() {
    // blink onboard led for ESP32-S3 DevKit
    if (count % 10 < 4) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(125);
        digitalWrite(LED_BUILTIN, LOW);
        delay(125);
    }
    count++;
}


void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    delay(1000);
    Serial.begin(115200);
    Serial1.begin(115200);
    while(!Serial || !Serial1){
        blink_fast();
        delay(10);
    }
    yield();
    Serial.println("Hello, Arduino!");
    Serial1.println("Hello, Arduino! 1");
}

void loop() {
    Serial.println("looping..");
    Serial1.println("looping 1..");
    // blink onboard led for ESP32-S3 DevKit
    blink_slow();
    delay(50);
}

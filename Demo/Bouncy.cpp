#include <Arduino.h>
#include <algorithm>
#include <FastLED.h>
#include "Bouncy.h"

Bouncy::Bouncy(CRGB* leds, unsigned int num_leds, float color_decay) {
    this->leds = leds;
    this->num_leds = num_leds;
    this->velocity = 0.0;
    this->position = 1.0;
    this->stalled_since = 0;
    this->last_msec = 0;    // zero until timing starts at first render()
    this->color = BASE_COLOR;
    this->color_decay = color_decay;
}

// Update LEDs for current time according to "falling ball" animation
void Bouncy::render() {
    render_color(this->color);
}

void Bouncy::render_color(CRGB color) {
    unsigned long delta_t = 0;
    // How much time elasped?
    if (last_msec == 0) {
        last_msec = millis();

    } else {
        delta_t = millis() - last_msec;
    }
    float delta_t_sec = (float)delta_t / 1000.0;

    // Update velocity
    if (delta_t > 0) {
        // Acceleration due to gravity: 9.8 m/s^2
        float g = 9.8;
        velocity = velocity - (1.0 / LENGTH_SCALE) *
            g * delta_t_sec * delta_t_sec;
        last_msec += delta_t;
        // Light wind resistance
        float vel_loss_factor = WIND_RESISTANCE *
            velocity * velocity * delta_t_sec;
        velocity *= (1.0 - vel_loss_factor);
    }

    // Update position of ball
    position = position + velocity * delta_t_sec;

    // Detect ground
    if (position <= 0.0) {
        position = 0.0;
        // Reverse, but lose some energy on bounce
        velocity = -velocity * (1.0 - BOUNCE_ENERGY_LOSS);
    }
    // Detect stopped bouncing
    if (position < 0.0001) {
        if (stalled_since == 0) {
            stalled_since = millis();
        } else {
            unsigned long stalled_time = millis() - stalled_since;
            if (stalled_time > STALL_MSEC) {
                // Reset to top
                position = 1.0;
                velocity = 0.0;
                stalled_since = 0;
            }
        }
    } else {
        stalled_since = 0;
    }

    write_leds(color);
}

void Bouncy::write_leds(CRGB color) {
    unsigned int led_index = (unsigned int)(position * (float)(num_leds - 1));
    led_index = std::min(led_index, num_leds - 1);
    led_index = std::max(led_index, 0u);
    for (int i = 0; i < num_leds; i++) {
        if (i == led_index) {
            leds[i] = color;
        } else {
            leds[i] = decay_color(leds[i], color_decay);
        }
    }
}

static const CRGB FLOOR_COLOR = CRGB(2, 0, 0);
CRGB Bouncy::decay_color(CRGB c, float factor) {

#if 1
    return blend(c, FLOOR_COLOR, (uint8_t)((1.0 - factor) * 255));
#else

    return CRGB(
        (uint8_t)((float)c.r * factor),
        (uint8_t)((float)c.g * factor),
        (uint8_t)((float)c.b * factor)
    );
#endif
}

float Bouncy::get_position() {
    return position;
}

float Bouncy::get_velocity() {
    return velocity;
}


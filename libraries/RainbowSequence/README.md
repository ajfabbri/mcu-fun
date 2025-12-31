# RainbowSequence: Arduino color sequence generator.

Generates a sequence of RGB colors that evolve like a rainbow.

Can be used with FastLED to implement rainbow-like colors and movement.

## FAQ

### How to use with FastLED?

This library doesn't depend on FastLED directly, but you can use the
`TO_CRGB()` macro to convert from our `struct rgb` to FastLED's `CRGB` value.

See the [header file](./src/RainbowSequence.h) for details.

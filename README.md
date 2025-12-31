# Microcontroller (MCU) Fun

## Demo
Sketch that animates an array of addressable LEDs. Also shows how to use the
RainbowSequence and SerialPrintf libraries included in this repo.

Currently includes rainbow color movement as well as a bouncing ball animation.
The bouncing ball animation has tunable bounce energy conservation, scale, and
wind resistance constants.

## Arduino Libraries

### RainbowSequence

Uses three out-of-phase sine waves for R, G, and B colors to generate smooth
rainbow like effects as a sequence of color values.

### SerialPrintf

Format strings al la `printf` and write to serial output, in one handy function.

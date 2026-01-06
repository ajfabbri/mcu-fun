#!/usr/bin/env bash
set -e

BOARD="esp32:esp32:esp32s3"
BUILD="build"

set -x
arduino-cli compile \
  --fqbn "$BOARD" \
  --build-path "$BUILD/$BOARD" \
  $@
set +x
#  --build-property build.cdc_on_boot=0 \
#  --build-property build.usb_mode=0 \
# --verbose 2>&1| compiledb -o compile_commands.json

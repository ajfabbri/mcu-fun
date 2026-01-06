#!/usr/bin/env bash
set -e

target=$1
BOARD="esp32:esp32:esp32s3"
BUILD="build"

arduino-cli upload \
  -p /dev/cu.usbmodem* \
  --fqbn "$BOARD" \
  --build-path "$BUILD/$BOARD" \
  $target

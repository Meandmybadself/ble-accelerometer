# Bluetooth Low Energy Accelerometer

## Overview

Using an [Adafruit Feather 32u4 Bluefruit](https://www.adafruit.com/product/3406) to relay the measurements from two [ADXL335 - 5V ready triple-axis accelerometers](https://www.adafruit.com/product/163).

## Prerequisites

- Install the [Adafruit Bluefruit nRF52 BSP (Board Support Package)](https://learn.adafruit.com/bluefruit-nrf52-feather-learning-guide/arduino-bsp-setup)
- Connect the pins as specified in sketch.

## Output

The device will output notify messages for each sensor at the delay specified in the loop of `ble-accelerometer.ino`.

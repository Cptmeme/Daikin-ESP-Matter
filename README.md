Work in progress

ESP32 project to get matter to work with all Daikin A/C with a S21 port.

Code copied or inspiration taken from:

-https://github.com/espressif/esp-matter (copied example code for A/C)

-https://github.com/revk/ESP32-Faikin (taken s21 protocol files)

-https://github.com/asund/esphome-daikin-s21 (inspiration)

Custom PCB:
Based on an esp32c6-mini-1 to allow for thread support, has been verified to work in esphome using asund's esphome-daikin-s21 code.

WARNING!:
Do not connect the pcb to the A/C and a PC over the usb-c port at the same time as this will short out your usb port!

Main:
Main code for a matter thermostat example, can be used by taking the official A/C example from the esp-matter repository and replacing the main files with the ones in this repository

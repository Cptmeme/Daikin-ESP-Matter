//S21Utils.h
#pragma once

#include "ThermostatMode.h"
#include <math.h>

// Calculate packet checksum
static inline uint8_t s21_checksum(uint8_t* buf, int len) {
    uint8_t c = 0;

    // The checksum excludes STX, checksum field itself, and ETX
    for (int i = 1; i < len - 2; i++)
        c += buf[i];

    // Special bytes are forbidden even as checksum bytes, they are promoted
    if (c == STX || c == ETX || c == ACK)
        c += 2;

    return c;
}

// Target temperature is encoded as one character
static inline float s21_decode_target_temp(unsigned char v) {
    return 18.0 + 0.5 * ((signed)v - AC_MIN_TEMP_VALUE);
}

static inline float s21_encode_target_temp(float temp) {
    return lroundf((temp - 18.0) * 2) + AC_MIN_TEMP_VALUE;
}

static inline int s21_decode_int_sensor(const unsigned char* payload) {
    int v = (payload[0] - '0') + (payload[1] - '0') * 10 + (payload[2] - '0') * 100;
    if (payload[3] == '-')
        v = -v;
    return v;
}

static inline uint16_t s21_decode_hex_sensor(const unsigned char* payload) {
#define hex(c) (((c)&0xF)+((c)>'9'?9:0))
    return (hex(payload[3]) << 12) | (hex(payload[2]) << 8) | (hex(payload[1]) << 4) | hex(payload[0]);
#undef hex
}

static inline float s21_decode_float_sensor(const unsigned char* payload) {
    return (float)s21_decode_int_sensor(payload) * 0.1;
}

// Convert between Daikin and Faikin fan speed enums
static inline unsigned char s21_encode_fan(int speed) {
    return convertFanSpeedToAC(speed);
}

static inline int s21_decode_fan(unsigned char v) {
    return convertACToFanSpeed(v);
} 
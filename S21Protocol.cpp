#include "S21Protocol.h"
#include <cstdint>
#include <cstddef>
#include <cstring>             // for memcpy
#include "driver/uart.h"       // for uart_*
#include "freertos/FreeRTOS.h" // for pdMS_TO_TICKS
#include "S21Utils.h"          // for s21_checksum

S21Protocol::S21Protocol(uart_port_t uart)
    : uart_num(uart),
      protocolVersion(S21ProtocolVersion::UNKNOWN),
      features(),
      isInitialized(false)
{}

bool S21Protocol::begin() {
    isInitialized = true;
    return true;
}

bool S21Protocol::sendCommand(char cmd0, char cmd1, const uint8_t* payload, size_t len) {
    uint8_t packet[16] = {0}; // adjust size if needed
    size_t pktLen = S21_MIN_PKT_LEN + len;

    if (pktLen > sizeof(packet)) return false;

    packet[S21_STX_OFFSET] = STX;
    packet[S21_CMD0_OFFSET] = cmd0;
    packet[S21_CMD1_OFFSET] = cmd1;

    for (size_t i = 0; i < len; ++i) {
        packet[S21_PAYLOAD_OFFSET + i] = payload[i];
    }

    packet[pktLen - 2] = s21_checksum(packet, pktLen);
    packet[pktLen - 1] = ETX;

    int written = uart_write_bytes(uart_num, (const char*)packet, pktLen);
    return (written == pktLen);
}

bool S21Protocol::parseResponse(uint8_t& cmd0, uint8_t& cmd1, uint8_t* payload, size_t& payloadLen) {
    uint8_t buffer[32] = {0};
    int len = uart_read_bytes(uart_num, buffer, sizeof(buffer), pdMS_TO_TICKS(200));

    if (len < S21_MIN_PKT_LEN) return false;
    if (buffer[0] != STX || buffer[len - 1] != ETX) return false;

    uint8_t crc = buffer[len - 2];
    if (crc != s21_checksum(buffer, len)) return false;

    cmd0 = buffer[S21_CMD0_OFFSET];
    cmd1 = buffer[S21_CMD1_OFFSET];

    payloadLen = len - S21_MIN_PKT_LEN;
    if (payloadLen > 0 && payload) {
        memcpy(payload, &buffer[S21_PAYLOAD_OFFSET], payloadLen);
    }

    return true;
}

bool S21Protocol::isFeatureSupported(const S21Features&) const {
    return true;
}

bool S21Protocol::isCommandSupported(char, char) const {
    return true;
}
//ThermostatController.cpp
#include "ThermostatController.h"

ThermostatController::ThermostatController(S21Protocol& p) : protocol(p), power(false), mode(AC_MODE_AUTO), targetTemperature(24.0), currentTemperature(24.0), fanSpeed(FAN_AUTO), consecutiveErrors(0), lastUpdateTime(0) {}

bool ThermostatController::setPower(bool on) { power = on; return true; }

bool ThermostatController::setTargetMode(uint8_t newMode) {
    mode = convertHomeKitToACMode(newMode);
    return true;
}

bool ThermostatController::setTargetTemperature(float temperature) {
    targetTemperature = temperature;
    return true;
}

void ThermostatController::update() {
    currentTemperature = targetTemperature; // dummy update
}
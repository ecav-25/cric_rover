// ============================================================================
// ControllerManager.cpp 
// ============================================================================

#include <Arduino.h>
#include "ControllerManager.h"

ControllerManager::ControllerManager() 
    : battery(0), alive(0), lastRxMillis(0) {
    setNeutral();
}

void ControllerManager::setNeutral() {
    portENTER_CRITICAL(&mutex);
    memset(&data, 0, sizeof(ControllerData));
    data.ax = 255;
    data.ay = 255;
    data.bx = 255;
    data.by = 255;
    battery = 0;
    alive = 0;
    portEXIT_CRITICAL(&mutex);
}

void ControllerManager::updateFromPacket(const ControllerData& ctrl, uint8_t batt) {
    portENTER_CRITICAL(&mutex);
    data = ctrl;
    battery = batt;
    alive = 1;
    lastRxMillis = millis();
    portEXIT_CRITICAL(&mutex);
}

void ControllerManager::getSnapshot(ControllerData& out_data, uint8_t& out_battery, uint8_t& out_alive) {
    portENTER_CRITICAL(&mutex);
    out_data = data;
    out_battery = battery;
    out_alive = alive;
    portEXIT_CRITICAL(&mutex);
}

bool ControllerManager::checkTimeout(uint32_t timeout_ms) {
    portENTER_CRITICAL(&mutex);
    bool timed_out = (alive == 1) && ((millis() - lastRxMillis) > timeout_ms);
    
    if (timed_out) {
        alive = 0;
        setNeutral();
    }
    portEXIT_CRITICAL(&mutex);
    
    return timed_out;
}

uint8_t ControllerManager::isAlive() const {
    return alive;
}

uint8_t ControllerManager::getBattery() const {
    return battery;
}
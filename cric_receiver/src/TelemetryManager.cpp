// ============================================================================
// TelemetryManager.cpp 
// ============================================================================

#include <Arduino.h>
#include "TelemetryManager.h"

TelemetryManager::TelemetryManager() : events(nullptr) {
    memset(&data, 0, sizeof(RoverTelemetry));
    events = xEventGroupCreate();
    
    if (!events) {
        Serial.println("[TELEMETRY] ERROR: Failed to create event group!");
    }
}

TelemetryManager::~TelemetryManager() {
    if (events) {
        vEventGroupDelete(events);
    }
}

void TelemetryManager::update(const RoverTelemetry& telemetry) {
    portENTER_CRITICAL(&mutex);
    memcpy(&data, &telemetry, sizeof(RoverTelemetry));
    portEXIT_CRITICAL(&mutex);
    
    if (events) {
        xEventGroupSetBits(events, DATA_READY_BIT);
    }
}

RoverTelemetry TelemetryManager::getSnapshot() {
    RoverTelemetry snapshot;
    portENTER_CRITICAL(&mutex);
    memcpy(&snapshot, &data, sizeof(RoverTelemetry));
    portEXIT_CRITICAL(&mutex);
    return snapshot;
}

void TelemetryManager::waitDataReady() {
    if (events) {
        xEventGroupWaitBits(events, DATA_READY_BIT, pdTRUE, pdTRUE, portMAX_DELAY);
    }
}

uint16_t TelemetryManager::getRPMFrontLeft() const {
    return data.rpm_fl;
}

uint16_t TelemetryManager::getRPMFrontRight() const {
    return data.rpm_fr;
}

uint8_t TelemetryManager::getBatteryPercent() const {
    return data.battery_percent;
}

uint16_t TelemetryManager::getSonarCenter() const {
    return data.sonar_c;
}
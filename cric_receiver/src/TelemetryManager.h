// ============================================================================
// TelemetryManager.h 
// ============================================================================

#ifndef TELEMETRY_MANAGER_H
#define TELEMETRY_MANAGER_H

#include "config.h"
#include "types.h"

class TelemetryManager {
private:
    RoverTelemetry data;
    portMUX_TYPE mutex = portMUX_INITIALIZER_UNLOCKED;
    EventGroupHandle_t events;

public:
    static constexpr int DATA_READY_BIT = (1 << 0);

    TelemetryManager();
    ~TelemetryManager();
    
    void update(const RoverTelemetry& telemetry);
    

    RoverTelemetry getSnapshot();

    void waitDataReady();
    

    uint16_t getRPMFrontLeft() const;
    uint16_t getRPMFrontRight() const;
    uint8_t getBatteryPercent() const;
    uint16_t getSonarCenter() const;
};

#endif // TELEMETRY_MANAGER_H
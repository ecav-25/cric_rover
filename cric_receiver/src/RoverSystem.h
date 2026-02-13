// ============================================================================
// RoverSystem.h 
// ============================================================================

#ifndef ROVER_SYSTEM_H
#define ROVER_SYSTEM_H

#include "config.h"
#include "ControllerManager.h"
#include "TelemetryManager.h"
#include "CommunicationManagers.h"

/**
 * Sistema principale che coordina tutti i componenti del rover
 * - Gestione controller wireless
 * - Ricezione telemetria via I2C
 * - Processing AI per rilevamento anomalie
 * - Trasmissione dati via UART
 */
class RoverSystem {
private:

    ControllerManager controller;
    TelemetryManager telemetry;
    

    I2CComm i2cComm;
    UARTComm uartComm;
    WirelessComm wirelessComm;
    
    TaskHandle_t aiTaskHandle = NULL;
    TaskHandle_t uartTaskHandle = NULL;

    bool isInitialized;
    unsigned long systemUptime;

public:
    RoverSystem();
    
    void initialize();
    

    void update();
    

    bool isSystemReady() const { return isInitialized; }

    void processSystemPipeline();
    

private:

    static void pipelineTaskWrapper(void* param);

    void printSystemStatus();
};

#endif // ROVER_SYSTEM_H
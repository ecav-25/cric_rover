// ============================================================================
// RoverSystem.h 
// ============================================================================

#ifndef ROVER_SYSTEM_H
#define ROVER_SYSTEM_H

#include "config.h"
#include "ControllerManager.h"
#include "TelemetryManager.h"
#include "AIEngine.h"
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
    AIEngine aiEngine;
    

    I2CComm i2cComm;
    UARTComm uartComm;
    WirelessComm wirelessComm;
    
    TaskHandle_t aiTaskHandle = NULL;
    TaskHandle_t uartTaskHandle = NULL;

    bool isInitialized;
    unsigned long systemUptime;

public:
    RoverSystem();
    
    /**
     * Inizializza tutti i sottosistemi
     */
    void initialize();
    
    /**
     * Aggiorna il sistema (da chiamare nel loop principale)
     * Esegue watchdog del controller
     */
    void update();
    
    /**
     * Legge stato del sistema
     */
    bool isSystemReady() const { return isInitialized; }
    
    /**
     * Task Pipeline di gestione delle telemetrie
     */
    void processSystemPipeline();
    

private:
    /**
     * Wrapper statici per FreeRTOS tasks
     */
    static void pipelineTaskWrapper(void* param);

    /**
     * Stampa diagnostic info
     */
    void printSystemStatus();
};

#endif // ROVER_SYSTEM_H
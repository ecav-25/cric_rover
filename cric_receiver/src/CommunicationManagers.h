// ============================================================================
// CommunicationManagers.h
// ============================================================================

#ifndef COMMUNICATION_MANAGERS_H
#define COMMUNICATION_MANAGERS_H

#include "config.h"
#include "types.h"
#include "ControllerManager.h"
#include "TelemetryManager.h"
#include "AIEngine.h"

// ============================================================================
// I2C COMMUNICATION MANAGER
// ============================================================================

class I2CComm {
private:
    ControllerManager& controllerMgr;
    TelemetryManager& telemetryMgr;

public:
    I2CComm(ControllerManager& c, TelemetryManager& t);
    
    /**
     * Inizializza comunicazione I2C come slave
     */
    void begin();
    
    /**
     * Callback quando master richiede dati
     */
    void onRequest();
    
    /**
     * Callback quando master invia dati
     */
    void onReceive(int len);

private:
    /**
     * Handler globali per Wire library
     */
    static I2CComm* instance;
    static void onRequestHandler();
    static void onReceiveHandler(int len);
};

// ============================================================================
// UART COMMUNICATION MANAGER
// ============================================================================

class UARTComm {
private:
    TelemetryManager& telemetryMgr;
    AIEngine& aiEngine;

public:
    UARTComm(TelemetryManager& t, AIEngine& a);
    
    /**
     * Inizializza seriale UART per telemetria
     */
    void begin();
    
    /**
     * Trasmette pacchetto telemetria completo via UART
     */
    void sendPacket(const FullTelemetry& packet);

private:
    
};

class WirelessComm {
private:
    ControllerManager& controllerMgr;

public:
    WirelessComm(ControllerManager& c);
    
    /**
     * Inizializza WiFi e ESP-NOW
     */
    void begin();
    void sendTelemetry(const FullTelemetry& packet);
    /**
     * Callback statica per ricezione dati ESP-NOW
     */
    static void onDataRecv(const uint8_t* mac, const uint8_t* data, int len);

private:
    static WirelessComm* instance;
};

#endif // COMMUNICATION_MANAGERS_H
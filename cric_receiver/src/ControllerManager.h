// ============================================================================
// ControllerManager.h 
// ============================================================================

#ifndef CONTROLLER_MANAGER_H
#define CONTROLLER_MANAGER_H

#include "config.h"
#include "types.h"

class ControllerManager {
private:
    ControllerData data;
    uint8_t battery;
    uint8_t alive;
    unsigned long lastRxMillis;
    portMUX_TYPE mutex = portMUX_INITIALIZER_UNLOCKED;

public:
    ControllerManager();
    
    /**
     * Imposta il controller nello stato neutro
     */
    void setNeutral();
    
    /**
     * Aggiorna il controller con dati ricevuti via ESP-NOW
     */
    void updateFromPacket(const ControllerData& ctrl, uint8_t batt);
    
    /**
     * Legge lo stato attuale del controller (thread-safe)
     */
    void getSnapshot(ControllerData& out_data, uint8_t& out_battery, uint8_t& out_alive);
    
    /**
     * Controlla se il controller ha superato il timeout
     * @return true se è scattato il timeout
     */
    bool checkTimeout(uint32_t timeout_ms);

    uint8_t isAlive() const;
    uint8_t getBattery() const;
};

#endif // CONTROLLER_MANAGER_H
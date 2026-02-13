// ============================================================================
// RoverSystem.cpp - IMPLEMENTAZIONE SISTEMA PRINCIPALE
// ============================================================================

#include <Arduino.h>
#include "RoverSystem.h"

RoverSystem::RoverSystem()
    : i2cComm(controller, telemetry),
      uartComm(telemetry),
      wirelessComm(controller),
      isInitialized(false),
      systemUptime(0) {}

void RoverSystem::initialize() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n" + String(80, '='));
    Serial.println("  C.R.I.C. ROVER  ");
    Serial.println(String(80, '='));


    Serial.println("[INIT] Initializing I2C interface...");
    i2cComm.begin();

    Serial.println("[INIT] Initializing UART telemetry...");
    uartComm.begin();


    Serial.println("[INIT] Initializing wireless communication...");
    wirelessComm.begin();

    Serial.println("[INIT] Creating FreeRTOS tasks...");
    
    xTaskCreatePinnedToCore(
        pipelineTaskWrapper,
        "PIPELINETask",
        TaskConfig::PIPELINE_TASK_STACK,
        this,
        TaskConfig::PIPELINE_TASK_PRIORITY,
        NULL,
        TaskConfig::PIPELINE_CORE
    );

    isInitialized = true;
    systemUptime = millis();
    
    Serial.println(String(80, '='));
    Serial.println("[INIT] ✓ All systems ready!");
    Serial.println(String(80, '=') + "\n");
    
    printSystemStatus();
}
void RoverSystem::processSystemPipeline() {
    while (true) {
        telemetry.waitDataReady();
        RoverTelemetry roverData = telemetry.getSnapshot();
        uartComm.sendPacket(roverData);
        wirelessComm.sendTelemetry(roverData);
    }
}


void RoverSystem::update() {

    if (controller.checkTimeout(HardwareConfig::CONTROLLER_TIMEOUT_MS)) {
        Serial.println("[WATCHDOG] Controller timeout! Setting neutral position.");
    }

    systemUptime = millis();
    
    delay(TaskConfig::WATCHDOG_DELAY_MS);
}


void RoverSystem::pipelineTaskWrapper(void* param) {
    ((RoverSystem*)param)->processSystemPipeline();
}


void RoverSystem::printSystemStatus() {
    Serial.println("\n--- SYSTEM CONFIGURATION ---");
    Serial.printf("Hardware Address (I2C): 0x%02X\n", HardwareConfig::I2C_DEV_ADDR);
    Serial.printf("UART Baudrate: %lu\n", HardwareConfig::UART_BAUDRATE);
    Serial.printf("Controller Timeout: %lu ms\n\n", 
                  HardwareConfig::CONTROLLER_TIMEOUT_MS);
}
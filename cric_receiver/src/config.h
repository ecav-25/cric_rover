#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>
#include "freertos/FreeRTOS.h"

// ============================================================================
// HARDWARE CONFIGURATION
// ============================================================================

namespace HardwareConfig {

    constexpr uint8_t I2C_DEV_ADDR = 0x55;
    constexpr uint32_t I2C_BAUDRATE = 100000;


    constexpr uint8_t WIFI_CHANNEL = 11;

    
    constexpr uint8_t UART_TX_PIN = 17;
    constexpr uint8_t UART_RX_PIN = 16;
    constexpr uint32_t UART_BAUDRATE = 115200;
    constexpr uint8_t CONTROLLER_MAC[] = { 0xC8, 0xF0, 0x9E, 0xF2, 0x6A, 0x24 };

    // Timeouts
    constexpr uint32_t CONTROLLER_TIMEOUT_MS = 1500;
}

// ============================================================================
// AI CONFIGURATION
// ============================================================================

namespace AIConfig {
    constexpr int TIME_STEPS = 8;
    constexpr int NUM_FEATURES = 8;
    constexpr int TOTAL_INPUTS = TIME_STEPS * NUM_FEATURES;

    constexpr float ANOMALY_THRESHOLD = 0.012f;
    constexpr int ALARM_TRIGGER_COUNT = 5;
    constexpr float MIN_RANGE = 0.0001f;


    constexpr int TENSOR_ARENA_SIZE = 200 * 1024;
}

// ============================================================================
// FREERTOS & MULTITASKING CONFIGURATION
// ============================================================================

namespace TaskConfig {
    constexpr UBaseType_t PIPELINE_TASK_PRIORITY = 2;
    constexpr UBaseType_t WATCHDOG_PRIORITY = 1;

    constexpr uint32_t PIPELINE_TASK_STACK = 16384;

    constexpr BaseType_t PIPELINE_CORE = 0;
    
    constexpr TickType_t WATCHDOG_DELAY_MS = 100;
}

// ============================================================================
// COMMUNICATION MAGIC BYTES
// ============================================================================

namespace CommProtocol {
    constexpr uint8_t UART_MAGIC_BYTE_1 = 0xCD;
    constexpr uint8_t UART_MAGIC_BYTE_2 = 0xAB;
}

// ============================================================================
// DEBUG & LOGGING CONFIGURATION
// ============================================================================

namespace DebugConfig {
    constexpr bool ENABLE_SERIAL_DEBUG = true;
    constexpr bool ENABLE_AI_DEBUG = false;
    constexpr bool ENABLE_COMM_DEBUG = true;
    constexpr bool ENABLE_TIMING_DEBUG = true;

    #if ENABLE_SERIAL_DEBUG
        #define DEBUG_PRINT(fmt, ...) Serial.printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
    #else
        #define DEBUG_PRINT(fmt, ...) 
    #endif

    #if ENABLE_AI_DEBUG
        #define AI_DEBUG(fmt, ...) Serial.printf("[AI] " fmt "\n", ##__VA_ARGS__)
    #else
        #define AI_DEBUG(fmt, ...) 
    #endif

    #if ENABLE_COMM_DEBUG
        #define COMM_DEBUG(fmt, ...) Serial.printf("[COMM] " fmt "\n", ##__VA_ARGS__)
    #else
        #define COMM_DEBUG(fmt, ...) 
    #endif
}


#endif // CONFIG_H
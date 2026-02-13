// ============================================================================
// CommunicationManagers.cpp 
// ============================================================================

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h> 
#include "esp_rom_crc.h"
#include <Wire.h>
#include "CommunicationManagers.h"

// ============================================================================
// I2C COMMUNICATION
// ============================================================================

I2CComm* I2CComm::instance = nullptr;

I2CComm::I2CComm(ControllerManager& c, TelemetryManager& t)
    : controllerMgr(c), telemetryMgr(t) {
    instance = this;
}

void I2CComm::begin() {
    Wire.begin((uint8_t)HardwareConfig::I2C_DEV_ADDR);
    Wire.onRequest(onRequestHandler);
    Wire.onReceive(onReceiveHandler);
    COMM_DEBUG("I2C initialized (addr=0x%02X)", HardwareConfig::I2C_DEV_ADDR);
}

void I2CComm::onRequestHandler() {
    if (instance) {
        instance->onRequest();
    }
}

void I2CComm::onRequest() {
    ControllerData data;
    uint8_t battery, alive;
    controllerMgr.getSnapshot(data, battery, alive);
    uint8_t crc=0;
    crc=esp_rom_crc8_le(crc,(uint8_t*)&data,sizeof(ControllerData));
    crc=esp_rom_crc8_le(crc,(uint8_t*)&battery,sizeof(uint8_t));
    crc=esp_rom_crc8_le(crc,(uint8_t*)&alive,sizeof(uint8_t));

    ControllerStatus packet = {alive, battery, data,crc};
    Wire.write((const uint8_t*)&packet, sizeof(ControllerStatus));
}

void I2CComm::onReceiveHandler(int len) {
    if (instance) {
        instance->onReceive(len);
    }
}

void I2CComm::onReceive(int len) {
    if (len < (int)sizeof(RoverTelemetry)) {
        COMM_DEBUG("I2C packet too small: %d bytes", len);
        while (Wire.available()) Wire.read();
        return;
    }

    RoverTelemetry temp;
    uint8_t* ptr = (uint8_t*)&temp;
    
    for (int i = 0; i < (int)sizeof(RoverTelemetry) && Wire.available(); i++) {
        *ptr++ = Wire.read();
    }
    
    while (Wire.available()) Wire.read();
    telemetryMgr.update(temp);
    DEBUG_PRINT("I2C received telemetry: sonar_c=%u, rpm_fl=%d", 
                temp.sonar_c, temp.rpm_fl);
}

// ============================================================================
// UART COMMUNICATION
// ============================================================================

UARTComm::UARTComm(TelemetryManager& t)
    : telemetryMgr(t) {}

void UARTComm::begin() {
    Serial2.begin(
        HardwareConfig::UART_BAUDRATE,
        SERIAL_8N1,
        HardwareConfig::UART_RX_PIN,
        HardwareConfig::UART_TX_PIN
    );
    COMM_DEBUG("UART initialized (baudrate=%lu)", HardwareConfig::UART_BAUDRATE);
}

void UARTComm::sendPacket(const RoverTelemetry& packet) {
    Serial2.write(CommProtocol::UART_MAGIC_BYTE_1);
    Serial2.write(CommProtocol::UART_MAGIC_BYTE_2);
    Serial2.write((const uint8_t*)&packet, sizeof(RoverTelemetry));
}


// ============================================================================
// WIRELESS COMMUNICATION (ESP-NOW)
// ============================================================================

WirelessComm* WirelessComm::instance = nullptr;

WirelessComm::WirelessComm(ControllerManager& c)
    : controllerMgr(c) {
    instance = this;
}

void WirelessComm::begin() {
    WiFi.mode(WIFI_STA);

    esp_wifi_set_promiscuous(true);
    esp_wifi_set_channel(HardwareConfig::WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);
    esp_wifi_set_promiscuous(false);

    if (esp_now_init() != ESP_OK) ESP.restart();

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, HardwareConfig::CONTROLLER_MAC, 6);
    peerInfo.channel = HardwareConfig::WIFI_CHANNEL;
    peerInfo.encrypt = false;
    
    esp_now_add_peer(&peerInfo);
    esp_now_register_recv_cb(onDataRecv); // Gira su Core 1
}

void WirelessComm::sendTelemetry(const RoverTelemetry& packet) {
    esp_now_send(HardwareConfig::CONTROLLER_MAC, (uint8_t*)&packet, sizeof(RoverTelemetry));
}

void WirelessComm::onDataRecv(const uint8_t* mac, const uint8_t* data, int len) {
    if (!instance) return;
    
    if (len != sizeof(ESPNowPacket)) {
        COMM_DEBUG("Invalid ESP-NOW packet size: %d", len);
        return;
    }

    ESPNowPacket packet;
    memcpy(&packet, data, sizeof(ESPNowPacket));
    
    instance->controllerMgr.updateFromPacket(packet.controller, packet.battery);
    
    DEBUG_PRINT("ESP-NOW received: ax=%u, ay=%u, batt=%d%%",
                packet.controller.ax, packet.controller.ay, packet.battery);
}
#include <Arduino.h>
#include "config.h"
#include "types.h"
#include "ControllerManager.h"
#include "TelemetryManager.h"
#include "CommunicationManagers.h"
#include "RoverSystem.h"


RoverSystem roverSystem;


void setup() {
    roverSystem.initialize();
}


void loop() {
    roverSystem.update();
}
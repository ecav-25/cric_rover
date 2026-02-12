classdef REAR_LED_TYPE < Simulink.IntEnumType
    enumeration
        IDLE             (0)
        BACKLIGHTS       (1)
        BRAKING_LIGHTS   (2)
        BACKWARD_LIGHTS  (3)
        ARROW_LEFT       (4)
        ARROW_RIGHT      (5)
        SPECIAL_LIGHTS   (6)
        EMERGENCY_LIGHTS (7)
        DEGRADED_LIGHTS  (8)
    end
end
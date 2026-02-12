classdef WORKING_STATUS_TYPE < Simulink.IntEnumType
    enumeration
        NORMAL_WORKING                (0)  
        CRITICAL_VOLTAGE_WORKING      (1)
        MOTOR_ERROR_WORKING           (2)
    end
end
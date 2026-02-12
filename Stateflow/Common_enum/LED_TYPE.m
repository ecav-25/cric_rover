classdef LED_TYPE < Simulink.IntEnumType
    enumeration
        OFF                 (0)  
        WHITE               (1)
        RED                 (2)
        BLINKING_WHITE      (3)
        BLINKING_RED        (4)
        BLINKING_RED_SLOW   (5)
    end
end
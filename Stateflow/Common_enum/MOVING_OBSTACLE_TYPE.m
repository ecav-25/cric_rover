classdef MOVING_OBSTACLE_TYPE < Simulink.IntEnumType
    enumeration
        NO_OBSTACLE         (0)
        MOVING_FROM_LEFT    (1)
        MOVING_FROM_RIGHT   (2)
        MOVING_FROM_BOTH    (3)
    end
end
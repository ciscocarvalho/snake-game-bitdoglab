#pragma once

#include "./types.h"

typedef struct JoystickInfo {
    // uint angle;
    Direction direction;
    uint max; 
    uint x_raw; 
    uint y_raw; 
    uint center; 
    float x_normalized; 
    float y_normalized; 
    uint raw_distance; 
    uint x_raw_distance; 
    uint y_raw_distance; 
    float normalized_distance;
    float x_normalized_distance; 
    float y_normalized_distance; 
} JoystickInfo;

void joystick_init();

JoystickInfo joystick_get_info();
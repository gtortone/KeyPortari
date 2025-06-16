#pragma once

#include "PS2Keymaps.h"
#include "global.h"

// 24 Char display 'special' key values
#define BACKSPACE_24CHAR   2
#define ENTER_24CHAR       3

#define LEFT_JOYSTICK_UP          0b11101111
#define LEFT_JOYSTICK_DOWN        0b11011111
#define LEFT_JOYSTICK_LEFT        0b10111111
#define LEFT_JOYSTICK_RIGHT       0b01111111


uint8_t getChar24Id(char c);

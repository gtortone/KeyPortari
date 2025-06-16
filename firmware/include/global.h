/*
 * Global defines/variables for the Project
*/
#pragma once

#define DEBUG        0

// Arduino Pins
#define CLOCK              9 // USB D-
#define DATA               8 // USB D+
#define OE_LEFT_PORT      12
#define OE_RIGHT_PORT     13
#define DIP_A_PIN         A6

#define SWCHA_0         0
#define SWCHA_1         1
#define SWCHA_2         2
#define SWCHA_3         3
#define SWCHA_4         4
#define SWCHA_5         5
#define SWCHA_6         6
#define SWCHA_7         7

// Flags for keyboard_state
#define MODIFIER        0x01
#define SHIFT           0x02
#define ALTGR           0x04

// global variables
extern volatile bool keyboard_action;

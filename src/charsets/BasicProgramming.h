/*
 * This protocol is "inspired by"
 * https://github.com/SteveGuidi/video-companion-media/tree/main/2021.06.09%20-%20Atari%202600%20Keyboard
*/

#pragma once

#include "../../PS2Keymaps.h"
#include "../../global.h"
#include <Arduino.h>

// Basic Programming 'special' key values
#define INPT_0     A0
#define INPT_1     A1
#define INPT_2     A2
#define INPT_3     A3
#define INPT_4     10
#define INPT_5     11

#define COLOR_KEY_SCAN_PIN    SWCHA_7
#define COLOR_KEY_OUTPUT_PIN  INPT_1


enum class KeyColor : uint8_t {
  White = 0x01,
  Red   = 0x02,
  Blue  = 0x04,
  Green = 0x08,
  All   = 0x0F  // Sum all values above.
};

constexpr KeyColor operator|(KeyColor lhs, KeyColor rhs) {
  return static_cast<KeyColor>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
}

constexpr KeyColor operator&(KeyColor lhs, KeyColor rhs) {
  return static_cast<KeyColor>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

inline KeyColor next(KeyColor color) {
  if (color == KeyColor::Green) return KeyColor::White;
  if (color == KeyColor::All) return color;
  return static_cast<KeyColor>(static_cast<uint8_t>(color) << 1);
}

struct KeyControllerEntry {
  char    ascii;
  KeyColor  color;
  uint8_t scanPin;     // SWCHA-Strobe
  uint8_t outPin;      // TIA-Input
};


static constexpr KeyControllerEntry BasicProgMap[] = {
    { PS2_F10, KeyColor::All,  SWCHA_7, INPT_0 },     // RUN/HALT PS2_FUNCTION | PS2_KEY_F10
    { PS2_TAB, KeyColor::All,  SWCHA_7, INPT_1 },     // COLOR PS2_FUNCTION | PS2_KEY_TAB
    { PS2_BACKSPACE, KeyColor::All,  SWCHA_7, INPT_4 },     // ERASE PS2_FUNCTION | PS2_KEY_BS

    { PS2_F1, KeyColor::White,  SWCHA_4, INPT_0 },   // STATUS PS2_FUNCTION | PS2_KEY_F1
    { PS2_F2, KeyColor::White,  SWCHA_4, INPT_1 },   // PROGRAM PS2_FUNCTION | PS2_KEY_F2
    { PS2_F3, KeyColor::White,  SWCHA_4, INPT_4 },   // STACK PS2_FUNCTION | PS2_KEY_F3
    { PS2_F4, KeyColor::White,  SWCHA_5, INPT_0 },   // VARIABLE PS2_FUNCTION | PS2_KEY_F4
    { PS2_F5, KeyColor::White,  SWCHA_5, INPT_1 },   // OUTPUT PS2_FUNCTION | PS2_KEY_F5
    { PS2_F6, KeyColor::White,  SWCHA_5, INPT_4 },   // GRAPHICS PS2_FUNCTION | PS2_KEY_F6
    { PS2_F11, KeyColor::White,  SWCHA_6, INPT_0 },   // STEP PS2_FUNCTION | PS2_KEY_F11
    //{ PS2_CTRL | PS2_KEY_MINUS,    KeyColor::White,  SWCHA_6, INPT_1 },   // SLOWER
    //{ PS2_CTRL | PS2_KEY_EQUAL,    KeyColor::White,  SWCHA_6, INPT_4 },   // FASTER
  
    { PS2_LEFTARROW, KeyColor::White,  SWCHA_3, INPT_2 },    // BACKWARD PS2_FUNCTION | PS2_KEY_L_ARROW
    { PS2_RIGHTARROW, KeyColor::White,  SWCHA_3, INPT_3 },   // FORWARD PS2_FUNCTION | PS2_KEY_R_ARROW
    { PS2_ENTER, KeyColor::White, SWCHA_3, INPT_5 },   // NEW LINE PS2_FUNCTION | PS2_KEY_ENTER
  
    { '+', KeyColor::Red,  SWCHA_4, INPT_0 },     // +        PS2_SHIFT | PS2_KEY_EQUAL
    { '-', KeyColor::Red,  SWCHA_4, INPT_1 },     // -        PS2_KEY_MINUS
    { ':', KeyColor::Red,  SWCHA_4, INPT_4 },     // ← (:)    PS2_SHIFT | PS2_KEY_SEMI
    { '*', KeyColor::Red,  SWCHA_5, INPT_0 },     // *        PS2_SHIFT | PS2_KEY_8
    { '/', KeyColor::Red,  SWCHA_5, INPT_1 },     // /        PS2_KEY_DIV
    { 'N', KeyColor::Red,  SWCHA_5, INPT_4 },     // NOTE (N) PS2_SHIFT | PS2_KEY_N
    { 'H', KeyColor::Red,  SWCHA_6, INPT_0 },     // HOR1 (H) PS2_SHIFT | PS2_KEY_H
    { 'V', KeyColor::Red,  SWCHA_6, INPT_1 },     // VER1 (V) PS2_SHIFT | PS2_KEY_V
    { 'K', KeyColor::Red,  SWCHA_6, INPT_4 },     // KEY (K)  PS2_SHIFT | PS2_KEY_K
  
    { '1', KeyColor::Red,  SWCHA_0, INPT_2 },     // 1        PS2_KEY_1
    { '2', KeyColor::Red,  SWCHA_0, INPT_3 },     // 2        PS2_KEY_2
    { '3', KeyColor::Red,  SWCHA_0, INPT_5 },     // 3        PS2_KEY_3
    { '4', KeyColor::Red,  SWCHA_1, INPT_2 },     // 4        PS2_KEY_4
    { '5', KeyColor::Red,  SWCHA_1, INPT_3 },     // 5        PS2_KEY_5
    { '6', KeyColor::Red,  SWCHA_1, INPT_5 },     // 6        PS2_KEY_6
    { '7', KeyColor::Red,  SWCHA_2, INPT_2 },     // 7        PS2_KEY_7
    { '8', KeyColor::Red,  SWCHA_2, INPT_3 },     // 8        PS2_KEY_8
    { '9', KeyColor::Red,  SWCHA_2, INPT_5 },     // 9        PS2_KEY_9
    //{ PS2_ALT | PS2_KEY_H,       KeyColor::Red,  SWCHA_3, INPT_2 },   // HOR2
    //{ PS2_ALT | PS2_KEY_V,       KeyColor::Red,  SWCHA_3, INPT_3 },   // VER2
    { '0', KeyColor::Red,  SWCHA_3, INPT_5 },     // 0        PS2_KEY_0
  
    { 'a', KeyColor::Blue,  SWCHA_4, INPT_0 },    // A (a)    PS2_KEY_A
    { 'b', KeyColor::Blue,  SWCHA_4, INPT_1 },    // B        PS2_KEY_B
    { 'c', KeyColor::Blue,  SWCHA_4, INPT_4 },    // C        PS2_KEY_C
    { 'd', KeyColor::Blue,  SWCHA_5, INPT_0 },    // D        PS2_KEY_D
    { 'e', KeyColor::Blue,  SWCHA_5, INPT_1 },    // E        PS2_KEY_E
    { 'f', KeyColor::Blue,  SWCHA_5, INPT_4 },    // F        PS2_KEY_F
    { 'g', KeyColor::Blue,  SWCHA_6, INPT_0 },    // G        PS2_KEY_G
    { 'h', KeyColor::Blue,  SWCHA_6, INPT_1 },    // H        PS2_KEY_H
    { 'i', KeyColor::Blue,  SWCHA_6, INPT_4 },    // I        PS2_KEY_I
  
    { 'j', KeyColor::Blue,  SWCHA_0, INPT_2 },    // J        PS2_KEY_J
    { 'k', KeyColor::Blue,  SWCHA_0, INPT_3 },    // K        PS2_KEY_K
    { 'l', KeyColor::Blue,  SWCHA_0, INPT_5 },    // L        PS2_KEY_L
    { 'm', KeyColor::Blue,  SWCHA_1, INPT_2 },    // M        PS2_KEY_M
    { 'n', KeyColor::Blue,  SWCHA_1, INPT_3 },    // N        PS2_KEY_N
    { 'o', KeyColor::Blue,  SWCHA_1, INPT_5 },    // O        PS2_KEY_O
    { 'p', KeyColor::Blue,  SWCHA_2, INPT_2 },    // P        PS2_KEY_P
    { 'q', KeyColor::Blue,  SWCHA_2, INPT_3 },    // Q        PS2_KEY_Q
    { 'r', KeyColor::Blue,  SWCHA_2, INPT_5 },    // R        PS2_KEY_R
    { 's', KeyColor::Blue,  SWCHA_3, INPT_2 },    // S        PS2_KEY_S
    { 't', KeyColor::Blue,  SWCHA_3, INPT_3 },    // T        PS2_KEY_T
    { ' ', KeyColor::Blue,  SWCHA_3, INPT_5 },    // SPACE    PS2_FUNCTION | PS2_KEY_SPACE
  
    { 'I', KeyColor::Green,  SWCHA_4, INPT_0 },   // IF       PS2_SHIFT | PS2_KEY_I
    { 'T', KeyColor::Green,  SWCHA_4, INPT_1 },   // THEN     PS2_SHIFT | PS2_KEY_T
    { 'E', KeyColor::Green,  SWCHA_4, INPT_4 },   // ELSE     PS2_SHIFT | PS2_KEY_E
    { 'G', KeyColor::Green,  SWCHA_5, INPT_0 },   // GOTO     PS2_SHIFT | PS2_KEY_G
    { '?', KeyColor::Green,  SWCHA_5, INPT_1 },   // PRINT    PS2_SHIFT | PS2_KEY_DIV
    { '"', KeyColor::Green,  SWCHA_5, INPT_4 },   // "        PS2_SHIFT | PS2_KEY_APOS
    { '(', KeyColor::Green,  SWCHA_6, INPT_0 },   // (        PS2_SHIFT | PS2_KEY_9
    { ')', KeyColor::Green,  SWCHA_6, INPT_1 },   // )        PS2_SHIFT | PS2_KEY_0
    { ',', KeyColor::Green,  SWCHA_6, INPT_4 },   // ,        PS2_KEY_COMMA
  
    { 'u', KeyColor::Green,  SWCHA_0, INPT_2 },   // U        PS2_KEY_U
    { 'v', KeyColor::Green,  SWCHA_0, INPT_3 },   // V        PS2_KEY_V
    { 'w', KeyColor::Green,  SWCHA_0, INPT_5 },   // W        PS2_KEY_W
    { 'x', KeyColor::Green,  SWCHA_1, INPT_2 },   // X        PS2_KEY_X
    { 'y', KeyColor::Green,  SWCHA_1, INPT_3 },   // Y        PS2_KEY_Y
    { 'z', KeyColor::Green,  SWCHA_1, INPT_5 },   // Z        PS2_KEY_Z
    { PS2_HOME, KeyColor::Green,  SWCHA_2, INPT_2 }, // CLEAR PS2_FUNCTION | PS2_SHIFT | PS2_KEY_HOME
    { '%', KeyColor::Green,  SWCHA_2, INPT_3 },   // MOD      PS2_SHIFT | PS2_KEY_5
//    { PS2_ALT | PS2_KEY_T,       KeyColor::Green,  SWCHA_2, INPT_5 }, // HIT
    { '=', KeyColor::Green,  SWCHA_3, INPT_2 },   // =        PS2_KEY_EQUAL
    { '>', KeyColor::Green,  SWCHA_3, INPT_3 },   // >        PS2_SHIFT | PS2_KEY_DOT
    { '<', KeyColor::Green,  SWCHA_3, INPT_5 }    // <        PS2_SHIFT | PS2_KEY_COMMA
};

/// Lookup function
inline const KeyControllerEntry* getKeyControllerEntry(char c) {
    for (const auto &e : BasicProgMap) {
        if (e.ascii == c) {
            return &e;
        }
    }
    return nullptr;
}

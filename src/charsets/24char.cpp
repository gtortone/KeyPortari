#include "24char.h"

uint8_t getChar24Id(char c) {
  uint8_t o = (uint8_t)c;

  // Upper case chars (A-Z)
  if(o > 64 && o < 91) {
    return (o - 63) * 4;  // e.g. 'A' (65) -> (65 - 63) * 4 = 8
  }
  // Lower case chars (a-z)
  if(o > 96 && o < 123) {
    return (o - 69) * 4;  // e.g. 'a' (97) -> (97 - 69) * 4 = 112
  }
  // Digits (0-9)
  if(o > 47 && o < 58) {
    return (o + 6) * 4;   // e.g. '0' (48) -> (48 + 6) * 4 = 216
  }
  // Other
  switch(o) {
    case 46:  // Dot
      return 1;
    case 44:  // Comma
      return 0;
    case PS2_UPARROW:
      return LEFT_JOYSTICK_UP;
    case PS2_LEFTARROW:
      return LEFT_JOYSTICK_LEFT;
    case PS2_DOWNARROW:
      return LEFT_JOYSTICK_DOWN;
    case PS2_RIGHTARROW:
      return LEFT_JOYSTICK_RIGHT;
    case PS2_BACKSPACE: // or PS2_DELETE
      return BACKSPACE_24CHAR;
    case PS2_ENTER:
      return ENTER_24CHAR;
    default:
      return 4; // Default is Space
  }
}

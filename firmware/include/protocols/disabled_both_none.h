#pragma once

inline void protocol_disabled_both_none_setup(){
  digitalWrite(OE_LEFT_PORT, HIGH);  // Activate left joystick forwarding via first 74HC4066
  digitalWrite(OE_RIGHT_PORT, HIGH); // Activate right joystick forwarding via second 74HC4066
}
inline void protocol_disabled_both_none_keyUp(){
  // empty
}
inline void protocol_disabled_both_none_keyDown(char /*value*/){
  // empty
}

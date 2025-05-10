#include "parallel_both_24char.h"

void protocol_parallel_both_24char_setup(){
}

void protocol_parallel_both_24char_keyUp(){
  // set port on RIOT bus to input. Also used on init!
  DDRD = 0b00000000;
  PORTD = 0b00000000; // floating, no pullups
  digitalWrite(OE_LEFT_PORT, HIGH);
  digitalWrite(OE_RIGHT_PORT, HIGH);
}

void protocol_parallel_both_24char_keyDown(char value){
  // Key down for
  // 1. set both SN74HC4066 to OE off!
  // 2. set data Port to output
  // 3. set data Port to keyvalue
  digitalWrite(OE_LEFT_PORT, LOW);
  digitalWrite(OE_RIGHT_PORT, LOW);
  
  #if DEBUG
  Serial.print(value);
  Serial.println(" down");
  #else
  DDRD = 0b11111111;
  // todo mapping/protocol specific
  PORTD = getChar24Id(value);
  #endif
}
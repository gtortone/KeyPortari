/*

todo:
 - Init process !
 - Add TJs ASCII mapping
 - I2C protocol ?

*/

#include "PS2Keymaps.h"

#define DEBUG        0

// Arduino Pins
#define CLOCK              9 // USB D-
#define DATA               8 // USB D+
#define LED_AND_OE        13
#define DIP_A_PIN         A0

// Flags for keyboard_state
#define MODIFIER        0x01
#define SHIFT           0x02
#define ALTGR           0x04

// 24 Char display 'special' key values
#define BACKSPACE_24CHAR   2
#define ENTER_24CHAR       3

#define LEFT_JOYSTICK_UP          0b11101111
#define LEFT_JOYSTICK_DOWN        0b11011111
#define LEFT_JOYSTICK_LEFT        0b10111111
#define LEFT_JOYSTICK_RIGHT       0b01111111


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

void set_port_input(){
  // set port on RIOT bus to input
  DDRD = 0b00000000;
  PORTD = 0b00000000; // floating, no pullups
}

void set_port_output(char value){
  // Key down for 
  // 1. set SN74LVC245AN to OE off !
  // 2. set data Port to output
  // 3. set data Port to keyvalue
  digitalWrite(LED_AND_OE, HIGH);
  #if DEBUG
  Serial.print(value);
  Serial.println(" down");
  #else
  DDRD = 0b11111111;
  // todo mapping/protocol specific
  PORTD = getChar24Id(value);
  #endif
}

uint8_t dipConfig;
PS2Keymap_t *keymap;

void setup() {
  #if DEBUG
  Serial.begin(115200);
  #endif

  pinMode(CLOCK, INPUT_PULLUP); // For most keyboards the builtin pullups are sufficient, so the 10k pullups can be omitted
  pinMode(DATA, INPUT_PULLUP);
  pinMode(LED_AND_OE, OUTPUT);
  digitalWrite(LED_AND_OE, LOW); // Activate joystick forwarding via transceiver (e.g. SN74LVC245AN)

  bitSet(PCICR, PCIE0);   // Enable pin change interrupts on pin B0-B7
  bitSet(PCMSK0, PCINT1); // Pin change interrupt on Clock pin (PCINT1 = PB1 Arduino Pin 9)

  int adcValue = analogRead(DIP_A_PIN);      // Read voltage of DIP-Switch block resistor ladder (0-1023)
  dipConfig = map(adcValue, 0, 1023, 15, 0); // Get binary value of DIP-Switch Block (0-15) 
 
  switch (dipConfig & 0b11) { // lower 2 bits of dipConfig define keyboard layout
    case 0b01: keymap = &PS2Keymap_German;      break;
    case 0b10: keymap = &PS2Keymap_French;      break;
    default: keymap = &PS2Keymap_US;            break;
  }

  #if DEBUG
  Serial.println("Keyboard test");
  Serial.println(dipConfig);
  #endif
}

uint8_t curscan = 0;
bool key_action = false;

ISR(PCINT0_vect) {
  uint16_t newscan = 0;
  for(int i = 0; i<11; i++) {
    while(digitalRead(CLOCK));
    newscan |= digitalRead(DATA) << i;
    while(!digitalRead(CLOCK));
  }
  newscan >>= 1;
  newscan &= 0xFF;
  key_action = true;

  curscan = newscan;
  bitSet(PCIFR, PCIF0);
}

uint8_t lastscan = 0;

void loop() {
 	static uint8_t keyboard_state = 0;
  int init_step = 0;
  bool init_done = true;
  unsigned long init_timer = 0;
  uint8_t busConfig = 0;
  char c;
/*
  if(init_step == 0 && (PORTD & 0x0F) == 0){
    init_step++;
    init_timer = millis();
  }else if(init_step == 1 && (PORTD & 0x0F) == 0b1111){
    init_step++;
  }else if(init_step == 2 && (PORTD & 0x0F) == 0){
    init_step++;
  }else if(init_step == 3 && (PORTD & 0x0F) != 0){
    uint8_t busConfig = (PORTD & 0x0F);
    init_step++;
    delay(20); // 16.6667 one 60 Hz frame
    set_port_output(busConfig); // return busConfig for confirmation on the bus
    delay(20); // 16.6667 one 60 Hz frame
    init_done = true;
  }
*/
  if(init_done){
    if(key_action){
      key_action = false;
      if(curscan != 0xF0 && curscan != 0xE0){
 				if (curscan < PS2_KEYMAP_SIZE){
          if (keyboard_state & SHIFT) {
            c = pgm_read_byte(keymap->shift + curscan);
          } else if ((keyboard_state & ALTGR) && keymap->uses_altgr) {
            c = pgm_read_byte(keymap->altgr + curscan);
          } else {
            c = pgm_read_byte(keymap->noshift + curscan);
          }
        }

        if(lastscan == 0xF0){
          if (curscan == 0x12 || curscan == 0x59) {
            keyboard_state &= ~SHIFT;
          } else if (curscan == 0x11 && (keyboard_state & MODIFIER)) {
            keyboard_state &= ~ALTGR;
          } else {
            // Key up
            // 1. set data Port to input
            // 2. set SN74LVC245AN to OE on !
            digitalWrite(LED_AND_OE, LOW);
            #if DEBUG
            Serial.print(c);
            Serial.println(" up");
            #else
            // Call Key up of Protocol here!
            set_port_input();
            #endif
          }
          // CTRL, ALT & WIN keys could be added
          // but is that really worth the overhead?
          keyboard_state &= ~MODIFIER;
        }else{
          if (curscan == 0x12 || curscan == 0x59) {
            keyboard_state |= SHIFT;
          } else if (curscan == 0x11 && (keyboard_state & MODIFIER)) {
            keyboard_state |= ALTGR;
          } else {
            if(lastscan == 0xE0){ // MODIFIER
              switch (curscan) {
                case 0x70: c = PS2_INSERT;      break;
                case 0x6C: c = PS2_HOME;        break;
                case 0x7D: c = PS2_PAGEUP;      break;
                case 0x71: c = PS2_DELETE;      break;
                case 0x69: c = PS2_END;         break;
                case 0x7A: c = PS2_PAGEDOWN;    break;
                case 0x75: c = PS2_UPARROW;     break;
                case 0x6B: c = PS2_LEFTARROW;   break;
                case 0x72: c = PS2_DOWNARROW;   break;
                case 0x74: c = PS2_RIGHTARROW;  break;
                case 0x4A: c = '/';             break;
                case 0x5A: c = PS2_ENTER;       break;
                default: break;
              }
            }
            // Call key-down of active protocol here!
            set_port_output(c);
          }
    			keyboard_state &= ~MODIFIER;
        }
      }
      lastscan = curscan;
    }

  }else  if( (init_timer - millis()) > 1000){ // init prozess takes too long! -> reseting
    init_step = 0;
  }

}

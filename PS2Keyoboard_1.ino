/*
docs/sources:
https://www.sciencebuddies.org/science-fair-projects/references/ascii-table
https://wolles-elektronikkiste.de/en/interrupts-part-2-pin-change-interrupts
https://www.hackster.io/michalin70/connect-a-usb-keyboard-with-an-arduino-50c077
https://www.youtube.com/watch?v=Bv_zbPUtAuQ

todo:
 - Add multiple keymaps (DE, US, ??) use switchblock 2-3 GPIOs (4-8 Keymaps)
 - Arrow keys = left joystick 
 - ASCII except "xxxx 1111" right port nibble
 - Add ShiftKey (0b00010010 left 0b01011001 right)
*/
#include <unordered_map>

#include "PS2Keymaps.h"

#define DEBUG           1

#define KEY_BACKSPACE   2
#define KEY_ENTER       3
#define KEY_SHIFT_1     3
#define KEY_SHIFT_2     3

#define KEY_UP          0b11101111  // same than left jostick up
#define KEY_DOWN        0b11011111  // same than left jostick down
#define KEY_LEFT        0b10111111  // same than left jostick left
#define KEY_RIGHT       0b01111111  // same than left jostick right

// Arduino Pins
#define CLOCK       9 //D-
#define DATA        8 //D+
#define LED_AND_OE 13

//#define BREAK     0x01
#define MODIFIER  0x01
#define SHIFT     0x02
#define ALTGR     0x04

// keymap for 24 char display
const char keymap2[][132] = {{
 /*   0 */  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
 /*  16 */  4, 4, 4, 4, 4, 176, 220, 4, 4, 4, 212, 184, 112, 200, 224, 4, 
 /*  32 */  4, 120, 204, 124, 128, 232, 228, 4, 4, 4, 196, 132, 188, 180, 236, 4, 
 /*  48 */  4, 164, 116, 140, 136, 208, 240, 4, 4, 4, 160, 148, 192, 244, 248, 4, 
 /*  64 */  4, 4, 152, 144, 168, 216, 252, 4, 4, 1, 4, 156, 4, 172, 0, 4, 
 /*  80 */  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, KEY_ENTER, 4, 4, 4, 4, 4, 
 /*  96 */  4, 4, 4, 4, 4, 4, KEY_BACKSPACE, 4, 4, 220, 4, 232, 244, 4, 4, 4, 
 /* 112 */  216, 1, 224, 236, 240, 248, 4, 4, 4, 4, 228, 0, 4, 252, 4, 4, 
 /* 128 */  4, 4, 4, 4
},{ // US Keyboard Layout map
    0,  0,   0,  0,  0,  0,  0, 0, 0, 0,  0,  0,  0,  0, '`', 0,
    0,  0,   0,  0,  0,'q','1', 0, 0, 0, 'z','s','a','w','2', 0,
    0,'c', 'x','d','e','4','3', 0, 0,' ','v','f','t','r','5', 0,
    0,'n', 'b','h','g','y','6', 0, 0, 0, 'm','j','u','7','8', 0,
    0,',', 'k','i','o','0','9', 0, 0,'.','/','l',';','p','-', 0,
    0,  0,'\'',  0,'[','=',  0, 0, 0, 0,13, ']', 0, '\\', 0, 0,
    0,  0,   0,  0,  0,  0,127, 0, 0,'1', 0,'4','7', 0, 0, 0,
  '0','.', '2','5','6','8',  0, 0, 0,'+','3','-','*','9', 0, 0,
    0,  0,  0,  0
}
};

uint8_t getChar24Id(char c) {
  uint8_t o = (uint8_t)c;

  // Großbuchstaben (A-Z)
  if(o > 64 && o < 91) {
    return (o - 63) * 4;  // Beispiel: 'A' (65) -> (65 - 63) * 4 = 8
  }
  // Kleinbuchstaben (a-z)
  if(o > 96 && o < 123) {
    return (o - 69) * 4;  // Beispiel: 'a' (97) -> (97 - 69) * 4 = 112
  }
  // Ziffern (0-9)
  if(o > 47 && o < 58) {
    return (o + 6) * 4;   // Beispiel: '0' (48) -> (48 + 6) * 4 = 216
  }
  // Sonderfälle
  switch(o) {
    case 32:  // Leerstelle
      return 4;
    case 46:  // Punkt
      return 1;
    case 45:  // Minus
      return 0;
    default:
      return 4;
  }
}

void set_port_input(){
  // set RIOT ports to input
  DDRD = 0b00000000;
  PORTD = 0b00000000; // floating, no pullups
/*
  pinMode(PORTD0, INPUT); // D0
  pinMode(PORTD1, INPUT); // D1
  pinMode(PORTD2, INPUT); // D2
  pinMode(PORTD3, INPUT); // D3
  pinMode(PORTD4, INPUT); // D4
  pinMode(PORTD5, INPUT); // D5
  pinMode(PORTD6, INPUT); // D6
  pinMode(PORTD7, INPUT); // D7
*/
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
  // todo find Char24 or TJ for value
  DDRD = 0b11111111;
  PORTD = value;
  #endif
}

void setup() {
  #if DEBUG
  Serial.begin(115200);
  #else
  set_port_input();
  #endif
  pinMode(CLOCK, INPUT_PULLUP); // For most keyboards the builtin pullups are sufficient, so the 10k pullups can be omitted
  pinMode(DATA, INPUT_PULLUP);
  pinMode(LED_AND_OE, OUTPUT);


  digitalWrite(LED_AND_OE, LOW); // Activate joystick forwarding via transceiver (e.g. SN74LVC245AN)

  bitSet(PCICR, PCIE0);   // Enable pin change interrupts on pin B0-B7
  bitSet(PCMSK0, PCINT1); // Pin change interrupt on Clock pin (PCINT1 = PB1 Arduino Pin 9)

  #if DEBUG
  Serial.println("Keyboard test");
  #endif
}

uint8_t curscan = 0;
bool key_action = false;
const PS2Keymap_t *keymap = &PS2Keymap_German;

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

int init_step = 0;
bool init_done = true;
unsigned long init_timer = 0;
uint8_t lastscan = 0;

void loop() {
 	static uint8_t state = 0;
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
    init_step++;
    init_done = true;
  }
*/
  if(init_done){

    if(key_action){
      key_action = false;
      if(curscan != 0xF0 && curscan != 0xE0){
 				if (curscan < PS2_KEYMAP_SIZE){
          if (state & SHIFT) {
            c = pgm_read_byte(keymap->shift + curscan);
          } else if ((state & ALTGR) && keymap->uses_altgr) {
            c = pgm_read_byte(keymap->altgr + curscan);
          } else {
            c = pgm_read_byte(keymap->noshift + curscan);
          }
        }

        if(lastscan == 0xF0){
          if (curscan == 0x12 || curscan == 0x59) {
            state &= ~SHIFT;
          } else if (curscan == 0x11 && (state & MODIFIER)) {
            state &= ~ALTGR;
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
          state &= ~MODIFIER; //~(BREAK | MODIFIER);
        }else{
          if (curscan == 0x12 || curscan == 0x59) {
            state |= SHIFT;
          } else if (curscan == 0x11 && (state & MODIFIER)) {
            state |= ALTGR;
          } else {
            if(lastscan == 0xE0){ // MODIFIER
            Serial.println("MODIFIER");
              switch (curscan) {
                case 0x70: c = PS2_INSERT;      break;
                case 0x6C: c = PS2_HOME;        break;
                case 0x7D: c = PS2_PAGEUP;      break;
                case 0x71: c = PS2_DELETE;      break;
                case 0x69: c = PS2_END;         break;
                case 0x7A: c = PS2_PAGEDOWN;    break;
                case 0x75: c = 'A';     break; // PS2_UPARROW
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
    			state &= ~MODIFIER;// ~(BREAK | MODIFIER);
        }
      }
      lastscan = curscan;
    }

  }else  if( (init_timer - millis()) > 1000){ // init prozess takes too long! -> reseting
    init_step = 0;
  }

}

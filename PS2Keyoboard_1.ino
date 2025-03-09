/*

  https://wolles-elektronikkiste.de/en/interrupts-part-2-pin-change-interrupts

*/

// Arduino Pins
#define CLOCK 9 //D-
#define DATA  8 //D+

const char keymap[] = {
  0, 0,  0,  0,  0,  0,  0,  0,
  0, 0,  0,  0,  0,  0, '`', 0,
  0, 0 , 0 , 0,  0, 'q','1', 0,
  0, 0, 'z','s','a','w','2', 0,
  0,'c','x','d','e','4','3', 0,
  0,' ','v','f','t','r','5', 0,
  0,'n','b','h','g','y','6', 0,
  0, 0, 'm','j','u','7','8', 0,
  0,',','k','i','o','0','9', 0,
  0,'.','/','l',';','p','-', 0,
  0, 0,'\'', 0,'[', '=', 0, 0,
  0, 0,13, ']', 0, '\\', 0, 0,
  0, 0, 0, 0, 0, 0, 127, 0,
  0,'1', 0,'4','7', 0, 0, 0,
  '0','.','2','5','6','8', 0, 0,
  0,'+','3','-','*','9', 0, 0,
  0, 0, 0, 0 };

void setup() {
  Serial.begin(115200);
  pinMode(CLOCK, INPUT_PULLUP); //For most keyboards the builtin pullups are sufficient, so the 10k pullups can be omitted
  pinMode(DATA, INPUT_PULLUP);
  pinMode(13, OUTPUT);

  bitSet(PCICR, PCIE0);   // Enable pin change interrupts on pin B0-B7
  bitSet(PCMSK0, PCINT1); // Pin change interrupt on Clock pin (PCINT1 = PB1 Arduino Pin 9)

  Serial.println("Keyboard test");
}

uint8_t lastscan;
uint8_t line = 0, col = 0;

ISR(PCINT0_vect) {
  uint16_t scanval = 0;
  for(int i = 0; i<11; i++)
  {
    while(digitalRead(CLOCK));
    scanval |= digitalRead(DATA) << i;
    while(!digitalRead(CLOCK));
  }
  scanval >>= 1;
  scanval &= 0xFF;
//  if(scanval == lastscan){
//    Serial.print(".");
//  }else{
    Serial.println(scanval, HEX);
    if(scanval != 0xF0){
      Serial.print(keymap[scanval]);
      if(lastscan != 0xF0){
        digitalWrite(13, HIGH);
        // Key down
        // 1. set SN74LVC245AN to OE off !
        // 2. set data Port to output
        // 3. set data Port to keyvalue
  //      set_port_output(keymap[scanval]);
        Serial.println(" down");
      }else{
        digitalWrite(13, LOW);
        // Key up
        // 1. set data Port to input
        // 2. set SN74LVC245AN to OE on !
  //      set_port_input();
        Serial.println(" up");
      }
//    }
  }
  lastscan = scanval;
  bitSet(PCIFR, PCIF0);

/*
  if(lastscan != 0xF0 && scanval != 0xF0){
    switch(scanval){
      case 0x5A: //Enter
        lcd.setCursor(0, ++line & 0x03);
        col = 0;
        break;
      case 0x66: //Backspace
        lcd.setCursor(--col, line);
        lcd.write(' ');
        lcd.setCursor(col, line);
      break;
      default:
        lcd.write(keymap[scanval]);
        col++;
    }
  }
*/
}

void loop() {
/*
  digitalWrite(13, LOW);
  delay(500);  
  digitalWrite(13, HIGH);
  delay(500);
// */
}

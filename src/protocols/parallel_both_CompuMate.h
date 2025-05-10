#pragma once

#include <Arduino.h>
#include "../../global.h"
#include "../charsets/CompuMate.h"

void protocol_parallel_both_CompuMate_setup();
void protocol_parallel_both_CompuMate_keyUp();
void protocol_parallel_both_CompuMate_keyDown(char value);

//extern volatile uint8_t my_4017_counter;

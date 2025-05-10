#pragma once

#include "../../PS2Keymaps.h"
#include "../../global.h"
#include <Arduino.h>

// CompuMate 'special' key values (uses the same as Basic Programming)
#define INPT_0     A0
#define INPT_1     A1
#define INPT_2     A2
#define INPT_3     A3
#define INPT_4     10
#define INPT_5     11

#define ROW_0     INPT_4
#define ROW_1     SWCHA_2
#define ROW_2     INPT_5
#define ROW_3     SWCHA_3


struct CompuMateKeyEntry {
  char    ascii;
  uint8_t column;
  uint8_t row;
  bool    shift;
  bool    func;
};

/*
 There's 10 columns labelled 0-9, and 4 rows, labelled 0-3.

                           Column
    0     1     2     3     4     5     6     7     8     9
  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
  | 7 | | 6 | | 8 | | 2 | | 3 | | 0 | | 9 | | 5 | | 1 | | 4 |  0
  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
  | U | | Y | | I | | W | | E | | P | | O | | T | | Q | | R |  1
  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+     Row
  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
  | J | | H | | K | | S | | D | |ent| | L | | G | | A | | F |  2
  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
  | M | | N | | < | | X | | C | |spc| | > | | B | | Z | | V |  3
  +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
*/
// move map to flash, if search is as fast as RAM? 
//const PROGMEM CompuMateKeyEntry CompuMateMap[] = {
static constexpr CompuMateKeyEntry CompuMateMap[] = {
  { '7', 0,  ROW_0, false, false },
  { 'u', 0,  ROW_1, false, false },
  { 'j', 0,  ROW_2, false, false },
  { 'm', 0,  ROW_3, false, false },

  { '6', 1,  ROW_0, false, false },
  { 'y', 1,  ROW_1, false, false },
  { 'h', 1,  ROW_2, false, false },
  { 'n', 1,  ROW_3, false, false },

  { '8', 2,  ROW_0, false, false },
  { 'i', 2,  ROW_1, false, false },
  { 'k', 2,  ROW_2, false, false },
  { ',', 2,  ROW_3, false, false },

  { '2', 3,  ROW_0, false, false },
  { 'w', 3,  ROW_1, false, false },
  { 's', 3,  ROW_2, false, false },
  { 'x', 3,  ROW_3, false, false },

  { '3', 4,  ROW_0, false, false },
  { 'e', 4,  ROW_1, false, false },
  { 'd', 4,  ROW_2, false, false },
  { 'c', 4,  ROW_3, false, false },

  { '0', 5,  ROW_0, false, false },
  { 'p', 5,  ROW_1, false, false },
  { PS2_ENTER, 5,  ROW_2, false, false },
  { ' ', 5,  ROW_3, false, false },

  { '9', 6,  ROW_0, false, false },
  { 'o', 6,  ROW_1, false, false },
  { 'l', 6,  ROW_2, false, false },
  { '.', 6,  ROW_3, false, false },

  { '5', 7,  ROW_0, false, false },
  { 't', 7,  ROW_1, false, false },
  { 'g', 7,  ROW_2, false, false },
  { 'b', 7,  ROW_3, false, false },

  { '1', 8,  ROW_0, false, false },
  { 'q', 8,  ROW_1, false, false },
  { 'a', 8,  ROW_2, false, false },
  { 'z', 8,  ROW_3, false, false },

  { '4', 9,  ROW_0, false, false },
  { 'r', 9,  ROW_1, false, false },
  { 'f', 9,  ROW_2, false, false },
  { 'v', 9,  ROW_3, false, false },

  // Upper case -> with shift key
  { '$', 0,  ROW_0, true, false },
  { 'U', 0,  ROW_1, true, false },
  { 'J', 0,  ROW_2, true, false },
  { 'M', 0,  ROW_3, true, false },

  { '?', 1,  ROW_0, true, false },
  { 'Y', 1,  ROW_1, true, false },
  { 'H', 1,  ROW_2, true, false },
  { 'N', 1,  ROW_3, true, false },

  { '(', 2,  ROW_0, true, false },
  { 'I', 2,  ROW_1, true, false },
  { 'K', 2,  ROW_2, true, false },
  { '<', 2,  ROW_3, true, false },

  { '-', 3,  ROW_0, true, false },
  { 'W', 3,  ROW_1, true, false },
  { 'S', 3,  ROW_2, true, false },
  { 'X', 3,  ROW_3, true, false },

  { '*', 4,  ROW_0, true, false },
  { 'E', 4,  ROW_1, true, false },
  { 'D', 4,  ROW_2, true, false },
  { 'C', 4,  ROW_3, true, false },

  { '"', 5,  ROW_0, true, false },
  { 'P', 5,  ROW_1, true, false },
//  { PS2_ENTER, 5,  ROW_2, true, false },
//  { ' ', 5,  ROW_3, true, false },

  { ')', 6,  ROW_0, true, false },
  { 'O', 6,  ROW_1, true, false },
  { 'L', 6,  ROW_2, true, false },
  { '.', 6,  ROW_3, true, false },

  { '=', 7,  ROW_0, true, false },
  { 'T', 7,  ROW_1, true, false },
  { 'G', 7,  ROW_2, true, false },
  { 'B', 7,  ROW_3, true, false },

  { '+', 8,  ROW_0, true, false },
  { 'Q', 8,  ROW_1, true, false },
  { 'A', 8,  ROW_2, true, false },
  { 'Z', 8,  ROW_3, true, false },

  { '/', 9,  ROW_0, true, false },
  { 'R', 9,  ROW_1, true, false },
  { 'F', 9,  ROW_2, true, false },
  { 'V', 9,  ROW_3, true, false },
  // Todo FUNC-Key modifier!
//  { PS2_ENTER, 5,  ROW_2, true, false },
  { PS2_BACKSPACE, 5,  ROW_3, false, true }

};

/// Lookup function
inline const CompuMateKeyEntry* getCompuMateKeyEntry(char c) {
    for (const auto &e : CompuMateMap) {
        if (e.ascii == c) {
            return &e;
        }
    }
    return nullptr;
}


#include <avr/pgmspace.h>

const int ditperiods = 40;

const char PROGMEM CWCHARS[] =  
                  // Index Char ASCII  CW   length
    {0B10010101,  //   0    /    47   -..-.   5
     0B11111101,  //   1    0    48   -----   5
     0B01111101,  //   2    1    49   .----   5
     0B00111101,  //   3    2    50   ..---   5
     0B00011101,  //   4    3    51   ...--   5
     0B00001101,  //   5    4    52   ....-   5
     0B00000101,  //   6    5    53   .....   5
     0B10000101,  //   7    6    54   -....   5
     0B11000101,  //   8    7    55   --...   5
     0B11100101,  //   9    8    56   ---..   5
     0B11110101,  //  10    9    57   ----.   5
     0B10001101,  //  11    =    61   -...-   5
     0B01000010,  //  12    A    65   .-      2
     0B10000100,  //  13    B    66   -...    4 
     0B10100100,  //  14    C    67   -.-.    4
     0B10000100,  //  15    D    68   -..     3
     0B00000001,  //  16    E    69   .       1
     0B00100100,  //  17    F    70   ..-.    4
     0B11000011,  //  18    G    71   --.     3
     0B00000100,  //  19    H    72   ....    4
     0B00000010,  //  20    I    73   ..      2
     0B01110100,  //  21    J    74   .---    4
     0B10100011,  //  22    K    75   -.-     3
     0B01000100,  //  23    L    76   .-..    4
     0B11000010,  //  24    M    77   --      2
     0B10000010,  //  25    N    78   -.      2
     0B11100011,  //  26    O    79   ---     3
     0B01100100,  //  27    P    80   .--.    4
     0B11010100,  //  28    Q    81   --.-    4
     0B01000011,  //  29    R    82   .-.     3
     0B00000011,  //  30    S    83   ...     3
     0B10000001,  //  31    T    84   -       1
     0B00100011,  //  32    U    85   ..-     3
     0B00010100,  //  33    V    86   ...-    4
     0B01100011,  //  34    W    87   .--     3
     0B10010100,  //  35    X    88   -..-    4
     0B10110100,  //  36    Y    89   -.--    4
     0B11000100}; //  37    Z    90   --..    4


void cwSendText()
{
  const char *p = CWTEXT ;
  
  uint8_t ch ;
  ch = pgm_read_byte( p );
  while( ch ) {
    cwSendCharacter( ch );
    p++ ;
    ch = pgm_read_byte( p );
  }
}

void cwSendCharacter(byte character) 
{
  uint8_t idx = 0 ;
  
  // Determine the CW character to send from the lookup table
  if (character >= (byte) '/' && character <= (byte) '9') {
    idx = character - (byte) '/' ;
  } else if (character >= (byte) 'A' && character <= (byte) 'Z') {
    idx = character - (byte) 'A' + 12 ;
  } else if (character == (byte) '=') {
    idx = character - (byte) '=' + 11 ;
  } else if (character == (byte) '.') {
    // Period is 6 symbols so does not fit a byte
    cwSound (1, true);
    cwSound (3, true);
    cwSound (1, true);
    cwSound (3, true);
    cwSound (1, true);
    cwSound (3, true);
    cwSound(3,false); // space between characters is one dah
    return ;
  } else {
    // Space or any unrecognized char, silence.
    cwSound(4, false);
    return;
  }

  byte cw = pgm_read_byte( &CWCHARS[idx] );

  // Send the character from the CW table.
  byte nrsymbols = cw & 0B00000111;  
  while (nrsymbols > 0) {
    byte dits = (cw & 0B10000000) > 0 ? 3 : 1;
    cwSound (dits, true);
    cw = cw << 1;
    nrsymbols--;
  }
  cwSound(3,false); // space between characters is one dah
}

/*
 * Sends sound for the number of dits + 1, or silence.
 * 
 * cwSound(1,false); // No sound, duration 1 dit.
 * cwSound(2,true);  // 1 dit sound, + 1 dit silence.
 */
void cwSound(int dits, boolean on) {
  int periodcounter = ditperiods * dits;
  while (periodcounter > 0) {
    digitalWrite(audioPin, on ? HIGH : LOW);
    delay(8);
    digitalWrite(audioPin, LOW);
    delay(8);
    periodcounter--;
  }
  if (on) {
    // one dit pause after sound.
    cwSound(1,false);
  }
}


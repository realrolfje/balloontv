
const int ditperiods = 40;

void cwSendText() {
  for (int i = 0; i < CWTEXT.length(); i++) {
    byte character = 0;
    switch (CWTEXT.charAt(i)) {
      // One byte CW characters:
      // number of dots/dashes -,
      // 1 = dash, 0 = dot     vvv
      //                  vvvvv
      case 'A': character = 0B01000010; break; // .-    length 2
      case 'B': character = 0B10000100; break; // -...  length 4 
      case 'C': character = 0B10100100; break; // -.-.  length 4
      case 'D': character = 0B10000100; break; // -..   length 3
      case 'E': character = 0B00000001; break; // .     length 1
      case 'F': character = 0B00100100; break; // ..-.  length 4
      case 'G': character = 0B11000011; break; // --.   length 3
      case 'H': character = 0B00000100; break; // ....  length 4
      case 'I': character = 0B00000010; break; // ..    length 2
      case 'J': character = 0B01110100; break; // .---  length 4
      case 'K': character = 0B10100011; break; // -.-   length 3
      case 'L': character = 0B01000100; break; // .-..  length 4
      case 'M': character = 0B11000010; break; // --    length 2
      case 'N': character = 0B10000010; break; // -.    length 2
      case 'O': character = 0B11100011; break; // ---   length 3
      case 'P': character = 0B01100100; break; // .--.  length 4
      case 'Q': character = 0B11010100; break; // --.-  length 4
      case 'R': character = 0B01000011; break; // .-.   length 3
      case 'S': character = 0B00000011; break; // ...   length 3
      case 'T': character = 0B10000001; break; // -     length 1
      case 'U': character = 0B00100011; break; // ..-   length 3
      case 'V': character = 0B00010100; break; // ...-  length 4
      case 'W': character = 0B01100011; break; // .--   length 3
      case 'X': character = 0B10010100; break; // -..-  length 4
      case 'Y': character = 0B10110100; break; // -.--  length 4
      case 'Z': character = 0B11000100; break; // --..  length 4
      case '0': character = 0B11111101; break; // ----- length 5
      case '1': character = 0B01111101; break; // .---- length 5
      case '2': character = 0B00111101; break; // ..--- length 5
      case '3': character = 0B00011101; break; // ...-- length 5
      case '4': character = 0B00001101; break; // ....- length 5
      case '5': character = 0B00000101; break; // ..... length 5
      case '6': character = 0B10000101; break; // -.... length 5
      case '7': character = 0B11000101; break; // --... length 5
      case '8': character = 0B11100101; break; // ---.. length 5
      case '9': character = 0B11110101; break; // ----. length 5
      case '/': character = 0B10010101; break; // -..-. length 5
      case ' ': character = 0B00000000; break; // adds extra dah of silence
    } // switch
    cwSendCharacter(character);
  } // for
}

void cwSendCharacter(byte character){
  byte nrsymbols = character & 0B00000111;  
  while (nrsymbols > 0) {
    byte dits = (character & 0B10000000) > 0 ? 3 : 1;
    cwSound (dits, true);
    character = character << 1;
    nrsymbols--;
  }
  cwSound(3,false); // space is one dah
}

void cwSound(int dits, boolean on) {
  int periodcounter = ditperiods * dits;
  while (periodcounter > 0) {
    digitalWrite(audioPin, on ? HIGH : LOW);
    delay(1);
    digitalWrite(audioPin, LOW);
    delay(1);
    periodcounter--;
  }
  if (on) {
    // one dit pause after sound.
    cwSound(1,false);
  }
}


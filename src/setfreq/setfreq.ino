/*
 * Sets the frequency of the ATV transmitter.
 * 
 * Connections from tiny8/pins_arduino.h:
 * 
 * ATMEL ATTINY45/85 / ARDUINO
 *
 *                          +-\/-+
 * (reset) Ain0 (D 5) PB5  1|    |8  Vcc
 *         Ain3 (D 3) PB3  2|    |7  PB2 (D 2) Ain1 SCL
 *         Ain2 (D 4) PB4  3|    |6  PB1 (D 1) pwm1
 *                    GND  4|    |5  PB0 (D 0) pwm0 SDA
 *                          +----+
 * 
 * Supported ATtiny commands:
 * pinMode()
 * digitalWrite() 
 * digitalRead()
 * analogRead()
 * analogWrite()
 * shiftOut()
 * pulseIn()
 * millis()
 * micros()
 * delay()
 * delayMicroseconds()
 *
 */

#include <TinyWireM.h>                  // I2C Master lib for ATTinys which use USI

#include <avr/pgmspace.h>

// The TinyWire library uses the lower 7 bits in the address byte, 
// shifts them left and adds a 1 for write and 0 for read. See line 46 of TinyWireM.cpp.
// 
// The address of the SP055 is 1100 0010 for write, 
// See datasheets/SP5055.PDF, table 4 and table 1.
// 00 : 0V to 0,2V
// 01 : Always valid (don't know what this means in the datasheet)
// 10 : 0,3V to 0,7V
// 11 : 0,8V to 13,2V
//                   `-------vv
#define SP5055_ADDR   B01100001  // 0xC2 >> 1
 
const int ledPin   = 4; // D4 (pin 3)
const int audioPin = 3; // D3 (pin 2)

const char PROGMEM CWTEXT[] = "PI4RCG = WWW.BALLONVOSSENJACHT.NL = ";

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(audioPin, OUTPUT);
  TinyWireM.begin();                    // initialize I2C lib

  digitalWrite(ledPin, HIGH);
  setFrequency(1252000000L);
  digitalWrite(ledPin, LOW);
}

void loop() {

  cwSendText();
}

void squarewave() {  
  /* 
   * Flip pin. For testing the programmer and
   * temperature stability of the ATtiny85
   * at -20 C.
   */
  while (true) {
    digitalWrite(ledPin, HIGH);
    delay(1);
    digitalWrite(ledPin, LOW);
    delay(1);
  };
}

void setFrequency(unsigned long ftx) {
  /*
   * Pseudo-code for calculating the divider
   * 
   * Ftx = Fcomp * 16 * divider
   * Where Fcomp = 4MHz/512 = 7812,5 Hz
   * 
   * Which means: 
   * divider = Ftx/(16 * Fcomp)
   * divider = Ftx/(16 * 7812,5)
   * divider = Ftx/(125.000)
   * 
   * Example:
   * Ftx = 1252 MHz
   * divider = (1.252.000.000 / 125.000)
   * divider = (1.252.000 / 125)
   * divider = 10.016 (decimal)
   * divider = 0010 0111 0010 0000 (binary)
   *              2    7   2    0
   * 
   */

  unsigned long divider = ftx / 125000;

  byte dividerLSB = (byte) divider;
  byte dividerMSB = (byte) (divider >> 8);

  /*
   * Code for sending stuff over i2c to the SP5055. Draft.
   */
  TinyWireM.beginTransmission(SP5055_ADDR); // Address SP5055.
  
  TinyWireM.send(dividerMSB);           // Programmable Divider MSB
                                        // bit 7: Always 0
                                        // bit 6 to 0: 2^14 to 2^8
                                        
  TinyWireM.send(dividerLSB);           // Programmable Divider LSB
                                        // bit 7 to 0: 2^7 to 2^0

  TinyWireM.send(B10001110);            // Charge pump and test bits.
                                        // bit 7: Always 1
                                        // bit 6: CP, Charge pump: 
                                        //        0 = 50uA
                                        //        1 = 170uA
                                        // bit 5: T1 test mode bit,
                                        //        0 = normal mode
                                        //        1 = test mode, connects Fcomp to P6 
                                        //            and Fdiv to P7.
                                        // bit 4: T0 test mode bit,
                                        //        0 = normal mode
                                        //        1 = disable charge pump
                                        // bit 3: Always 1
                                        // bit 2: Always 1
                                        // bit 1: Always 1
                                        // bit 0: OS, Varactor drive disable switch
                                        //        0 = normal mode
                                        //        1 = Disable charge pump drive amplifier
  
  TinyWireM.send(B00000000);            // I/O port control bits (blinkin' lights)
                                        // bit 7: P7
                                        // bit 6: P6
                                        // bit 5: P5
                                        // bit 4: P4
                                        // bit 3: P3
                                        // bit 2: don't care
                                        // bit 1: don't care
                                        // bit 0: P0
  
  TinyWireM.endTransmission();          // Send to the slave
}



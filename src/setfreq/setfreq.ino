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

// See datasheets/SP5055.PDF, table 4 and table 1.
// 00 : 0V to 0,2V
// 01 : Always valid (don't know what this means in the datasheet)
// 10 : 0,3V to 0,7V
// 11 : 0,8V to 13,2V
//                   `--------vv
#define SP5055_W_ADDR   B11000110
#define SP5055_R_ADDR   B11000111
 
int ledPin = 4; // D4 (pin 3)

void setup() {
  pinMode(ledPin, OUTPUT);
  TinyWireM.begin();                    // initialize I2C lib
}

void loop() {
  digitalWrite(ledPin, HIGH);
  setFrequency(1275000000);
  digitalWrite(ledPin, LOW);

  delay(500);
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

void setFrequency(long ftx) {
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
   * Ftx = 1275 MHz
   * divider = (1.275.000.000 / 125.000)
   * divider = (1.275.000 / 125)
   * divider = 10.200 (decimal)
   * divider = 0010 0111 1101 1000 (binary)
   * 
   */

  long divider = ftx / 125000;

  byte dividerLSB = (byte) divider;
  byte dividerMSB = (byte) (divider >> 8);

  /*
   * Code for sending stuff over i2c to the SP5055. Draft.
   */
  TinyWireM.beginTransmission(SP5055_W_ADDR); // Address SP5055.
  
  TinyWireM.send(dividerMSB);           // Programmable Divider MSB
                                        // bit 7: Always 0
                                        // bit 6 to 0: 2^14 to 2^8
                                        
  TinyWireM.send(dividerLSB);           // Programmable Divider LSB
                                        // bit 7 to 0: 2^7 to 2^0

  TinyWireM.send(0x10001110);           // Charge pump and test bits.
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
  
  TinyWireM.send(0x00);                 // I/O port control bits (blinkin' lights)
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



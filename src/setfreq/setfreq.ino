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
 */
int ledPin = 1; // D1

void setup() {
  pinMode(ledPin, OUTPUT);

  /* 
   * Flip pin. For testing the programmer and
   * temperature stability of the ATtiny85
   * at -20 C.
   */
  
  while (true) {
    digitalWrite(ledPin, HIGH);
    delay(10);
    digitalWrite(ledPin, LOW);
    delay(10);
  };
  
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

  

}

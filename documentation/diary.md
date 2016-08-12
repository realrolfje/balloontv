The project diary
=================

A list of funny notes and events of what happens on the project:

- 28 June project started, uploaded to github on https://github.com/realrolfje/balloontv
- 29 June Randy orders 23cm ATV transmitter
- 2 July IC selection based on weight, power usage, I2C capabilities and termperature
  stability. Ordered ATTiny45 and 85 in normal and SMD packages.
- 5 July Configured Arduino IDE with board info from highlowtech, found pin mappings
  (what is connected to what), created Sketch with square wave on pin 6 (D1) to measure
  temperature stability in the freezer when the ICs are delivered.
- 6 july Added I2C library, added code for programming the SP5055, and some pseudo
  code to calculate the divider.
- 7 july Tested the ATTiny 45 (8 pin DIL housing, 8MHz) for temperature stability. Code
  for flipping a pin every 1 mS, produces aa square wave.
  At room temperature: 525,0 Hz (with some jitter)
  After 45 minutes without power in a freezer, at -20 C: 515,2 Hz (with jitter, 4% slower).
  After 20 minutes under power in the freezer, back up to 516 Hz.
  After 4 more minutes under power in the freezer, back up to 517 Hz.
  Conclusion: Temperature instability will likely not be a problem in this application.
- 8 july I2C code added, screenshot of the oscilloscope showing data being transmitted,
  looking good so far.
- 10 july Added CW sounder on pin 1. Problem: Long strings don't fit the program memory.
  When using the original idea to do cwSendText("SOME TEXT HERE"), the program crashes,
  not producing sound or I2C. Sending all characters as separate cwSendCharacter(0B01100100);
  calls works. Although the current workaround works, it is not very neat code.
- 17 july Tighter code with lookup table for CW sending, uses slightly less memory.
  I2C + Long CW text working on SMD ATTiny85 (has more memory). Needs testing with the
  ATV transmitter and final tweaks and assembly. Looks like it can be inline programmed.
  Found a link to http://nootropicdesign.com/projectlab/2011/03/20/text-and-graphics-overlay/
  for video overlay.
  http://electronics-home-projects.tripod.com/
- 28 july Analyzed the I2C data on the commtech board. Does not look anything like I
  expected to see based on the SP5055 datasheet. Spoke to people who have experience with
  this board, and the mention that it also sends display data over I2C, which explaines the
  strange bytes I see.
- 31 july Fixed a few bugs in the code, burned it to the tiny and hooked it up to the
  SP5055 in the transmitter. Works like a charm. Next up: Couple the Audio for the CW
  into the left and right audio channels. Need to figure out what the level needs to be.
- 12 Aug Simon applied a cool pull request for freeing up program space by storing the CW
  lookup table in program memory. The old version uses bytes (22%) of dynamic memory, and
  the change frees up 38 bytes and reduces usage to 78 bytes (15%) of dynamic memory.


# Notes

Install ITTiny library into Arduino IDE from 
code.google arduino-tiny

In the IDE, load the  Arduino ISP sketch and upload it to an UNO.

On the UNO, connect 10uF over reset/gnd. Then connect:
- 5v to pin 8 of the ATTiny
- GND to pin 4
- pin 10 to pin 1 (reset)
- pin 11 to pin 5
- pin 12 to pin 6
- pin 13 to pin 7

In the Arduino IDE, select the attiny 8MHz as board.
Select arduino as ISP as programmer.
Burn boatloader onto the attiny.

Check this video:
https://www.youtube.com/watch?v=i9WOwDrpRKs

Check high-low tech for board manager links
http://highlowtech.org/?p=1695

In Arduino 1.6.9, the board URLs are in the settings. Add the URL there. Then go to the board manager and install the attiny manager. Select Board ATTiny, Processor AT85 and clock 8MHz from the Tools menu in the IDE. The bottom right of the screen should now read "ATTiny 24/45/85, ATTiny85, Internal 8MHz on /dev/..."

I2C library for ATtiny
http://playground.arduino.cc/Code/USIi2c

The I2C connections are pretty straight forward:
    Arduino SDA <— 4.7k Resistor Tied to 5v —-> ATtiny 85 – PB0
    Arduino SCL <— 4.7k Resistor Tied to 5v —-> ATtiny 85 – PB3
    ATtiny 85 – PB1 <— 330 Resistor —- LED —- > GND
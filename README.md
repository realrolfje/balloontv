Balloon TV
==========

This is a little project for adding an ATTiny 85 to a Commtech Video transmitter in order
to get it to transmit it on 1252MHz, and send out a CW (morse code) identifier on the 
audio channels. The transmitter is a quick and simple solution for the annual Dutch
Balloon Fox Hunt called [Landelijke Ballonvossenjacht](http://www.ballonvossenjacht.nl/)

Information on how to build your own can be found in the [development guide](documentation/how-to-build.md).
Datasheets of all the used hardware can be found in the [datasheets directory](documentation/datasheets).

A brief timeline with notes and funny stories can be found in the [project diary](documentation/diary.md).
I'll try to keep it updated with the latest info as much as possible.

TODO list
---------

In order of importance:
- [x] Create Arduino code for setting the frequency of the Comtech transmitter (using
      I2C to program the SP5055).
- [x] Create code for sending CW (morse code) over the audio channel of the Comtech.
- [x] Add microcontroler inside the comtech enclosure
- [x] Add SPI connector for external programming
- [x] Create lightweight cloverleaf antenna
- [ ] Connect and test Video signal (send/receive)
- [ ] Add output amplifier to the Comtech to create approx. 1.5 Watts of output power.
- [ ] Make the whole setup water-proof.

Ideas and nice to haves (expected to be implemented for the 2017 version of the ATV transmitter)
- [ ] Video text overlay with callsign (and other info?). I2C teletext ICs are available
      for this.
- [ ] Temperature inside/outside as text overlay. I2C temperature sensor.
- [ ] Add the audio of the voice transponder in the balloon to the audio channels of
      the video.

![I2C communication](documentation/images/I2C-communcation.jpg)

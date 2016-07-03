# Timeline
28 June project started
29 June Randy orders 23cm ATV transmitter
2 July IC selection based on weight, power usage, I2C capabilities and termperature stability. Ordered ATTiny45 and 85 in normal and SMD packages.


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
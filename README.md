With this project you are able to control the LED stripes of your living wall with you TV remote control and refers to tinkerers.

**Before you start reading this text below or use the code/PCB I mention below:** 
_I know my project is not perfect and may have the ... let's say 'potential' for improvements. Any productive comment is welcome!_

There are some reasons why I did not simply went into the next electronic store and bought a pre-made LED-stripe:
- I want to get more into electronics, what is better than learning by doing? (no, this time no smoke esacped during testing)
- pre-made LED strips controller works, in most cases, only with LED strip from the same company. So you have to replace your current LED strips which is, in my opinion, nonsense
- even if there is at least one LED strip controller for your LED strips, you get one more IR remote control device (yes, one MORE)
- a pre-made LED strip controller does only what it is supposed to do. And this leads us to the biggest advantage of DIY projects: you can define anything by your own. 

Nevertheless this project has also a few downsides:
- you need a specific PCB or at least a template solder mask to create the circuit (if you do not want to have it on a bread-board)
- you need to find out the IR codes of your buttons which are unused on your TV remote control
- and this leads to the next point: you need some buttons on your TV remote control which are not used yet
- you need to have the equipment available (solder station, ...)
- you need to buy all the parts (which may cost more than a pre-made LED stripe + controller)

Anyway, as a tinkerer you do have some parts available and also at least an Arduino UNO to program the ATTINY85.
With the help of an Arduino (UNO) it is also possible to read the IR codes of the unused button of your TV remote control. So let's get started ...

Requirements:
- Arduino Software
- Arduino Software: library to program an ATTINY85
- Hardware: see PCB and the BOM list below

The wiring of this project and BOM can be found at this web location: https://easyeda.com/michi/Wohnwand_LED-1d8cbbe2c72c49daaca4bd0bf47e9f24

For additional information on the PCB layout see https://aisler.net/p/AGCJZQNS 

R7, Q3 and D3 are used as a reverse voltage protection. If want more information just search for "reverse voltage protection mosfet" on the internet. 

The idea behind a temperature sensor: it measures the actucal temperature inside the case and, if a threshold is exceeded, it will turn off all outputs. You can use any temperature sensor, in my project I used a **TMP36**. If you use a different sensor then you need to adapt the arduino code tho. I also used the diagnostic LED as indicator if something went wrong.

Before you upload the code onto the ATTINY85 please make sure that you **first upload the bootloader with _8MHz_**. Otherwise the IR code may not work (I only got it running with 8Mhz, otherwise the received values could be random/differs each time).

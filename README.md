With this project you are able to control the LED stripes of your living wall with you TV remote control and refers to tinkerers.

**Before you start reading this text below or use the code/PCB I mention below:** 
_I know my project is not perfect and may have the ... let's say 'potential' for improvements. Any productive comment is welcome, but comments like " ... OMG what the f!"§ ..." does not help improving this project. So please be respectful._

There are some reasons why I did not simply went into the next electronic store and bought a pre-made LED-stripe:
- I want to get more into electronics, what is a better as learning by doing? (no, this time no smoke esacped during testing)
- pre-made LED stripes controller works, in most cases, only with LED stripe from the same company. So you have to replace your current LED stripes which is, in my opinion, nonsense
- even if there is at least one LED stripe controller for your LED stripes, you get one more IR remote control device (yes, one MORE)
- a pre-made LED stripe controller does only what it is supposed to do. If you make one on your own you can, for instance, add a fading function with a fade-in/fade-out time what YOU define

Nevertheless this project has also a few downsides:
- you need a specific PCB or at least a template solder mask to create the circuit (if you do not want to have it on a bread-board)
- you need to find out the IR codes of your buttons which are useless on your TV remote control
- you need some buttons on your TV remote control which are not used yet
- you need to have the equipment available (solder station, ...)
- you need to buy all the parts (which may cost more than a pre-made LED stripe + controller)

Anyway, as a tinkerer you do have at least some parts available and have an Arduino UNO to program the ATTINY85 and use the Arduino UNO to read the IR codes of the unused button of your TV remote control. So let's get started ...

Requirements:
- Arduino Software
- Arduino Software: library to program an ATTINY85
- Hardware: see BOM

For programming the ATTINY85 I used my homemade Arduino UNO shield (if you use a websearch of your choice to find one on the internet).

For additional information on the PCB layout see https://aisler.net/p/AGCJZQNS (BOM will follow).

The code for the ATTINY85 is added in this github repository. Be aware that the code did not cover the temperature monitoring feature so far. 

Before you upload the code onto the ATTINY85 please make sure that you **first upload the bootloader with _8MHz_**. Otherwise the IR code will not work (it will read out complete nonsense).

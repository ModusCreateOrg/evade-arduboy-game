# Arduboy Game

Arduboy Game for Client Gifts, 2016 Holidays.

![Arduboy](arduboy.gif)

## Quick Start

Arduino has it's own IDE based upon the Wiring and Processing IDE. A program in this
environment is refered to as a Sketch. 

The IDE can be downloaded here:

[Arduino IDE](https://www.arduino.cc/en/Main/Software)

Once you have it installed follow the steps under Resources to install
the Arduboy library.

To add new files to a sketch you can use the drop-down in the top right.

Select the option "New tab"

Tabs can be renamed and deleted from this menu.

When you plug a device into your USB port you need to set the *Port* from the
*Tools* menu.

In addition to the above you also need to select the *Board* as Arduboy (explained in the Arduboy library guide).  If you don't see Arduboy, use Arduino Leonardo instead.

The *Tick* icon can be used to verify the code prior to uploading. The *Arrow* button uploads the sketch to
the device.  

## Troubleshooting

If the Arduboy doesn't upload your sketch, try the following procedure:

* Turn off device. 
* Press upload, on ide.  
* Then hold reset. 
* While holding reset press power, then let go of reset.  It will work.

Alternatively:

* Press and -hold- reset button (paperclip)
* Hit upload
* Wait for the “Sketch uses xxx bytes” message 
* Count to 4
* Release reset button

Further information on how to unbrick it [here](http://community.arduboy.com/t/how-to-reset-arduboy-if-its-bricked/807).

## Image Conversion

We used [this tool](https://github.com/yyyc514/img2ard) to convert images.  It has its own documentation.

## Resources

* Start learning with the [Arduboy Quick Start](http://community.arduboy.com/t/arduboy-quick-start-guide/725)
* [Arduboy Library](https://github.com/Arduboy/Arduboy/tree/master/src) - not much for documentation but reading the source shows what it does
* [Some documentation for Arduboy Library](https://asmcbain.net/projects/arduboy/docs/1.2/api/index.html#arduboy)
* [Image Encoding Utility](http://www.andrewlowndes.co.uk/blog/graphics/arduboy-image-converter) - we didn't use this one in the end
* [Reset Procedure if you brick device](https://www.youtube.com/watch?v=l1ifTSq0VK4)
* [Information about Memory](https://www.arduino.cc/en/Tutorial/Memory) we are space constrained and need to pay attention to this
* [Information on use of PROGMEM](https://www.arduino.cc/en/Reference/PROGMEM)

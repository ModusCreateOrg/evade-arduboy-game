# Evade - A Space Shooter Game for Arduboy by Modus Create

This is an Arduboy game, written as a gift for [Modus Create](http://moduscreate.com) clients for the 2016 holiday season.

![Arduboy](arduboy.gif)

## Quick Start

You will need an Arduboy, which you can get from:

* [Arduboy](https://www.arduboy.com/store/products/arduboy) (different case colors available)
* [Adafruit] (https://www.adafruit.com/products/3264) (white case only)

Ardubo is Arduino based.  A program for the Arduino is referred to as a "Sketch".  To install our game on your Arduboy, you will need to get the Arduino IDE.

The IDE can be downloaded here:

[Arduino IDE](https://www.arduino.cc/en/Main/Software)

Once you have it installed follow the steps under Resources to install the Arduboy library.

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

## JavaScript Tool for Converting Strings to Bitmap Font Representations

We created a small JavaScript tool (`tools/text_generator.js` in this repo) to convert arrays of strings into arrays of integers representing the characters in the bitmap font that we use.  This can be run directly in [JS Fiddle](https://jsfiddle.net/).

## Resources

* Start learning with the [Arduboy Quick Start](http://community.arduboy.com/t/arduboy-quick-start-guide/725)
* [Arduboy Library](https://github.com/Arduboy/Arduboy/tree/master/src) - not much for documentation but reading the source shows what it does
* [Some documentation for Arduboy Library](https://asmcbain.net/projects/arduboy/docs/1.2/api/index.html#arduboy)
* [Image Encoding Utility](http://www.andrewlowndes.co.uk/blog/graphics/arduboy-image-converter) - we didn't use this one in the end
* [Reset Procedure if you brick device](https://www.youtube.com/watch?v=l1ifTSq0VK4)
* [Information about Memory](https://www.arduino.cc/en/Tutorial/Memory) we are space constrained and need to pay attention to this
* [Information on use of PROGMEM](https://www.arduino.cc/en/Reference/PROGMEM)

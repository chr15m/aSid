![aSid Logo](./img/aSid.png)

aSid is a program which runs on your Commodore 64, and turns it into a moog-like monophonic synthesizer. Connect paddles to your commodore 64 and use each paddle to control a different parameter. The synthesizer is played by using the lower two rows of commodore keys like piano keys. You can also run sounds into your C64 using the A/V in port and the filters will affect the sound as it passes through the machine.

## Use ##

aSid comes as a binary file that has been compiled for the Commodore 64 computer. You can download the binary file from <./bin> and then either run it in an emulator, or copy it to an [cartridge](https://encrypted.google.com/search?hl=en&q=c64%20turbo%20cartridge), or a diskette, or tape, and load it into an actual C64. Running it on the hardware gives the best results as every 6581 SID chip sounds unique.

## Instructions & Keys ##

 * octave
  * '<' down one octave
  * '>' up one octave

 * wave-form
  * 'Y' square wave
  * 'T' sawtooth wave
  * 'R' triangle wave
  * 'U' noise wave

 * paddle 1 filter type
  * 'Q' lowpass
  * 'W' band pass
  * 'E' high pass
 
 * paddle 2 filter type
  * 'I' volume control
  * 'O' Ring modulator (when waveform is set to triangle wave)
  * 'P' "programmed" ring modulation (works on external sounds)

## Develop ##

Compiled with Ullrich von Bassewitz `cc65` compiler.

## License ##

aSid is [Free Software](http://www.wikipedia.org/wiki/Free_Software) and is licensed to you under the terms of the GNU Public License. Copyright Chris McCormick 2001-2015.

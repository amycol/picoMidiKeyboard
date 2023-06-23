# picoMidiKeyboard
 Pi Pico code I wrote to convert an old Casio keyboard to midi.

 I used:
 Casio SA-46
 Pi Pico

 Overall, so far this project has only cost me the £3.90 I paid for the Pico, since I had the keyboard already. Note that this doesn't include the cost for solder though.

 The Casio SA-46 I used has a 4x8 key matrix, so all the code is written for that currently, although I may make it more configurable at some point.
 
 I have not soldered the keyboard to the Pico yet, nor have I put the Pico in the keyboard's casing, which is why the list is so short. I will probably use solder (ofc), a micro USB extension (or a port breakout and some wire, might even use USB-C if I can find a breakout for cheap enough) and possibly some 3D printed parts. I'm hoping to not have to put any holes or cutouts in the keyboard's casing but I might have to if I can't get the old IO(?) ports out. Unfortunately I did have to cut the ribbon cable that connected the keyboard PCB to the main PCB since I could not lift out the main board to desolder it, I think it was glued or something but I didn't want to put too much force on it and risk breaking it since I do plan to add on to this project in the future with a switch or something to switch the keyboard's connection between the Pico and the original main board.
 
 Things I plan on adding/changing in the future:
 - A switch or button to switch the keyboard's connection between the Pico and the original main board
 - Code to indicate when a key is detected as pressed on the Pico's built in LED (at the moment the LED just blinks different speeds to indicate the USB connection status, as is default in TinyUSB)
 - A potentiometer to control the key velocity reported over midi/USB since the keyboard I wrote this for is just a basic membrane keyboard
 - A second Pico to optionally switch the midi output of the first Pico to, which converts the midi signal to audio, with different sound options and possibly a digital synthesizer (might have to use a Pi Zero/2/W if going the digital synth route)(I don't know if I'll do this or not, it mainly depends on if I manage to make it switchable to the old board as I mentioned above, and if I feel like upgrading the main board, as well as if I miss the "untethered" functionality the keyboard had before this conversion)
 - A 3D printed case (If I can find a way to join large prints together in a way that is strong enough and doesn't feel cheap, since my printer's bed is about half the size of the keyboard)
 - Wireless functionality (depends very much on if I can find a protocol for wireless midi)
 - Cleaner code
 - Customisable options such as key matrix and additional options

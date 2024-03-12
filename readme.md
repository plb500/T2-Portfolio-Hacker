# Terminator 2 replica Atari Portfolio "PIN Hacker" and Cyberdyne keypad prop

- Latest Atari Portoflio executable is [here](https://github.com/plb500/T2-Portfolio-Hacker/releases/latest)
- Atari Portfolio exectuable C source code and build instructions are [here](pofo_src)
- Credit card graphics are [here](docs/graphics/Credit%20Card)
- Keypad replica code is [here](keypad)
- Schematics/code for the electronics box are [here](hack_box)

A project born out of insanity and obsession.

When I was a kid I was obsessed with the little computer John Connor had in T2. 

I decided to make a "working" prop replica - something that would actually use the Atari Portoflio to "hack" a locked container. This is that.

There is MUCH more to this story. Getting a replica of the credit card is a whole story in itself, but I don't have the energy to document all this right now

https://www.youtube.com/watch?v=ELD_mX-Tf1Y

## Parts List

- For the Atari Portfolio
  - Atari Portfolio (HPC-004)
  - Atari Portfolio Serial Interface (HPC-102)
  - Atari Portfolio memory card (64K will do)
- For the "Hacker box"
  - Radio Shack RS-232C cable, model number 26-269, red box (for maximum authenticity). Failing that any female DB9 to male DB25 cable will do.
  - 10 pin ribbon cable (3-4')
  - Copper/conductive tape
  - Aluminium tape
  - Black electronics enclosure, approx. dimensions: 6.5" x 2.75" 1.125" (I used a Hammond enclosure, model 1591HBK)
  - Female DB25 panel mount connector
  - Fake credit card (if you're really dedicated you can use the SVG files in this repo to get your own replicas printed. Or just buy/use any old credit card)
- For the secure terminal/safe
  - Raspberry Pi (I use a model 3, you can almost certainly get away with older models if you can find a touchscreen that works)
  - Spring loaded pins with solder cup ([Digikey link](https://www.digikey.ca/en/products/detail/mill-max-manufacturing-corp/7949-0-15-20-09-14-11-0/13531603))
  - 4.3" DSI Pi touchscreen ([Waveshare link](https://www.waveshare.com/4.3inch-dsi-lcd.htm))
  - 16x1 LCD I2C character display ([BuyDisplay link](https://www.buydisplay.com/lcd-display-16x1-datasheet-in-pdf-hd44780-controller-i2c-arduino))
  - Plasti/milled housing (more details coming)
  - 3mm LEDs, 1 red, 1 green.
  - 12v solenoid lock
  - Some kind of cabinet to install the above lock in. Reader's choice. I used an old janky lockbox. 

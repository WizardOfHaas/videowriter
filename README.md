# videowriter
USB protocol conversion software for Magnavox VideoWriter keyboards

![keyboard](https://i.imgur.com/7oRfT3C.jpg)

## What is a VideoWriter?

The Magnavox VideoWriter was a stand alone word proccessor system produced in the late 80s. On it's own, that's not veryu interesting. What matters for this project is that it came with a strange 70%-ish keyboard that uses a bizzare porotocol for data transfer.

## The Keyboard

Near as I can tell the VideoWriter came with 2 main keyboard options, one using the rare Alps SKCM Brown switches and another that used tactile Mitsumi KPQ Hybrid switches. The one that I have uses the later Mitsumi switches.

## The Motive

There are existing projects that convert VideoWriter keyboards to USB by replacing the origional keyboard controller. This is the definite better solution. However, I liked the idea of attempting a protocol-based solution.

## The Protocol

As stated before, the keyboard protocol is somewhat unique. Thanks to some Deskthority posts and tinkering I was able to discern the following. The ogitional system used a phone cable style connector with 4 pins: GND, +5v, Data, and ... something I'll call the Control pin for lack of a better term. While Control is held low Data sends keystrokes as 9600 baud serial packets.

What is in those packets? Well, it's kind of a mix. First of all, the board only sends key down events, no key up codes of any kind(except for Caps Lock, more on that later).

The alpha-block keys all send ASCII from code 32 - 126. The Shift keys are handled internally to the keyboard so there are no packets sent for pressing Shift(i.e. 'Shift + 1' just sends '!').

Unsuprisingly, all the keys outside the alpha-block send special codes above 128. For most of these Shift also acts as a modifier(i.e. 'Shift + Tab' sends a seperate code from just 'Tab'). This made it relatively easy to just set up an array to transpose special keys to whatever mapping I wanted.

The next high strangeness comes from the Caps Lock key. This is an actual locking switch, at least on the Mitsumi board I have. When on all special keys are unchanged, but all the alpha-block keys are shifted down by 97(so a -> 1, b -> 2...). It's odd behaviour, at least to me. The good part here is that I can just feed those codes through my transform array to remap them to whatever special keys I want.

## The Conversion

The physical layer is handled by a Trinket M0, but any microcontroller with Keyboard HID support and a UART should work fine. As I mentioned above, alpha-block characters are sent through without change and special characters are mapped using an array.

The control and alt keys are handled specially. I mapped those to "DO" and "Insert" on my board. Since there are no key release codes sent I setup these keys to work as CTRL-Lock and ALT-lock keys, think sticky-keys.

## Next Steps

Right now the keyboard basically works using this code. My plans for the future are to refactor the code since it is pretty kludge-heavy. I also plan on changing the transform mapping proccess to allow for macros.

# Swap DRC for TV-Only Wii U Games

Introducing Swap DRC, a homebrew application that allows you to play TV-only Wii U games on your GamePad screen.

Swap DRC works by switching the display buffers for the TV and the GamePad with a press of a button. This app also includes the TCPGecko and Cafiine installers for other mods or hacks. The GamePad sensor bar can also be turned on when the TV display is on the GamePad screen, so playing with Wiimotes with just the GamePad is possible.

Update v1.1: Audio swapping support is now available!

## Instructions

To swap the TV and GamePad screens, press and hold the **L shoulder button**, then press **MINUS**. Pressing the button combination again will revert the displays back to normal.

If you're playing with Splatoon's enhanced swapping controls:

+ Press **B** to switch screens except in-game menus.
+ Hold **A** + **D-Pad** in any direction to super jump to a teammate or the spawn point during a match.

## Requirements

* An SD card
* The Homebrew Launcher ([Haxchi](https://gbatemp.net/threads/haxchi-v2-0-a-persistent-wiiu-hack.451071/) or [Loadiine.ovh](http://loadiine.ovh) method)
* A Wii U (on firmware version 5.5.1, 5.5.2 and lower versions aren't tested but may work)
* (Optional) A cafiine server (examples: [the original by Chadderz and MrRean](https://github.com/MrRean/Cafiine-410-551/blob/master/server/cafiine_server.exe), [MusicRandomizer](https://github.com/OatmealDome/SplatoonUtilities/blob/master/MusicRandomizer/README.md), [Ray's custom server](https://github.com/Syroot/CafiineServer), and more)
* (Optional) Your Wii U's IP for TCPGecko or your computer's local IP for Cafiine ([Windows](http://www.nirsoft.net/utils/wnetwatcher.zip), [macOS](http://osxdaily.com/2010/11/21/find-ip-address-mac/))
* For Splatoon's enhanced controls: ver 2.10.0 - 2.12.0


## Tutorial

### Video tutorial:

    (coming soon)

### Text tutorial:

1. Download the Homebrew Launcher v1.4 from [**here**](https://github.com/dimok789/homebrew_launcher/releases), and the swapdrc.elf application from [**here**](https://github.com/OatmealDome/geckiine_drc_swap/releases).

2. Unzip both apps and then drag them into your SD Card. Make sure both directories are at:

    * SD:/wiiu/apps/homebrew_launcher/homebrew_launcher.elf
    * SD:/wiiu/apps/swapdrc/swapdrc.elf

3. Safely remove the SD card and insert it into your Wii U, then launch the Homebrew Launcher from the [Loadiine.ovh](http://loadiine.ovh) website in your Wii U browser. If you have the [Haxchi](https://gbatemp.net/threads/haxchi-v2-0-a-persistent-wiiu-hack.451071/) Homebrew Launcher, that works too.

4. Press **A** to install Swap DRC with TCPGecko, or press **Y** to install Swap DRC with TCPGecko + Cafiine (a cafiine server on your computer is required)

5. You're done! You can now switch displays anywhere (but not in the settings menu, as it re-launches the entire System Menu).

# FAQ

### What's TCPGecko and Cafiine?

    TCPGecko is a mod that allows you to use a TCPGecko editor to modify the ram through a TCP connection.
    A perfect example for this is the program Splathax, which lets you modify gear in your Splatoon
    inventory.
    
    Cafiine is another mod that lets you replace game files with modified ones for the game to load. A
    perfect example for this is the popular use of model and skin edits for Smash Bros for Wii U. Unlike the
    "Install TCPGecko only" option, Cafiine requires you to have a server running on your computer at all
    times. Don't use this if you don't want to use game file mods and have the computer running all the time.
    
### Can I get you banned playing online with Swap DRC in any game?

    Swapping displays doesn't modify the games directly but through CafeOS instead. Therefore, it is 100%
    safe. However, you can still get banned with TCPGecko and/or Caffine mods if you don't use them properly.
    
### r u a l33t hax0r?

    YeS I Am.
    (...I'm joking this question isn't real)


# Credits

### Creators:

+ OatmealDome
+ Yahya14

### Special thanks:

+ Maschell for HID to VPAD and the audio swap
+ Dimok for function_hooks and the pygecko server
+ Brienj for the IP selector and initial UI
+ /u/MachMatic for the banner background
+ BKOOL999 for feedback & testing the Swap DRC app

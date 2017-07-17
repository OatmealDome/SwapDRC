# SWAP DRC for TV-Only Wii U Games

Introducing Swap DRC, a homebrew application that allows you to play TV-only Wii U games on your gamepad screen.

Swap DRC works by switching display buffers between the TV screen and the gamepad screen with a press of a button activation. This app also includes the TCPGecko and Cafiine installers for the other mediums of modding. The gamepad sensor bar can also switch on when the TV display is switched to the gamepad screen, so playing with wiimotes with just the gamepad is very possible.

Note that this mod has no audio swapping support, thus use headphones to listen to the audio from your gamepad if your game requires it.

## Instructions

To switch the TV and gamepad display buffers, press and hold the **L shoulder button**, then press **MINUS**. Switching them again will revert the displays back to normal.

If you're playing with Splatoon's enhanced swapping controls;

+ Press **B** to switch screens except in-game menus.
+ Hold **A** + **D-PAD** in any direction to super jump to a teammate or the spawn point during a match.

## Requirements

* An SD card
* The Homebrew Launcher ([Haxchi](https://gbatemp.net/threads/haxchi-v2-0-a-persistent-wiiu-hack.451071/) or [Loadiine.ovh](http://loadiine.ovh) method)
* A Wii U (on firmware version 5.1.1, lower versions aren't tested)
* (Optional) A cafiine server (examples: [the original by Chadderz and MrRean](https://github.com/MrRean/Cafiine-410-551/blob/master/server/cafiine_server.exe), [MusicRandomizer](https://github.com/OatmealDome/SplatoonUtilities/blob/master/MusicRandomizer/README.md), [Ray's custom server](https://github.com/Syroot/CafiineServer), and more)
* (Optional) Your Wii U's IP for TCPGecko or your computer's local IP for Cafiine ([Windows](http://www.nirsoft.net/utils/wnetwatcher.zip), [macOS](http://osxdaily.com/2010/11/21/find-ip-address-mac/))
* For Splatoon's enhanced controls; ver 2.10.0 - 2.12.0


## Tutorial

### Video tutorial:

    (coming soon)

### Text tutorial:

1. Download the homebrew launcher from [here](https://github.com/dimok789/homebrew_launcher/releases) (download v1.3 if v2.1 crashes for you), and the swadrc.elf application from here (soonTM).

2. Unzip both apps and then drag them into your SD Card, make sure both directories are at:

    * SD:/wiiu/apps/homebrew/app.elf
    * SD:/wiiu/apps/swapdrc/swapdrc.elf

3. Safely remove the SD card and insert it into your wii u, then launch the hombrew launcher from [loadiine.ovh](http://loadiine.ovh) in your Wii U browser. If you have the [haxchi](https://gbatemp.net/threads/haxchi-v2-0-a-persistent-wiiu-hack.451071/) homebrew launcher, that works too.

4. Press **A** to install Swap DRC with TCPGecko, or press **B** to install Swap DRC with TCPGecko + Cafiine (cafiine server on your computer is required)

5. You're done! You now switch displays anywhere but not in the settings menu (because it terminates any mod installers like it always does).

# FAQ

### What's TCPGecko and Cafiine?

    TCPGecko is a mod that allows you to use a TCPGecko editor to modify the ram through a TCP connection. A
    perfect example for this is the use of Splathax that let's you modify gears in your inventory in Splatoon.
    
    Caffine is another mod that lets you replace modified game files that you'd like the game to load. A perfect
    example for this is the popular use of model and skin edits for Smash Bros for Wii U. Unlike the "Install
    TCPGecko only" option, Caffine requires you to have a server running on your computer at all times. Don't
    use this if you don't want to use game file mods and have the computer running all the time.
    
### Can I get you banned playing online with Swap DRC in any game?

    Swapping displays doesn't modify the games directly but through the Cafe OS instead. Therefore it's 100% safe.
    However you can still get banned with TCPGecko and/or Caffine mods if you don't use them properly.
    
### r u a l33t hax0r?

    YeS I Am.
    (...I'm joking this question isn't real)


# Credits

### Creators:

+ OatmealDome
+ Yahya14

### Special thanks:

+ Maschell for HID to VPAD
+ Dimok for function_hooks and the pygecko server
+ Brienj for the IP selector and initial UI
+ /u/MachMatic for the banner background
+ BKOOL999 for feedback & testing the Swap DRC app





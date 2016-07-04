# geckiine

This tool installs both TCPGecko and Cafiine into the system memory so that you can use them at the same time.

## Usage

### Prerequisites
* Wii U (on firmware version 4.1.0 or higher)
* Homebrew Launcher
* a cafiine server (ex the original by MrRean, [MusicRandomizer](https://github.com/OatmealDome/SplatoonUtilities/blob/master/MusicRandomizer/README.md), [Ray's custom server](https://github.com/Syroot/CafiineServer), etc)
* Your computer's local IP ([Windows](https://support.microsoft.com/en-us/help/15291/windows-find-pc-ip-address), [macOS](http://osxdaily.com/2010/11/21/find-ip-address-mac/))

### Instructions

Note: It is highly recommended that you set a static IP for your computer when using geckiine. For more information, click [here](https://github.com/OatmealDome/SplatoonUtilities/blob/master/Misc/StaticIPGuide.md).

1. There are two possible ways to get a geckiine.elf file with your IP address.

   a. Go to [466gaming.ga/geckiine](http://466gaming.ga/geckiine) and enter in your IP address. Click the download button and extract the ZIP file

   b. Download ```geckiine.zip``` with Geckiine Patcher from [here](https://github.com/seresaa/geckiine-creator/releases/tag/v0.1) and extract it. Run ```Geckiine Creator.exe```, enter in your IP address into the four boxes and click the "Create/Patch ELF" button.
3. Copy the "geckiine" folder to the ```wiiu/apps/``` folder on your SD card.
4. Insert your SD card into the Wii U and launch the Homebrew Launcher from [loadiine.ovh](http://loadiine.ovh).
5. Start a cafiine server on your computer.
6. Scroll to the "geckiine" application and press A to start it. You will be returned to the Wii U menu.
7. Launch the game you want to modify.
8. Run any TCPGecko scripts and/or connect with TCPGecko dotNET/JGeckoU.

## Credits

* Maschell for HID to VPad
* Dimok for his amazing function_hooks system and pygecko server
* NWPlayer123 for the cafiine cient source code
* amiibu for helping me with the PHP script and giving me name ideas
* seresaa for putting together the banner, for name ideas, and for writing Geckiine Creator
* 466 for web hosting
* /u/MachMatic for creating the banner background

Shoutouts to everyone at the Splatoon Modding Hub Discord, because I probably annoyed them by talking endlessly about the various problems I encountered.

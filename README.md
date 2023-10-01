# STM32MP135 Dev Board Buildroot
  
A Buildroot external tree for a custom PCB based on an [MYiR SoM](https://www.myirtech.com/list.asp?id=726).
  
TODO: Add PCB Design Files

## Current State
The build is very similar to the mainline STM32MP157c-DK2 device tree however there are some minor changes due to the pinout of the SoM, and the RAM configuration. Not all of module-specific pinouts have been implemented yet.
* Uses Linux kernel 6.3
* Uses U-Boot 2023.07
* Tested with Buildroot 2023.05
* ARM Trusted Firmware 2.9 with *opteeos* for BL32.
* Uses mainline STM32 dtsi where possible, however some of the dt-bindings for the STM32MP135 have not made it to the mainline u-boot and Linux source yet. The missing and incomplete files have been included here and should be removed when they are no longer needed.
* Custom dts and dtsi are included for board specific changes. Not all of the SoM specific pinouts are reflected in the files.
* Includes a file system overlay with some configuration, but ```boot/extlinux.conf``` is the only file which is actually required.
* Some other files in the **overlay** folder are just to override defaults or test features but are not strictly required. 

## Usage
This *should* behave like any external buildroot tree. General steps for use:
* Clone this repository, as well as Buildroot.
* In the **Buildroot** directory, create a config file based on this external tree:
```
BR2_EXTERNAL=../STM32MP135_Dev_Board_Buildroot make stm32mp135_dev_board_defconfig
```
* Review or modify the system in ```make menuconfig```.
* Build the image using ```make```.
* **NOTE:** There is an issue with UBoot the first time you try to build, see below.
* Use **dd**, **Rufus**, or your software of choice to image an SD card with the *output/images/sdcard.img* file.

## Issues
There are so many! But some big ones:
* U-Boot does not include the custom DTB file in the Makefile when building for the first time. If you get errors about the dtb not being correctly specified, manually adding the dtb filename to the file *buildroot/output/build/uboot-2023.04/arch/arm/dts/Makefile*:
  
```
stm32mp135_dev_board.dtb
```
  
(Note: Make sure to add this line under the appropriate IF statement in the Makefile.)

### The Overlay Folder
There are a few config files in the overlay file for either quality of life, or to make things actually work. Most are optional depending, or may even conflict with your desired settings.
* **\boot\extlinux.conf** - Required to boot linux. U-Boot will look for this.
* **\etc\init.d\S40xorg** - A dummy file to overwrite a default which is creatred when Xorg compiles. I don't want Xorg to start on boot.
* **\etc\modprobe.d\esp.conf** - Config file telling the esp8089 driver which GPIO to use for reset. Since the driver doesn't work anyway, this will probably be removed.
* **\etc\opkg\distfeeds.conf** - Contains OpenWRT package feed. Need to force it to search the correct architecture when using opkg.
* **\etc\wpa_supplicant\wpa_cupplicant.conf** - A blank *wpa_supplicant* configuration to fill out.
* **\etc\X11\xorg.conf** - Sometimes the default are bad, sometimes they aren't.
* **\etc\directfbrc** - DirectFB config file because it doesn't seem to use FBDev by default, even though it says it does.
* **\lib\firmwmare** - Firmware files for some wifi cards.
* **\root** - Just some extra files for testing and playing with.

## Trips and Tricks
Some things that caught me out, or helped me out, along the way:
* I had trouble figuring out how to convince ARM Trusted Firmwawre to build a FIP package from within Buildroot. For reference, or if you want, or need, to create one manually you can do so from within the *buildroot/output/build/arm-trusted-firmware/build/stm32mp1/debug* directory:<br />
```
fiptool create --tos-fw bl32.bin --fw-config fdts/stm32mp135_dev_board-tf-a-fw-config.dtb --hw-config u-boot.dtb --nt-fw u-boot.bin --tos-fw-config fdts/stm32mp135_dev_board-tf-a-bl32.dtb fip.bin
```
  
Where the *u-boot.bin*, and *u-boot.dtb* files are found in the *buildroot/output/build/uboot* directory.

* Remember that the FIP package contains the second stage bootloader, and loads the U-Boot DTB file. So if you modify the device tree you will need to manually rebuild U-Boot, **and** ARM Trusted Firmware, and probably the Linux kernel as well depending on your changes. 
 1. ```make linux-rebuild```
 2. ```make uboot-rebuild```
 3. ```make arm-trusted-firmware-rebuild```
 4. Finally; ```make``` to generate the image.
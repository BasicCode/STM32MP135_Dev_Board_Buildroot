# STM32MP135 Dev Board Buildroot
  
A Buildroot external tree for a custom PCB based on an [MYiR SoM](https://www.myirtech.com/list.asp?id=726).
  
TODO: Add PCB Design Files

## Current State

* Uses Linux kernel v6.1 (from STM)
* Uses U-Boot 2023.07 (from STM)
* Uses TF-A v2.8 (from STM)
* Uses Optee-Os v3.19.0 (from STM)
* Tested with Buildroot 2023.05
* ARM Trusted Firmware 2.8 with *opteeos* for BL32. (from STM)
* Uses mainline STM32 dtsi where possible, however some of the dt-bindings for the STM32MP135 have not made it to the mainline u-boot and Linux source yet. Fortunately STM provides customised TF-A, Optee, U-Boot, and Linux.
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
* After an initial build, on first boot, you will get errors about Optee Header location:
```
WARNING: The load address in optee header 0xce000000 - 0xce081310 is not in reserved area: 0xce200000 - 0xd0000000.
ERROR:   OPTEE header parse error.
```
Modify / replace the *conf.mk* file in *output/build/optee-os-custom/core/arch/arm/plat-stm32mp1* to support 256MB RAM. A replacement file is included (*conf.mk.new*). Further information on the process is described by STM [here](https://wiki.stmicroelectronics.cn/stm32mpu/wiki/How_to_configure_a_256MB_DDR_mapping_from_STM32_MPU_Distribution_Package).
  
* There is a problem with the PCM3060 driver reporting which format the codec supports. Although it seems to be correct, it doesn't seem to work. As a workaround modify the file *linux-custom/sound/soc/codecs/pcm3060.c* on line 191 change the *.format* to ```SNDRV_PCM_FMTBIT_S32_LE```. TODO: Create a patch file for this.

### The Overlay Folder
There are a few config files in the overlay file for either quality of life, or to make things actually work. Most are optional depending, or may even conflict with your desired settings.
* **/boot/extlinux.conf** - Required to boot linux. U-Boot will look for this.
* **/etc/init.d/S40xorg** - A dummy file to overwrite a default which is creatred when Xorg compiles. I don't want Xorg to start on boot.
* **/etc/opkg/distfeeds.conf** - Contains OpenWRT package feed. Need to force it to search the correct architecture when using opkg.
* **/etc/wpa_supplicant/wpa_cupplicant.conf** - A blank *wpa_supplicant* configuration to fill out.
* **/etc/X11/xorg.conf** - Sometimes the default are bad, sometimes they aren't.
* **/etc/directfbrc** - DirectFB config file because it doesn't seem to use FBDev by default, even though it says it does.
* **/lib/firmwmare** - Firmware files for some wifi cards. [SiliconLabs GitHub](https://github.com/SiliconLabs/wfx-firmware).
* **/root** - Just some extra files for testing and playing with.

## Trips and Tricks
Some things that caught me out, or helped me out, along the way:
* I had trouble figuring out how to convince ARM Trusted Firmwawre to build a FIP package from within Buildroot. For reference, or if you want, or need, to create one manually you can do so from within the *buildroot/output/build/arm-trusted-firmware/build/stm32mp1/debug* directory:<br />
```
fiptool create --tos-fw bl32.bin --fw-config fdts/stm32mp135_dev_board-tf-a-fw-config.dtb --hw-config u-boot.dtb --nt-fw u-boot.bin --tos-fw-config fdts/stm32mp135_dev_board-tf-a-bl32.dtb fip.bin
```
  
Where the *u-boot.bin*, and *u-boot.dtb* files are found in the *buildroot/output/build/uboot* directory.

* Remember that the FIP package contains the second stage bootloader, and loads the U-Boot DTB file. So if you modify the device tree you will need to manually rebuild U-Boot, **and** ARM Trusted Firmware, and probably the Linux kernel as well depending on your changes. 
 1. ```make optee-os-rebuild```
 2. ```make linux-rebuild```
 3. ```make uboot-rebuild```
 4. ```make arm-trusted-firmware-rebuild```
 5. Finally; ```make``` to generate the image.

* I had to install *libssl-dev* for the OpenSSL Headers in order to build ATF.
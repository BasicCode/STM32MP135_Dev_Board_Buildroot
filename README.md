# STM32MP135 Dev Board Buildroot
  
A Buildroot external tree for a custom PCB based on an [MYiR MYC-YF135-256N SoM](https://www.myirtech.com/list.asp?id=726).
  
TODO: Add PCB Design Files
  
### Harwdare
* STM32MP135DAF7 inside the [MYC-YF135-256N SoM](https://www.myirtech.com/list.asp?id=726) from MYiR Electonics.
  * 1 GHz
  * 256MB RAM
  * 256MB NAND (not used in this example)
* [Silicon Labs WF200D](https://www.silabs.com/documents/public/data-sheets/wf200-datasheet.pdf) WiFi chipset.
* [Ti PCM3060](https://www.ti.com/lit/ds/symlink/pcm3060.pdf) audio codec.
* 40-pin LCD, and 6-pin CTP connector compatible with [Orient Display 480x272](https://www.orientdisplay.com/pdf/AFY480272B0-4.3N12NTM-C.pdf) TFT LCD panel.


## Current State
* Uses Linux kernel v6.1 (from STM)
* Uses U-Boot 2023.10 (from STM)
* Uses TF-A v2.1 (from STM)
* Uses Optee-Os v4.0.0 (from STM)
* Tested with Buildroot 2023.05
* Uses mainline STM32 dtsi where possible, however some of the dt-bindings for the STM32MP135 have not made it to the mainline u-boot and Linux source yet. Fortunately STM provides customised TF-A, Optee, U-Boot, and Linux.
* Includes a file system overlay with some configuration, but ```boot/extlinux.conf``` is the only file which is actually required.
* Some other files in the **overlay** folder are just to override defaults or test features but are not strictly required. 

## Usage
Use like any external buildroot tree:
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
* A patch is included to modify the *conf.mk* file in *output/build/optee-os-custom/core/arch/arm/plat-stm32mp1* to support 256MB RAM. Further information on the process is described by STM [here](https://wiki.stmicroelectronics.cn/stm32mpu/wiki/How_to_configure_a_256MB_DDR_mapping_from_STM32_MPU_Distribution_Package). If you're getting errors about things being in the wrong memory location it could be a related issue.
```
WARNING: The load address in optee header 0xce000000 - 0xce081310 is not in reserved area: 0xce200000 - 0xd0000000.
ERROR:   OPTEE header parse error.
```
* NOTE: That it seems as though the NAU88c22 chip will only support I2C bus speeds up to 100khz. Although it will respont to ACK requests at a higher speed, it does not reset properly at this speed.
  
## Bugs and TODO
* Some sort of soft start or larger capacitor is required across VSYS on BQ256xx chip (https://e2e.ti.com/support/power-management-group/power-management/f/power-management-forum/1021465/faq-why-vsys-does-not-start-up-normally-when-there-is-a-loading-on-the-sys-and-the-battery-is-not-present-for-bq25601-bq25601d-bq25606-bq25611d-bq25616-bq25616j-bq25618-bq25619-bq25600-bq25600d-bq25600c).
* Extra capacitor across VSYS to help with ripple
* Add a choke, or something, to help switchmode noise getting back up the power input.
* Add DC blocking capacitors to speaker outputs
* MMC CD line is incorrect
* A better wifi chipset
* LCD backlight voltage too damn high; change FB resistors (150k, 6.8k seems like a good number).
* Check if touch panel is even working.
* Charge and Power LEDs are wrong polarity.

### The Overlay Folder
There are a few config files in the overlay file for either quality of life, or to make things actually work. Most are optional depending, or may even conflict with your desired settings.
* **/boot/extlinux.conf** - Required to boot linux. U-Boot will look for this.
* **/etc/init.d/S40xorg** - A dummy file to overwrite a default which is creatred when Xorg compiles.
* **/etc/opkg/distfeeds.conf** - Contains OpenWRT package feed. Need to force it to search the correct architecture when using opkg.
* **/etc/X11/xorg.conf** - Sometimes the default are bad, sometimes they aren't.
* **/etc/directfbrc** - DirectFB config file because it doesn't seem to use FBDev by default, even though it says it does.
* **/lib/firmwmare/wfx** - Firmware files for some wifi cards. [SiliconLabs GitHub](https://github.com/SiliconLabs/wfx-firmware).
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

* The WiFi was getting buffer errors when the SPI bus was running at 42MHz. It seems to be fine at 24MHz, it might be able to run a bit quicker but I haven't tried yet.

* The */etc/X11/xinit/xinitrc* file, by default, opens several xterm windows and starts a window manager. You may want to check and modify the last few lines of this file to suit your needs.

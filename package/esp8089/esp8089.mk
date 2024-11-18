ESP8089_VERSION = ec7282656d541046ed8bbdff8950cb57e3c0f2e9
ESP8089_SITE = $(call github,Evlers,esp8089_wifi_drivers,$(ESP8089_VERSION))
ESP8089_LICENSE = GPLv2
ESP8089_LICENSE_FILES = COPYING
 
define KERNEL_MODULE_BUILD_CMDS
        $(MAKE) -C '$(@D)' LINUX_DIR='$(LINUX_DIR)' CC='$(TARGET_CC)' LD='$(TARGET_LD)' modules
endef
 
$(eval $(kernel-module))
$(eval $(generic-package))
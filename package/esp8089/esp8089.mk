ESP8089_VERSION = 1.9.20230804
ESP8089_SITE = $(call github,al177,ESP8089,$(ESP8089_VERSION))
ESP8089_LICENSE = GPLv2
ESP8089_LICENSE_FILES = COPYING
 
define KERNEL_MODULE_BUILD_CMDS
        $(MAKE) -C '$(@D)' LINUX_DIR='$(LINUX_DIR)' CC='$(TARGET_CC)' LD='$(TARGET_LD)' modules
endef
 
$(eval $(kernel-module))
$(eval $(generic-package))
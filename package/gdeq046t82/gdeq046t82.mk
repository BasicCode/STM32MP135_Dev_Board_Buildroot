GDEQ046T82_VERSION = 7da18241a23a9f71b44c92a9d8654df496693add
GDEQ046T82_SITE = $(call github,BasicCode,GDEQ046T82_Linux_Framebuffer,$(GDEQ046T82_VERSION))
GDEQ046T82_LICENSE = GPLv3
GDEQ046T82_LICENSE_FILES = COPYING
 
define KERNEL_MODULE_BUILD_CMDS
        $(MAKE) -C '$(@D)' LINUX_DIR='$(LINUX_DIR)' CC='$(TARGET_CC)' LD='$(TARGET_LD)' modules
endef
 
$(eval $(kernel-module))
$(eval $(generic-package))
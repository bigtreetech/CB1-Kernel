# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.
#
# MediaTek Inc. (C) 2017. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
#
# The following software/firmware and/or related documentation ("MediaTek Software")
# have been modified by MediaTek Inc. All revisions are subject to any receiver's
# applicable license agreements with MediaTek Inc.

LOCAL_PATH := $(call my-dir)

###############################################################################
#################### KERNEL ENVIRONMENT SETUP #################################
#
# Align variable in Kernel-x.xx/Android.mk
#
###############################################################################

ifeq ($(PRODUCT_OUT),)
    $(error PRODUCT_OUT is not defined)
endif

ifeq ($(LINUX_KERNEL_VERSION),)
    $(error LINUX_KERNEL_VERSION is not defined)
endif

ifneq ($(ANDROID_PRODUCT_OUT),)
export ALPS_OUT=$(ANDROID_PRODUCT_OUT)
else
export ALPS_OUT=$(PRODUCT_OUT)
endif

# set for module Makefile
export KERNEL_DIR=$(realpath $(LINUX_KERNEL_VERSION))
ifeq ($(KERNEL_OUT),)
export KERNEL_OUT=$(ALPS_OUT)/obj/KERNEL_OBJ
endif

# check kernel folder exist
ifeq (,$(wildcard $(KERNEL_DIR)))
    $(error kernel $(KERNEL_DIR) is not existed)
endif

ifeq ($(KERNEL_CROSS_COMPILE),)
ifeq ($(TARGET_ARCH), arm64)
  export KERNEL_CROSS_COMPILE=$(KERNEL_DIR)/$(TARGET_TOOLS_PREFIX)
else
  export KERNEL_CROSS_COMPILE=$(KERNEL_DIR)/prebuilts/gcc/$(HOST_PREBUILT_TAG)/arm/arm-eabi-$(TARGET_GCC_VERSION)/bin/arm-eabi-
endif
endif

# check cross compiler exist
ifeq (,$(wildcard $(KERNEL_CROSS_COMPILE)gcc))
    $(error $(KERNEL_CROSS_COMPILE) is not existed)
endif

LOCAL_KERNEL_MAKE_OPTION := O=$(KERNEL_OUT) ARCH=$(TARGET_ARCH) CROSS_COMPILE=$(KERNEL_CROSS_COMPILE) ROOTDIR=$(KERNEL_DIR) KERNEL_DIR=$(KERNEL_DIR) KERNEL_OUT=$(KERNEL_OUT)
export KERNEL_MAKE_OPTION=$(LOCAL_KERNEL_MAKE_OPTION)

ifeq ($(TARGET_ARCH), arm64)
  ifeq ($(MTK_APPENDED_DTB_SUPPORT), yes)
    export KERNEL_ZIMAGE_OUT=$(KERNEL_OUT)/arch/$(TARGET_ARCH)/boot/Image.gz-dtb
  else
    export KERNEL_ZIMAGE_OUT=$(KERNEL_OUT)/arch/$(TARGET_ARCH)/boot/Image.gz
  endif
else
  ifeq ($(MTK_APPENDED_DTB_SUPPORT), yes)
    export KERNEL_ZIMAGE_OUT=$(KERNEL_OUT)/arch/$(TARGET_ARCH)/boot/zImage-dtb
  else
    export KERNEL_ZIMAGE_OUT=$(KERNEL_OUT)/arch/$(TARGET_ARCH)/boot/zImage
  endif
endif

# set for module Makefile
export AUTO_CONF=$(KERNEL_OUT)/include/config/auto.conf


###############################################################################
# Generally Android.mk can not get KConfig setting
# we can use this way to get
# include the final KConfig
# but there is no $(AUTO_CONF) at the first time (no out folder) when make
#
#ifneq (,$(wildcard $(AUTO_CONF)))
#include $(AUTO_CONF)
#include $(CLEAR_VARS)
#endif

###############################################################################
###############################################################################
# Generally Android.mk can not get KConfig setting                            #
#                                                                             #
# do not have any KConfig checking in Android.mk                              #
# do not have any KConfig checking in Android.mk                              #
# do not have any KConfig checking in Android.mk                              #
#                                                                             #
# e.g. ifeq ($(CONFIG_MTK_COMBO_WIFI), m)                                     #
#          xxxx                                                               #
#      endif                                                                  #
#                                                                             #
# e.g. ifneq ($(filter "MT6632",$(CONFIG_MTK_COMBO_CHIP)),)                   #
#          xxxx                                                               #
#      endif                                                                  #
#                                                                             #
# All the KConfig checking should move to Makefile for each module            #
# All the KConfig checking should move to Makefile for each module            #
# All the KConfig checking should move to Makefile for each module            #
#                                                                             #
###############################################################################
###############################################################################

LOCAL_PATH := $(call my-dir)

ifeq ($(MTK_WLAN_SUPPORT), yes)

ifneq (true,$(strip $(TARGET_NO_KERNEL)))

include $(CLEAR_VARS)
LOCAL_MODULE := wlan_drv_gen4m.ko
LOCAL_STRIP_MODULE := true
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_TAGS := optional
LOCAL_MULTILIB := first
ifeq ($(TARGET_OUT_VENDOR),)
LOCAL_MODULE_PATH := $(ALPS_OUT)/vendor/lib/modules
else
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/lib/modules
endif
LOCAL_SRC_FILES := $(patsubst $(LOCAL_PATH)/%,%,$(shell find $(LOCAL_PATH) -type f -name '*.[cho]')) Makefile
LOCAL_REQUIRED_MODULES := wmt_chrdev_wifi.ko

LOCAL_POST_INSTALL_CMD := $(hide)$(TARGET_STRIP) --strip-unneeded $(LOCAL_MODULE_PATH)/$(LOCAL_MODULE)

include $(BUILD_SYSTEM)/base_rules.mk

WIFI_NAME := wlan_drv_gen4m
WIFI_OPTS := CONFIG_MTK_COMBO_WIFI_HIF=$(WIFI_HIF) MODULE_NAME=$(WIFI_NAME) MTK_COMBO_CHIP=$(WIFI_CHIP) MTK_ANDROID_WMT=$(WIFI_WMT) MTK_ANDROID_EMI=$(WIFI_EMI) WIFI_ENABLE_GCOV=$(WIFI_ENABLE_GCOV) MTK_ANDROID_EMI=y

### Copy Module.symvers from $(LOCAL_REQUIRED_MODULES) to this module #######
### For symbol link (when CONFIG_MODVERSIONS is defined)
LOCAL_SRC_EXPORT_SYMBOL := $(subst $(LOCAL_MODULE),$(LOCAL_REQUIRED_MODULES),$(intermediates))/Module.symvers
$(LOCAL_SRC_EXPORT_SYMBOL): $(subst $(LOCAL_MODULE),$(LOCAL_REQUIRED_MODULES),$(LOCAL_BUILT_MODULE))
LOCAL_EXPORT_SYMBOL := $(intermediates)/Module.symvers
$(LOCAL_EXPORT_SYMBOL): $(intermediates)/% : $(LOCAL_SRC_EXPORT_SYMBOL)
	$(copy-file-to-target)
LOCAL_GENERATED_SOURCES := $(addprefix $(intermediates)/,$(LOCAL_SRC_FILES))
$(LOCAL_GENERATED_SOURCES): $(intermediates)/% : $(LOCAL_PATH)/%
	$(copy-file-to-target)

$(AUTO_CONF): $(KERNEL_ZIMAGE_OUT)
$(AUTOCONF_H): $(KERNEL_ZIMAGE_OUT)
$(KERNEL_OUT)/scripts/sign-file: $(KERNEL_ZIMAGE_OUT);

$(LOCAL_BUILT_MODULE): KOPTS := $(KERNEL_MAKE_OPTION) M=$(abspath $(intermediates))
$(LOCAL_BUILT_MODULE): CERT_PATH := $(LINUX_KERNEL_VERSION)/certs
$(LOCAL_BUILT_MODULE): $(wildcard $(LINUX_KERNEL_VERSION)/certs/ko_prvk.pem)
$(LOCAL_BUILT_MODULE): $(wildcard $(LINUX_KERNEL_VERSION)/certs/ko_pubk.x509.der)
$(LOCAL_BUILT_MODULE): $(wildcard vendor/mediatek/proprietary/scripts/kernel_tool/rm_ko_sig.py)
$(LOCAL_BUILT_MODULE): $(LOCAL_GENERATED_SOURCES) $(KERNEL_OUT)/scripts/sign-file $(LOCAL_EXPORT_SYMBOL) $(LOCAL_SRC_EXPORT_SYMBOL) $(AUTO_CONF) $(AUTOCONF_H)
	@echo $@: $^
	$(MAKE) -C $(KERNEL_OUT) $(KOPTS) $(WIFI_OPTS)
	$(hide) $(call sign-kernel-module,$(KERNEL_OUT)/scripts/sign-file,$(CERT_PATH)/ko_prvk.pem,$(CERT_PATH)/ko_pubk.x509.der)

endif
endif

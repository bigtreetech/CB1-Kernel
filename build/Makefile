# Makefile for kunos
#
# Copyright (C) 2018 <huafenghuang@allwinnertech.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

# we want bash as shell
SHELL := $(shell if [ -x "$$BASH" ]; then echo $$BASH; \
               else if [ -x /bin/bash ]; then echo /bin/bash; \
               else echo sh; fi; fi)

TARGET_DIR := $(LICHEE_BR_OUT)/target
STAGING_DIR := ${LICHEE_BR_OUT}/staging
TARGET_TOP := $(shell pwd)

# Convenient variables
comma   := ,
quote   := "
squote  := '
empty   :=
space   := $(empty) $(empty)

$(warning "----------1--------")
$(warning $(shell pwd))

-include ${LICHEE_PRODUCT_CONFIG_DIR}/${LICHEE_BOARD}/copy_files.mk
###########################################################
## Read the word out of a colon-separated list of words.
## This has the same behavior as the built-in function
## $(word n,str).
##
## The individual words may not contain spaces.
##
## $(1): 1 based index
## $(2): value of the form a:b:c...
###########################################################

define word-colon
$(word $(1),$(subst :,$(space),$(2)))
endef

###########################################################
## Append a leaf to a base path.  Properly deals with
## base paths ending in /.
##
## $(1): base path
## $(2): leaf path
###########################################################

define append-path
$(subst //,/,$(1)/$(2))
endef
# Copy a single file from one place to another,
# preserving permissions and overwriting any existing
# file.
# When we used acp, it could not handle high resolution timestamps
# on file systems like ext4. Because of that, '-t' option was disabled
# and copy-file-to-target was identical to copy-file-to-new-target.
# Keep the behavior until we audit and ensure that switching this back
# won't break anything.
define copy-file-to-target
$(1):$(2) FORCE
	@echo '   COPY FILES  $$@'
	@mkdir -p $$(dir $$@)
	@rm -rf $$@
	@cp -rf $$< $$@
endef

#####################################################
# Define rules to copy PRODUCT_COPY_FILES defined by the product.
# PRODUCT_COPY_FILES contains words like <source file>:<dest file>[:<owner>].
# <dest file> is relative to $(TARGET_DIR), so it should look like,
# e.g."etc/file".
# The filter part means "only eval the copy-one-file rule if this
# src:dest pair is the first one to match the same dest"
#$(1): the src:dest pair
#####################################################
unique_product_copy_files_pairs :=
ALL_DEFAULT_INSTALLED_FILES :=
$(foreach cf,$(PRODUCT_COPY_FILES), \
    $(if $(filter $(unique_product_copy_files_pairs),$(cf)),,\
        $(eval unique_product_copy_files_pairs += $(cf))))
unique_product_copy_files_destinations :=
$(foreach cf,$(unique_product_copy_files_pairs), \
    $(eval _dest := $(call word-colon,2,$(cf))) \
	$(eval _src := $(call word-colon,1,$(cf))) \
    $(if $(filter $(unique_product_copy_files_destinations),$(_dest)), \
        $(info PRODUCT_COPY_FILES $(cf) ignored.), \
	$(eval _fullsrc := $(call append-path,$(LICHEE_TOP_DIR),$(_src))) \
	$(eval _fulldest := $(call append-path,$(TARGET_DIR),$(_dest))) \
	$(eval $(call copy-file-to-target,$(_fulldest),$(_fullsrc))) \
	$(eval ALL_DEFAULT_INSTALLED_FILES += $(_fulldest))))
unique_product_copy_files_pairs :=
unique_product_copy_files_destinations :=

.PHONY:INSTALL_FILES FORCE
INSTALL_FILES:$(ALL_DEFAULT_INSTALLED_FILES)

.PHONY:clean FORCE
clean:
	@rm -rf $(ALL_DEFAULT_INSTALLED_FILES)

PHONY += FORCE
FORCE:

# Declare the contents of the .PHONY variable as phony.  We keep that
# information in a variable so we can use it in if_changed and friends.
.PHONY: $(PHONY)


# define sources root directory before everything
SRC_ROOT := ../..

# include rule.mk
include $(SRC_ROOT)/rule.mk

# add you test case here
.PHONY: all
all:
	cp wifitester.sh $(BINDIR)/
	cp -rax firmware/* $(FIRMWARE)/

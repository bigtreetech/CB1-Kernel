include $(TOPDIR)/mk/checkconf.mk

conf-mk-file := $(SRCTREE)/autoconf.mk
$(conf-mk-file):
	$(call check-conf-mk)
conf-file := $(SRCTREE)/include/config.h
$(conf-file):
	$(call check-conf-h)

.PHONY:$(conf-file) $(conf-mk-file)
build-confs: $(conf-file) $(conf-mk-file)

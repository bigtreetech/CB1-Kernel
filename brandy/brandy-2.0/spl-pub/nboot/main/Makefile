
include $(TOPDIR)/mk/config.mk

LIB	:= $(obj)libmain.o

MAIN   += boot0_main.o
HEAD	+= boot0_head.o

SRCS	:= $(MAIN:.o=.c) $(COBJS:.o=.c) $(HEAD:.o=.c)
OBJS	:= $(addprefix $(obj),$(COBJS) $(COBJS-y) $(SOBJS))
HEAD    := $(addprefix $(obj),$(HEAD))
MAIN	:= $(addprefix $(obj),$(MAIN))

all:	$(obj).depend $(HEAD) $(MAIN) $(LIB) $(DATA)

$(LIB):	$(OBJS) $(MAIN) $(HEAD)
	$(call cmd_link_o_target, $(OBJS))

#########################################################################

# defines $(obj).depend target
include $(TOPDIR)/mk/rules.mk

sinclude $(obj).depend

#########################################################################

#
# (C) Copyright 2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

#########################################################################

_depend: .depend

.depend: $(TOPDIR)/mk/config.mk $(SRCS)
		@rm -f $@
		@touch $@
		@for f in $(SRCS); do \
			g=`basename $$f | sed -e 's/\(.*\)\.[[:alnum:]_]/\1.o/'`; \
			$(CC) -M $(ALL_CFLAGS) -MQ $(obj)$$g $$f >> $@ ; \
		done

#autoconf.mk generated in fes(sboot\nboot)/makefile
#rules.mk used in both fes(sboot\nboot)/makefile and $(subdir)/makefile
#to avoid loop dependency in fes(sboot\nboot)/makefile, only add this
#build rule in $(subdir)/makefile
ifneq ($(SKIP_AUTO_CONF),yes)
.depend: $(TOPDIR)/autoconf.mk
endif

sinclude .depend

#########################################################################

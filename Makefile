all: ceu contiki

APPS=servreg-hack
CONTIKI=/opt/contiki

WITH_UIP6=1
UIP_CONF_IPV6=1
CFLAGS+= -DUIP_CONF_IPV6_RPL

ceu:
	ceu $(CEUFILE) --m4 --defs-file _ceu_defs.h
	touch contiki.c

include $(CONTIKI)/Makefile.include

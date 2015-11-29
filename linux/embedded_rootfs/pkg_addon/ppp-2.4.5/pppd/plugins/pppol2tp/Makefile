#CC	= gcc
COPTS	= -O2 
CFLAGS	= $(COPTS) -I. -I../.. -I../../../include -I${OCTEON_ROOT}/linux/embedded_rootfs/../kernel_2.6/linux/include
LDFLAGS	= -shared
INSTALL	= install

#***********************************************************************

DESTDIR = ${ROOT}
LIBDIR = $(DESTDIR)/lib/pppd/$(VERSION)

VERSION = $(shell awk -F '"' '/VERSION/ { print $$2; }' ../../patchlevel.h)

PLUGINS := pppol2tp.so openl2tp.so

all: $(PLUGINS)

%.so: %.o
	$(CC) $(CFLAGS) -o $@ -shared $^ $(LIBS)
#	mkdir -p ${ROOT}/etc/ppp/plugins
#	cp -f ./openl2tp.so ${ROOT}/etc/ppp/plugins/openl2tp.so
#	cp -f ./pppol2tp.so ${ROOT}/etc/ppp/plugins/pppol2tp.so

install: all
	$(INSTALL) -d -m 755 $(LIBDIR)
	$(INSTALL) -c -m 4550 $(PLUGINS) $(LIBDIR)

clean:
	rm -f *.o *.so

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

 
top_srcdir = ../strongswan-4.0.4
INCLUDES = -I$(top_srcdir)/src/libfreeswan -I$(top_srcdir)/src/pluto

LIBS = -lnku
NKIPSEC_OBJ = nk_ipsec.o nk_whack.o ../webconfig/webconfig_util.o

NKUSBKEY_OBJ = nk_usbkey.o ../webconfig/webconfig_util.o

CFLAGS +=  -lpthread -Wall -Wpointer-arith -Wcast-align -Wcast-qual -Wshadow \
 -Waggregate-return -Wnested-externs -Winline -Wwrite-strings \
 -Wstrict-prototypes 

CFLAGS += -O

all:

_updown: _updown.o
	$(CC) $(CFLAGS) -o _updown _updown.o $(LIBS)

nk_ipsecd: $(NKIPSEC_OBJ)
	$(CC) $(CFLAGS) -o nk_ipsecd $(NKIPSEC_OBJ) $(LIBS)

nk_whack.o:nk_whack.c
	$(CC) $(CFLAGS) $(INCLUDES) -c nk_whack.c $(LIBS)


install:
	$(CP) nk_ipsecd $(DESTUSR)/sbin/nk_ipsecd
	$(CP) _updown $(DESTUSR)/sbin/_updown

clean:
	rm -f nk_ipsecd _updown *.o


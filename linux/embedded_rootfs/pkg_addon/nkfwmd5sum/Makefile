
#LIBS = -lnku
CFLAGS += -Os -I../../build/openssl-1.0.0a/include/

nkfwmd5sum:nkfwmd5sum.o
	$(CC) $(CFLAGS) -o $@ nkfwmd5sum.o $(LIBS) $(SSL_LIBS)

clean:
	rm -f *.o nkfwmd5sum

install:
	$(CP) nkfwmd5sum $(ROOT)/sbin/nkfwmd5sum

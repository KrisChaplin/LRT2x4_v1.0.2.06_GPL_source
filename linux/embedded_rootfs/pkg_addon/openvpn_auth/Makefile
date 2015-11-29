

LIBS = -lnku
NKI_OBJ = auth.o ../webconfig/webconfig_util.o

all: auth 

auth: $(NKI_OBJ)
	$(CC) $(CFLAGS) -o auth $(NKI_OBJ) $(LIBS)


install:
	$(CP) auth $(DESTUSR)/sbin/auth

clean:
	rm -f auth *.o




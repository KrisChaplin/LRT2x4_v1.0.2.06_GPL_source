BIN=main_bin

all: $(BIN)

include ../make-include.mk

CFLAGS+=-I$(SYS_LIB) -I$(AUTH_LIB)

main_bin:main_bin.c ../smm/libSmm.a $(SYS_LIB)/libSys.a  ../firebase/libFirebase.a ../ftp/libFtpsession.a
	${CC} -o $@ ${LDFLAGS} main_bin.c ../smm/libSmm.a ../firebase/libFirebase.a ../ftp/libFtpsession.a ../lib/libSys.a -lutil -lcrypt $(SSL_LIBS)

cleanbin:
	rm $(BIN)
	if [ -n '$(LIB)' ]; then rm -rf $(LIB); fi

clean:
	rm -f *.o
	rm -f $(BIN) smm firebase ftpsession
	if [ -n '$(LIB)' ]; then rm -rf $(LIB); fi

install:
	if [ -n '$(BIN)' ]; then $(STRIP) $(BIN); $(INSTALL) $(BIN) $(ACCESS_POINT_BIN); fi
	cd $(ACCESS_POINT_BIN); ln -sf main_bin smm
	cd $(ACCESS_POINT_BIN); ln -sf main_bin firebase
	cd $(ACCESS_POINT_BIN); ln -sf main_bin ftpsession

un-install:
	if [ -n '$(BIN)' ]; then cd $(ACCESS_POINT_BIN); rm -rf smm firebase ftpsession main_bin; fi

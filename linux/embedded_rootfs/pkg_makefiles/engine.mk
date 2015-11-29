
include ../.config

ifdef TOOLCHAIN_ABI_N32
   ABI=linux_n32
	INSTALLDIR=${ROOT}/usr/lib32
endif
ifdef TOOLCHAIN_ABI_64
   ABI=linux_64
	INSTALLDIR=${ROOT}/usr/lib64
endif
ENGINESDIR=${ROOT}/usr/lib/engines

PKG:=linux_engine_2.0
DIR:=${OCTEON_ROOT}/applications/${PKG}
OPENSSL_VERSION:=1.0.0a

.PHONY: all
all: install

.PHONY: build
build: 
	${MAKE} -C ${DIR} OCTEON_TARGET=${ABI}

.PHONY: install
install: ${DIR}
	cp ${DIR}/libocteon.so ${INSTALLDIR}
	mkdir -p ${ENGINESDIR}
	cp ${DIR}/libocteon.so ${ENGINESDIR}

	


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

PKG:=linux_engine
DIR:=${OCTEON_ROOT}/applications/${PKG}
OPENSSL_VERSION:=1.0.0a

.PHONY: all
all: build install

.PHONY: build
build: 
	${MAKE} -C ${DIR} OCTEON_TARGET=${ABI}

.PHONY: install
install: ${DIR}
	cp ${DIR}/libocteon.so ${INSTALLDIR}
	mkdir -p ${ENGINESDIR}
	cp ${DIR}/libocteon.so ${ENGINESDIR}
	cp ${OCTEON_ROOT}/linux/embedded_rootfs/build/openssl-${OPENSSL_VERSION}/apps/server.pem ${ENGINESDIR}
	cp ${OCTEON_ROOT}/applications/linux_engine/sample/sslserver ${ROOT}/usr/bin
	cp ${OCTEON_ROOT}/applications/linux_engine/sample/sercert1024.pem ${ENGINESDIR}
	cp ${OCTEON_ROOT}/applications/linux_engine/sample/server1024 ${ENGINESDIR}

	

BUILD=/netklass/GPL/LRT2x4/v1.0.2.06/Publication_Tarball/LRT2x4_v1.0.2.06_GPL_tarball_20140321/LRT2x4_v1.0.2.06_GPL_tarball/LRT2x4_v1.0.2.06_GPL/LRT2x4_v1.0.2.06_GPL_source/linux/embedded_rootfs/pkg_addon
INSTALL=cp
CONFIG_NK_CRAMFS=y
CONFIG_RV0XX=y
CONFIG_NK_NO_SSL_FUNCTION=y
CONFIG_COMMON_UI=y
CONFIG_IPV6=y
.PHONY=all clean cleanbin install un-install

SSL_DIR=$(BUILD)/../build/openssl-1.0.0a

KRB_DIR=$(BUILD)/../build/krb5-1.3.1
KRB_INC_DIR=$(KRB_DIR)/src/include
KRB_LIB_DIR=$(KRB_DIR)/src/lib

SAMBA_DIR=$(BUILD)/../build/samba-3.0.22
SAMBA_INC_DIR=$(SAMBA_DIR)/source/include
SAMBA_LIB_DIR=$(SAMBA_DIR)/source/bin

LDAP_DIR=$(BUILD)/../build/openldap-2.3.24
LDAP_INC_DIR=$(LDAP_DIR)/include
LDAP_LIB_DIR=$(LDAP_DIR)/libraries/libldap/.libs
LDAP_BER_LIB_DIR=$(LDAP_DIR)/libraries/liblber/.libs

TOP_DIR=$(BUILD)/EasyAccess
INSTALL_TOP_DIR=$(ROOT)/usr/local/EasyAccess

LDFLAGS = "-mabi=64"

CFLAGS+= -Wall -Os\
        -I$(SSL_DIR)/include -DSINGLE_BINARY \
        -DMAX_MSG_LEN=32768 \
        -DPRODUCT_ID=\"EasyAccess\" -DVENDOR_ID="\"Cavium Networks\"" \
        -DPRODUCT_STRING="\"Cavium Networks, EasyAccess SSL VPN\""\
        -DKILLCOMMAND="\"/usr/bin/killall\"" \
        -DNTPDATE="\"/bin/ntpdate\"" \
        -DNTPD="\"/bin/ntpd\"" -DTARGET_PC_Octeon

CFLAGS+= -DCONFIG_NK_SYNC_WITH_RVL200
CFLAGS+= -DCONFIG_NK_SYNC_WITH_RVL200_RESET_TIMER
CFLAGS+= -DCONFIG_NK_AUTO_SEARCH_LOGIN_DOMAIN
CFLAGS+= -DCONFIG_NK_CHECK_USER_EXPIRY_DATE
CFLAGS+= -DCONFIG_NK_CHECK_USER_COUNT_LIMITATION
CFLAGS+= -DCONFIG_NK_CHECK_USER_LOGIN_GROUP
#CFLAGS+= -DCONFIG_NK_RESTRICT_CRACK_CALCULATOR
#CFLAGS+= -DCONFIG_NK_IMAGE_VALIDATION_CODE

ifdef CONFIG_IPV6
CFLAGS+= -DCONFIG_IPV6
endif

STRIP=${CROSS}-strip -d -x -R .note -R .comment

SYS_LIB=$(TOP_DIR)/src/lib
AUTH_LIB=$(TOP_DIR)/src/libauth
GCGI_LIB=$(TOP_DIR)/src/libgcgi
SSLCERT_LIB=$(TOP_DIR)/src/libsslcert

WWW_ROOT=$(INSTALL_TOP_DIR)/www
ACCESS_POINT_BIN=$(INSTALL_TOP_DIR)/bin

DEST_CERT_BIN=$(WWW_ROOT)/cert-bin
DEST_CGI_BIN=$(WWW_ROOT)/cgi-bin
DEST_HTDOCS=$(WWW_ROOT)/htdocs

SSL_LIBS=${ROOT}/usr/lib64/libssl.so ${ROOT}/usr/lib64/libcrypto.so


ZLIB_DIR=$(BUILD)/../build/zlib-1.2.3
ZLIB_INC_DIR=$(ZLIB_DIR)
ZLIB_LIB_DIR=$(ZLIB_DIR)

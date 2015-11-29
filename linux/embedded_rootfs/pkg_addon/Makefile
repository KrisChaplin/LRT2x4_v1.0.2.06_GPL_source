include nkuserland.config

BUILD_DIR-y += nku
CLEAN_DIR += nku

BUILD_DIR-y += tool
CLEAN_DIR += tool

BUILD_DIR-${CONFIG_SQLITE} += sqlite-2
CLEAN_DIR += sqlite-2

BUILD_DIR-y += webconfig
CLEAN_DIR += webconfig

ifeq "$(NK_CONFIG_EASYACCESS)" "y"
CLEAN_DIR += EasyAccess
endif

BUILD_DIR-y += mailsend1.16
CLEAN_DIR += mailsend1.16

# smtpclient replaced by msmtp
#BUILD_DIR-y += smtpclient-1.0.0
#CLEAN_DIR += smtpclient-1.0.0
BUILD_DIR-${CONFIG_PPP2_4_3} += ppp-2.4.5
CLEAN_DIR += ppp-2.4.5

BUILD_DIR-${CONFIG_PPTP_CLIENT} += pptp-1.7.0
CLEAN_DIR += pptp-1.7.0

BUILD_DIR-${CONFIG_IPROUTE2} += iproute2
CLEAN_DIR += iproute2

BUILD_DIR-${CONFIG_NK_NSD} += nsd
CLEAN_DIR += nsd

BUILD_DIR-${NK_CONFIG_DHCPCD} += dhcpcd
CLEAN_DIR += dhcpcd

BUILD_DIR-${NK_CONFIG_UDHCP} += udhcp
CLEAN_DIR += udhcp

BUILD_DIR-${NK_CONFIG_DNSMASQ} += dnsmasq-2.40
CLEAN_DIR += dnsmasq-2.40

BUILD_DIR-${NK_CONFIG_DDNS} += ez-ipupdate
CLEAN_DIR += ez-ipupdate

BUILD_DIR-${CONFIG_NK_DDNS_QDNS} += ddns_update
CLEAN_DIR += ddns_update

BUILD_DIR-${NK_CONFIG_SYSKLOGD} += sysklogd-1.5_ca2
CLEAN_DIR += sysklogd-1.5_ca2

#Added by Gavin Lin, 2008/08/15
BUILD_DIR-${NK_CONFIG_SNMP} += net-snmp-5.4.1.2
CLEAN_DIR += net-snmp-5.4.1.2

BUILD_DIR-${CONFIG_NK_SUPPORT_USB_3G} += nk_3g_util
CLEAN_DIR += nk_3g_util


BUILD_DIR-y += nkfwmd5sum
CLEAN_DIR += nkfwmd5sum


BUILD_DIR-${NK_CONFIG_SNMP} += bind9

BUILD_DIR-${CONFIG_NK_SUPPORT_USB_3G} += libusb-0.1.12 
CLEAN_DIR += libusb-0.1.12 
BUILD_DIR-${CONFIG_NK_SUPPORT_USB_3G} += usb_modeswitch-1.0.5
CLEAN_DIR += usb_modeswitch-1.0.5

ifeq "$(NK_CONFIG_SNMP)" "y"
BUILD_DIR-${NK_CONFIG_SNMP} += sysstat-8.1.5
CLEAN_DIR += sysstat-8.1.5
else
BUILD_DIR-${CONFIG_COMMON_UI} += sysstat-8.1.5
CLEAN_DIR += sysstat-8.1.5
endif

BUILD_DIR-${NK_CONFIG_SNMP} += sysstat-8.1.5
CLEAN_DIR += sysstat-8.1.5

BUILD_DIR-${CONFIG_NK_BONJOUR} += mDNSResponder-107.6/mDNSPosix
CLEAN_DIR += mDNSResponder-107.6/mDNSPosix

BUILD_DIR-${NK_CONFIG_TFTP} += tftp
CLEAN_DIR += tftp

BUILD_DIR-${CONFIG_NK_RIP} += zebra
CLEAN_DIR += zebra

BUILD_DIR-${NK_CONFIG_PPTPD} += pptpd-1.3.4
CLEAN_DIR += pptpd-1.3.4

BUILD_DIR-${NK_CONFIG_THTTPD} += web_mng/thttpd
CLEAN_DIR += web_mng/thttpd

ifeq "$(CONFIG_COMMON_UI)" "y"
BUILD_DIR-${NK_CONFIG_THTTPD_CGI} += web_mng/web_ui_Cgi
CLEAN_DIR += web_mng/web_ui_Cgi
else
BUILD_DIR-${NK_CONFIG_THTTPD_CGI} += web_mng/cgi
CLEAN_DIR += web_mng/cgi
endif

BUILD_DIR-y += iptables-1.3.7
CLEAN_DIR += iptables-1.3.7

BUILD_DIR-y += netlinkctl
CLEAN_DIR += netlinkctl

BUILD_DIR-${CONFIG_NK_SUPPORT_USB_3G} += usblinkctl
CLEAN_DIR += usblinkctl

ifeq "$(NK_CONFIG_CLAMAV)" "y"
BUILD_DIR-y += bzip2-1.0.5
CLEAN_DIR += bzip2-1.0.5
endif

CLEAN_DIR += clamav-0.93
CFLAGS += -DOCTEON_MODEL=OCTEON_CN31XX

BUILD_DIR-${NK_CONFIG_NTPCLIENT} += ntpclient
CLEAN_DIR += ntpclient

BUILD_DIR-${CONFIG_NK_STRONGSWAN} += strongswan-4.0.4
CLEAN_DIR += strongswan-4.0.4

ifeq "$(CONFIG_LRTXXX)" "y"
BUILD_DIR-${CONFIG_NK_IPSEC} += ipsec-1.3
CLEAN_DIR += ipsec-1.3
else
BUILD_DIR-${CONFIG_NK_IPSEC} += ipsec
CLEAN_DIR += ipsec
endif

BUILD_DIR-${CONFIG_NK_L2TP_CLIENT} += xl2tpd-1.3.0
CLEAN_DIR += xl2tpd-1.3.0


BUILD_DIR-${CONFIG_NK_HA} += ha
CLEAN_DIR += ha

BUILD_DIR-${CONFIG_NK_IDS} += snort
CLEAN_DIR += snort

BUILD_DIR-y += lan_up_daemon
CLEAN_DIR += lan_up_daemon

BUILD_DIR-y += detect_wan_session
CLEAN_DIR += detect_wan_session


BUILD_DIR-y += certificate_management
CLEAN_DIR += certificate_management

BUILD_DIR-${CONFIG_NK_PPPSMALL} += pppd_small
CLEAN_DIR += pppd_small

BUILD_DIR-${CONFIG_NK_DHCP_RELAY} += dhcp-helper-1.0
CLEAN_DIR += dhcp-helper-1.0

BUILD_DIR-y += check_ui_lock
CLEAN_DIR += check_ui_lock

BUILD_DIR-y += switch_test
CLEAN_DIR += switch_test

BUILD_DIR-y += libupnp
CLEAN_DIR += libupnp

BUILD_DIR-y += libupnp/upnp/sample/linuxigd
CLEAN_DIR += libupnp/upnp/sample/linuxigd

SCRT_BUILD_DIR-${CONFIG_NK_LIBGMP} += gmp-4.2.1
CLEAN_DIR += gmp-4.2.1

#BUILD_DIR-y += msmtp-1.4.21
#CLEAN_DIR += msmtp-1.4.21

BUILD_DIR-${NK_CONFIG_IPV6} += dhcp-4.1.1-P1
CLEAN_DIR += dhcp-4.1.1-P1

SCRT_BUILD_DIR-${NK_CONFIG_IPV6} += radvd-1.6
CLEAN_DIR += radvd-1.6

BUILD_DIR-${NK_CONFIG_IPV6} += naptd-0.4.2
CLEAN_DIR += naptd-0.4.2



#add for anti SPAM-->
BUILD_DIR-${CONFIG_ANTI_SPAM} += db-4.6.18/build_unix
CLEAN_DIR += db-4.6.18/build_unix

BUILD_DIR-${CONFIG_ANTI_SPAM} += gsl-1.11
CLEAN_DIR += gsl-1.11

BUILD_DIR-${CONFIG_ANTI_SPAM} += pcre-7.7
CLEAN_DIR += pcre-7.7

BUILD_DIR-${CONFIG_ANTI_SPAM} += bogofilter-1.1.7
CLEAN_DIR += bogofilter-1.1.7

BUILD_DIR-${CONFIG_ANTI_SPAM} += fetchmail-6.3.13
CLEAN_DIR += fetchmail-6.3.13

# BUILD_DIR-${NK_CONFIG_IPV6} += slaac
# CLEAN_DIR += slaac
#<--

BUILD_DIR-${CONFIG_NK_OPENVPN_SERVER} += openvpn-2.2.1
CLEAN_DIR += openvpn-2.2.1

BUILD_DIR-${CONFIG_NK_OPENVPN_SERVER} += openvpn_auth
CLEAN_DIR += openvpn_auth

ifeq "$(CONFIG_NK_URL_TMUFE_FILTER)" "y"
BUILD_DIR-y += webconfig/stunnel
CLEAN_DIR += webconfig/stunnel
else 
ifeq "$(CONFIG_NK_EZLINKVPN)" "y"
BUILD_DIR-y += webconfig/stunnel
CLEAN_DIR += webconfig/stunnel
endif
endif

ifeq "$(CONFIG_IPV6_MROUTE)" "y"
BUILD_DIR-y += pim6dd
CLEAN_DIR += pim6dd
endif

MAN_USER = root
MAN_GROUP = root


ifeq "$(CONFIG_COMMON_UI)" "y"
ifeq "$(NK_CONFIG_EASYACCESS)" "y"
else
HTDOCS_DIR-${CONFIG_COMMON_UI} += vendor/web_ui_Base
endif

else


endif

.PHONY: all
all:
ifeq "$(CONFIG_COMMON_UI)" "y"
	rm -rf vendor/web_ui_Base/root/sbin/*


#copy the config file of the dhcp client ipv6 to /etc
ifeq "$(NK_CONFIG_IPV6)" "y"
	cp -r dhcp-4.1.1-P1/client/scripts/linux ${ROOT}/etc/dhclient-script
	chmod 755 ${ROOT}/etc/dhclient-script
	touch ${ROOT}/etc/dhclient.conf
	chmod 755 ${ROOT}/etc/dhclient.conf
endif

		cp -r vendor/web_ui_Data/Model_sbin/RV0XX_CRAMFS/. vendor/web_ui_Base/root/sbin

endif
	for s in ${SCRT_BUILD_DIR-y}; do \
		cd $$s;\
		./RUN; \
		cd -; \
	done

	for i in ${BUILD_DIR-y}; do \
		if ! ${MAKE} -C $$i ; then exit 1; fi; \
		if ! ${MAKE} -C $$i install; \
		then \
			echo "@@@@Error found! ...when doing [${MAKE} -C $$i install]"; \
		fi; \
	done	

	for u in ${HTDOCS_DIR-y}; do \
		cp -r $$u/root/. ${ROOT}/; \
	done



ifeq "$(NK_CONFIG_CLAMAV)" "y"
		cd ../build/zlib-1.2.3 && ${MAKE} install
		
		cd clamav-0.93 && ./configure --sysconfdir=/etc --host=${CROSS} CFLAGS="${CFLAGS}" LDFLAGS=${TOOLCHAIN_ABI} --with-user=${MAN_USER} --with-group=${MAN_GROUP} --with-zlib=${ROOT}/usr --bindir=${ROOT}/bin --sbindir=${ROOT}/sbin --datarootdir=${ROOT}/share/clamav --with-dbdir=/share/clamav --libdir=/usr/local/lib/clamav --with-libbz2-prefix=${ROOT}/usr/local/bzip2 --nk_libdir=${ROOT}/usr/local/lib/clamav 
		
		if ! ${MAKE} -C clamav-0.93 ; then exit 1; fi;
		if ! ${MAKE} -C clamav-0.93 install ; then exit 1; fi;
endif

ifeq "$(NK_CONFIG_EASYACCESS)" "y"
	cd EasyAccess && CONFIG_NK_NO_SSL_FUNCTION="$(CONFIG_NK_NO_SSL_FUNCTION)" CONFIG_RV0XX="$(CONFIG_RV0XX)" CONFIG_NK_CRAMFS="$(CONFIG_NK_CRAMFS)" CONFIG_COMMON_UI="$(CONFIG_COMMON_UI)" CONFIG_EPS="n" CONFIG_NK_LICENSE_COUNT_DOWN="$(CONFIG_NK_LICENSE_COUNT_DOWN)" CONFIG_NK_HTTPS_CONTROL="$(CONFIG_NK_HTTPS_CONTROL)" CONFIG_IPV6="$(NK_CONFIG_IPV6)" STATISTICS="n" ./RUN.Octeon
	${MAKE} -C EasyAccess PREFIX=${ROOT}/usr/local/EasyAccess -j4
	${MAKE} -C EasyAccess PREFIX=${ROOT}/usr/local/EasyAccess  install
endif

	#cp -r vendor/Nk7100/. ${ROOT}/usr/local/EasyAccess/www/htdocs


	#cp -r vendor/web_ui_Base/root/html/. ${ROOT}/usr/local/EasyAccess/www/htdocs
	#cp -r vendor/web_ui_SSL/. ${ROOT}/usr/local/EasyAccess/www/htdocs
	#cp -rf vendor/web_ui_Base/root/etc/. ${ROOT}/etc
	cp -r vendor/web_ui_CI005/root/base_html/. ${ROOT}/usr/local/EasyAccess/www/htdocs
	cp -r vendor/web_ui_CI005/root/login.html ${ROOT}/usr/local/EasyAccess/www/cgi-bin/
	cp -rf vendor/web_ui_CI005/root/etc/. ${ROOT}/etc
	cp -rf vendor/web_ui_Base/root/etc/l7-protocols/. ${ROOT}/etc/l7-protocols
	cp -rf vendor/web_ui_Base/root/sbin/. ${ROOT}/sbin
	rm -rf ${ROOT}/usr/local/EasyAccess/www/htdocs/help


ifeq "$(CONFIG_UTM_UI)" "y"
ifeq "$(CONFIG_COMMON_UI)" "y"
	cp -r vendor/web_ui_UTM/. ${ROOT}/usr/local/EasyAccess/www/htdocs
endif
endif


	rm -rf ${ROOT}/usr/local/EasyAccess/www/htdocs/016
	rm -rf ${ROOT}/usr/local/EasyAccess/www/htdocs/082	
	rm -rf ${ROOT}/usr/local/EasyAccess/www/htdocs/042
	rm -rf ${ROOT}/usr/local/EasyAccess/www/htdocs/042G
	rm -rf ${ROOT}/usr/local/EasyAccess/www/htdocs/rv0xx_help


	# remove .svn direction in root fs
	find ${ROOT} -name '.svn' | xargs rm -rf \;

#add for anti SPAM-->
ifeq "$(CONFIG_ANTI_SPAM)" "y"
	cd p3scan-3.0_rc1 && ./install.sh
endif
#<--



.PHONY: clean
clean:
	for j in ${CLEAN_DIR}; do \
		${MAKE} -C $$j clean; \
	done

ifeq "$(NK_CONFIG_EASYACCESS)" "y"
	cd EasyAccess ./RUN.CLEAN
	rm -rf /tmp/nklib/
endif

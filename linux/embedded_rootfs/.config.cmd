deps_config := \
	pkg_kconfig/pci-base.kconfig \
	pkg_kconfig/components-common.kconfig \
	pkg_kconfig/99-final-cleanup.kconfig \
	pkg_kconfig/77-atheros-wireless.kconfig \
	pkg_kconfig/76-easyaccess.kconfig \
	pkg_kconfig/75-broadcom-wireless.kconfig \
	pkg_kconfig/74-mtd-tools.kconfig \
	pkg_kconfig/72-octeon-utils.kconfig \
	pkg_kconfig/67-logrotate.kconfig \
	pkg_kconfig/66-syslog.kconfig \
	pkg_kconfig/65-pppd.kconfig \
	pkg_kconfig/64-smtpclient.kconfig \
	pkg_kconfig/59-samba.kconfig \
	pkg_kconfig/57-racoon2.kconfig \
	pkg_kconfig/57-ntp.kconfig \
	pkg_kconfig/55-tcpdump.kconfig \
	pkg_kconfig/54-net-tools.kconfig \
	pkg_kconfig/52-iptables.kconfig \
	pkg_kconfig/51-ipsec-tools.kconfig \
	pkg_kconfig/41-engine.kconfig \
	pkg_kconfig/40-octcrypto.kconfig \
	pkg_kconfig/30-openldap.kconfig \
	pkg_kconfig/29-kerberos.kconfig \
	pkg_kconfig/26-zlib.kconfig \
	pkg_kconfig/25-openssl.kconfig \
	pkg_kconfig/23-flex.kconfig \
	pkg_kconfig/22-libraries-64.kconfig \
	pkg_kconfig/21-libraries-n32.kconfig \
	pkg_kconfig/20-libpcap.kconfig \
	pkg_kconfig/11-n_hdlc.kconfig \
	pkg_kconfig/11-entropic-clink.kconfig \
	pkg_kconfig/10-cavium-ethernet.kconfig \
	pkg_kconfig/03-init-scripts.kconfig \
	pkg_kconfig/02-busybox.kconfig \
	pkg_kconfig/01-device-files.kconfig \
	.kconfig_include \
	embedded_rootfs.kconfig

.config include/config.h: $(deps_config)

$(deps_config):

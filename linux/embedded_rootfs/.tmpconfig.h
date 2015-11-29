#ifndef BB_CONFIG_H
#define BB_CONFIG_H
/*
 * Automatically generated header file: don't edit
 */

/* Version Number */
#define BB_VER "(null)"
#define BB_BT "(null)"


/*
 * Global Options
 */
#undef TOOLCHAIN_ABI_N32
#define ENABLE_ABI_N32 0
#define USE_ABI_N32(...)
#define SKIP_ABI_N32(...)  __VA_ARGS__

#define TOOLCHAIN_ABI_64 1
#define ENABLE_ABI_64 1
#define USE_ABI_64(...)  __VA_ARGS__
#define SKIP_ABI_64(...)

#undef TOOLCHAIN_UCLIBC
#define ENABLE_UCLIBC 0
#define USE_UCLIBC(...)
#define SKIP_UCLIBC(...)  __VA_ARGS__

#undef CONFIG_kernel-config
#define ENABLE_kernel-config 0
#define USE_kernel-config(...)
#define SKIP_kernel-config(...)  __VA_ARGS__

#define CFG_KERNEL_CONFIG_FILE ""
#define ENABLE_KERNEL_CONFIG_FILE 1
#define USE_KERNEL_CONFIG_FILE(...)  __VA_ARGS__
#define SKIP_KERNEL_CONFIG_FILE(...)

#define CONFIG_kernel-modules 1
#define ENABLE_kernel-modules 1
#define USE_kernel-modules(...)  __VA_ARGS__
#define SKIP_kernel-modules(...)

#define CFG_ENABLE_IPV6 1
#define ENABLE_ENABLE_IPV6 1
#define USE_ENABLE_IPV6(...)  __VA_ARGS__
#define SKIP_ENABLE_IPV6(...)

#define CONFIG_device-files 1
#define ENABLE_device-files 1
#define USE_device-files(...)  __VA_ARGS__
#define SKIP_device-files(...)

#define CONFIG_busybox 1
#define ENABLE_busybox 1
#define USE_busybox(...)  __VA_ARGS__
#define SKIP_busybox(...)

#undef CFG_BUSYBOX_TESTSUITE
#define ENABLE_BUSYBOX_TESTSUITE 0
#define USE_BUSYBOX_TESTSUITE(...)
#define SKIP_BUSYBOX_TESTSUITE(...)  __VA_ARGS__

#define CONFIG_init-scripts 1
#define ENABLE_init-scripts 1
#define USE_init-scripts(...)  __VA_ARGS__
#define SKIP_init-scripts(...)

#define CONFIG_cavium-ethernet 1
#define ENABLE_cavium-ethernet 1
#define USE_cavium-ethernet(...)  __VA_ARGS__
#define SKIP_cavium-ethernet(...)

#undef CONFIG_entropic-clink
#define ENABLE_entropic-clink 0
#define USE_entropic-clink(...)
#define SKIP_entropic-clink(...)  __VA_ARGS__

#undef CONFIG_n_hdlc
#define ENABLE_n_hdlc 0
#define USE_n_hdlc(...)
#define SKIP_n_hdlc(...)  __VA_ARGS__

#undef CONFIG_libpcap
#define ENABLE_libpcap 0
#define USE_libpcap(...)
#define SKIP_libpcap(...)  __VA_ARGS__

#undef CONFIG_libraries-n32
#define ENABLE_libraries-n32 0
#define USE_libraries-n32(...)
#define SKIP_libraries-n32(...)  __VA_ARGS__

#define CONFIG_libraries-64 1
#define ENABLE_libraries-64 1
#define USE_libraries-64(...)  __VA_ARGS__
#define SKIP_libraries-64(...)

#define CONFIG_flex 1
#define ENABLE_flex 1
#define USE_flex(...)  __VA_ARGS__
#define SKIP_flex(...)

#define CONFIG_openssl 1
#define ENABLE_openssl 1
#define USE_openssl(...)  __VA_ARGS__
#define SKIP_openssl(...)

#define CONFIG_zlib 1
#define ENABLE_zlib 1
#define USE_zlib(...)  __VA_ARGS__
#define SKIP_zlib(...)

#define CONFIG_kerberos 1
#define ENABLE_kerberos 1
#define USE_kerberos(...)  __VA_ARGS__
#define SKIP_kerberos(...)

#define CONFIG_openldap 1
#define ENABLE_openldap 1
#define USE_openldap(...)  __VA_ARGS__
#define SKIP_openldap(...)

#define CONFIG_octcrypto 1
#define ENABLE_octcrypto 1
#define USE_octcrypto(...)  __VA_ARGS__
#define SKIP_octcrypto(...)

#define CONFIG_engine 1
#define ENABLE_engine 1
#define USE_engine(...)  __VA_ARGS__
#define SKIP_engine(...)

#define CONFIG_ipsec-tools 1
#define ENABLE_ipsec-tools 1
#define USE_ipsec-tools(...)  __VA_ARGS__
#define SKIP_ipsec-tools(...)

#undef CONFIG_iptables
#define ENABLE_iptables 0
#define USE_iptables(...)
#define SKIP_iptables(...)  __VA_ARGS__

#define CONFIG_net-tools 1
#define ENABLE_net-tools 1
#define USE_net-tools(...)  __VA_ARGS__
#define SKIP_net-tools(...)

#undef CONFIG_tcpdump
#define ENABLE_tcpdump 0
#define USE_tcpdump(...)
#define SKIP_tcpdump(...)  __VA_ARGS__

#undef CONFIG_ntp
#define ENABLE_ntp 0
#define USE_ntp(...)
#define SKIP_ntp(...)  __VA_ARGS__

#undef CONFIG_racoon2
#define ENABLE_racoon2 0
#define USE_racoon2(...)
#define SKIP_racoon2(...)  __VA_ARGS__

#define CONFIG_samba 1
#define ENABLE_samba 1
#define USE_samba(...)  __VA_ARGS__
#define SKIP_samba(...)

#undef CONFIG_smtpclient
#define ENABLE_smtpclient 0
#define USE_smtpclient(...)
#define SKIP_smtpclient(...)  __VA_ARGS__

#undef CONFIG_pppd
#define ENABLE_pppd 0
#define USE_pppd(...)
#define SKIP_pppd(...)  __VA_ARGS__

#undef CONFIG_syslog
#define ENABLE_syslog 0
#define USE_syslog(...)
#define SKIP_syslog(...)  __VA_ARGS__

#undef CONFIG_logrotate
#define ENABLE_logrotate 0
#define USE_logrotate(...)
#define SKIP_logrotate(...)  __VA_ARGS__


/*
 * Octeon Utilities
 */
#define OCTEON_UTILS 1
#define ENABLE_UTILS 1
#define USE_UTILS(...)  __VA_ARGS__
#define SKIP_UTILS(...)

#define CONFIG_oct-linux-csr 1
#define ENABLE_oct-linux-csr 1
#define USE_oct-linux-csr(...)  __VA_ARGS__
#define SKIP_oct-linux-csr(...)

#define CONFIG_mtd-tools 1
#define ENABLE_mtd-tools 1
#define USE_mtd-tools(...)  __VA_ARGS__
#define SKIP_mtd-tools(...)

#undef CONFIG_broadcom-wireless-driver
#define ENABLE_broadcom-wireless-driver 0
#define USE_broadcom-wireless-driver(...)
#define SKIP_broadcom-wireless-driver(...)  __VA_ARGS__

#undef CONFIG_easyaccess
#define ENABLE_easyaccess 0
#define USE_easyaccess(...)
#define SKIP_easyaccess(...)  __VA_ARGS__

#undef CONFIG_atheros-wireless-driver
#define ENABLE_atheros-wireless-driver 0
#define USE_atheros-wireless-driver(...)
#define SKIP_atheros-wireless-driver(...)  __VA_ARGS__

#define CONFIG_final-cleanup 1
#define ENABLE_final-cleanup 1
#define USE_final-cleanup(...)  __VA_ARGS__
#define SKIP_final-cleanup(...)

#define CFG_EXTRA_FILES_DIR ""
#define ENABLE_EXTRA_FILES_DIR 1
#define USE_EXTRA_FILES_DIR(...)  __VA_ARGS__
#define SKIP_EXTRA_FILES_DIR(...)

#undef CONFIG_components-common
#define ENABLE_components-common 0
#define USE_components-common(...)
#define SKIP_components-common(...)  __VA_ARGS__

#undef CONFIG_pci-base
#define ENABLE_pci-base 0
#define USE_pci-base(...)
#define SKIP_pci-base(...)  __VA_ARGS__

#endif /* BB_CONFIG_H */

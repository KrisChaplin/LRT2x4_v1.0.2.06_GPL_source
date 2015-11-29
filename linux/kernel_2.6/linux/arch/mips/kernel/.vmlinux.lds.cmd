cmd_arch/mips/kernel/vmlinux.lds := mips64-octeon-linux-gnu-gcc -E -Wp,-MD,arch/mips/kernel/.vmlinux.lds.d  -nostdinc -isystem /usr/local/cavium/tools-gcc-4.1/bin/../lib/gcc/mips64-octeon-linux-gnu/4.1.1/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -include include/linux/nkuserlandconf.h  -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -ffreestanding -O2     -fomit-frame-pointer  -G 0 -mno-abicalls -fno-pic -pipe -msoft-float  -mabi=64 -march=octeon -Wa,--trap -Iinclude/asm-mips/mach-cavium-octeon -Iinclude/asm-mips/mach-generic -D"LOADADDR=0xffffffff81100000" -D"JIFFIES=jiffies_64" -D"DATAOFFSET=0" -P -C -Umips -D__ASSEMBLY__ -o arch/mips/kernel/vmlinux.lds arch/mips/kernel/vmlinux.lds.S

deps_arch/mips/kernel/vmlinux.lds := \
  arch/mips/kernel/vmlinux.lds.S \
    $(wildcard include/config/boot/elf64.h) \
    $(wildcard include/config/mapped/kernel.h) \
  include/linux/nkuserlandconf.h \
    $(wildcard include/config/signature/check.h) \
    $(wildcard include/config/nk/num/lan.h) \
    $(wildcard include/config/nk/max/num/wan.h) \
    $(wildcard include/config/nk/num/wan.h) \
    $(wildcard include/config/nk/num/usb.h) \
    $(wildcard include/config/nk/num/dmz.h) \
    $(wildcard include/config/nk/num/def/wan.h) \
    $(wildcard include/config/nk/num/vpn.h) \
    $(wildcard include/config/model/qvm2100.h) \
    $(wildcard include/config/model/qvm2000.h) \
    $(wildcard include/config/model/rv0xx.h) \
    $(wildcard include/config/model/lrtxxx.h) \
    $(wildcard include/config/nk/ipsec.h) \
    $(wildcard include/config/nk/ezlinkvpn.h) \
    $(wildcard include/config/nk/ipsec/rw/conn.h) \
    $(wildcard include/config/nk/ssl.h) \
    $(wildcard include/config/nk/ssl/upgrade.h) \
    $(wildcard include/config/nk/usbkey/server.h) \
    $(wildcard include/config/nk/qvm/server.h) \
    $(wildcard include/config/nk/padt/always.h) \
    $(wildcard include/config/nk/padi/retry.h) \
    $(wildcard include/config/nk/proto/binding.h) \
    $(wildcard include/config/nk/nsd.h) \
    $(wildcard include/config/nk/ip/base.h) \
    $(wildcard include/config/nk/rip.h) \
    $(wildcard include/config/nk/static/route.h) \
    $(wildcard include/config/nk/route/cache/modify.h) \
    $(wildcard include/config/nk/cpld.h) \
    $(wildcard include/config/support/pptpd.h) \
    $(wildcard include/config/support/pptpd/read/configfile/by/signal.h) \
    $(wildcard include/config/support/ip/mac/binding.h) \
    $(wildcard include/config/support/arp/spoof/protect.h) \
    $(wildcard include/config/nk/session/limit.h) \
    $(wildcard include/config/nk/session/limit/enhance.h) \
    $(wildcard include/config/nk/ipfilter/support/sorting.h) \
    $(wildcard include/config/nk/phylink/check.h) \
    $(wildcard include/config/nk/transparent/bridge.h) \
    $(wildcard include/config/nk/transparent/bridge/range/num.h) \
    $(wildcard include/config/nk/dos/enhancement.h) \
    $(wildcard include/config/nk/mlan.h) \
    $(wildcard include/config/nk/ddns/qdns.h) \
    $(wildcard include/config/nk/broadcast/prevention.h) \
    $(wildcard include/config/nk/change/src/port.h) \
    $(wildcard include/config/nk/content/filter.h) \
    $(wildcard include/config/nk/ssl/pass/html.h) \
    $(wildcard include/config/nk/ipsec/multiple/pass/through.h) \
    $(wildcard include/config/nk/restrict/app.h) \
    $(wildcard include/config/nk/snmp.h) \
    $(wildcard include/config/nk/ha.h) \
    $(wildcard include/config/nk/nat/firewall/rule/by/file.h) \
    $(wildcard include/config/nk/dynamic/port/num.h) \
    $(wildcard include/config/nk/num/max/lan.h) \
    $(wildcard include/config/nk/num/max/wan.h) \
    $(wildcard include/config/nk/nat/mode.h) \
    $(wildcard include/config/nk/line/drop.h) \
    $(wildcard include/config/ipbalance/enhance.h) \
    $(wildcard include/config/nk/sb/enhancement.h) \
    $(wildcard include/config/nk/import/export/adv.h) \
    $(wildcard include/config/nk/support/cn5010.h) \
    $(wildcard include/config/nk/support/multi/core.h) \
    $(wildcard include/config/nk/solve/ipsec/dualwan/multi/core.h) \
    $(wildcard include/config/nk/solve/multi/core/multi/wan/crash.h) \
    $(wildcard include/config/nk/qrtg/sys/inf.h) \
    $(wildcard include/config/nk/support/multiple/mtu.h) \
    $(wildcard include/config/nk/https/control.h) \
    $(wildcard include/config/easyaccess.h) \
    $(wildcard include/config/nk/db/checksum.h) \
    $(wildcard include/config/nk/port/trigger.h) \
    $(wildcard include/config/nk/local/dns/database.h) \
    $(wildcard include/config/nk/dhcp/relay.h) \
    $(wildcard include/config/nk/switch/arch.h) \
    $(wildcard include/config/nk/ipsec/splitdns.h) \
    $(wildcard include/config/nk/ipsec/netbios/bc.h) \
    $(wildcard include/config/nk/cramfs.h) \
    $(wildcard include/config/nk/nsd/netroute/change.h) \
    $(wildcard include/config/nk/pptpserver/mppe.h) \
    $(wildcard include/config/ipv6.h) \
    $(wildcard include/config/nk/ipv6/addon.h) \
    $(wildcard include/config/ipv6/mroute.h) \
    $(wildcard include/config/ipv6/subtrees.h) \
    $(wildcard include/config/nk/openvpn/server.h) \
    $(wildcard include/config/nk/new/syslog/design.h) \
    $(wildcard include/config/nk/8021q/simple.h) \
    $(wildcard include/config/nk/dhcp/server/relay.h) \
    $(wildcard include/config/nk/l2tp/client.h) \
    $(wildcard include/config/nk/backup/db.h) \
    $(wildcard include/config/nk/ip/conntrack/enhance.h) \
  include/linux/config.h \
    $(wildcard include/config/h.h) \
  include/asm/asm-offsets.h \
  include/asm-generic/vmlinux.lds.h \

arch/mips/kernel/vmlinux.lds: $(deps_arch/mips/kernel/vmlinux.lds)

$(deps_arch/mips/kernel/vmlinux.lds):

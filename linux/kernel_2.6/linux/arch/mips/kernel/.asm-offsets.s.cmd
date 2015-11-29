cmd_arch/mips/kernel/asm-offsets.s := mips64-octeon-linux-gnu-gcc -Wp,-MD,arch/mips/kernel/.asm-offsets.s.d  -nostdinc -isystem /usr/local/cavium/tools-gcc-4.1/bin/../lib/gcc/mips64-octeon-linux-gnu/4.1.1/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -include include/linux/nkuserlandconf.h -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -ffreestanding -O2     -fomit-frame-pointer  -G 0 -mno-abicalls -fno-pic -pipe -msoft-float  -mabi=64 -march=octeon -Wa,--trap -Iinclude/asm-mips/mach-cavium-octeon -Iinclude/asm-mips/mach-generic -Wdeclaration-after-statement -Wno-pointer-sign    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(asm_offsets)"  -D"KBUILD_MODNAME=KBUILD_STR(asm_offsets)" -S -o arch/mips/kernel/asm-offsets.s arch/mips/kernel/asm-offsets.c 

deps_arch/mips/kernel/asm-offsets.s := \
  arch/mips/kernel/asm-offsets.c \
    $(wildcard include/config/cpu/cavium/octeon.h) \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/64bit.h) \
    $(wildcard include/config/mips32/compat.h) \
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
  include/linux/compat.h \
    $(wildcard include/config/compat.h) \
  include/linux/stat.h \
  include/asm/stat.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  include/linux/compiler.h \
  include/linux/compiler-gcc4.h \
    $(wildcard include/config/forced/inlining.h) \
  include/linux/compiler-gcc.h \
  include/asm/posix_types.h \
  include/asm/sgidefs.h \
  include/asm/types.h \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/64bit/phys/addr.h) \
    $(wildcard include/config/lbd.h) \
  include/linux/time.h \
  include/linux/seqlock.h \
  include/linux/spinlock.h \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/preempt.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
  include/linux/thread_info.h \
  include/linux/bitops.h \
  include/asm/bitops.h \
    $(wildcard include/config/cpu/mips32.h) \
    $(wildcard include/config/cpu/mips64.h) \
  include/asm/bug.h \
    $(wildcard include/config/bug.h) \
  include/asm/break.h \
  include/asm-generic/bug.h \
  include/asm/byteorder.h \
    $(wildcard include/config/cpu/mipsr2.h) \
  include/linux/byteorder/big_endian.h \
  include/linux/byteorder/swab.h \
  include/linux/byteorder/generic.h \
  include/asm/cpu-features.h \
    $(wildcard include/config/cpu/mipsr2/irq/vi.h) \
    $(wildcard include/config/cpu/mipsr2/irq/ei.h) \
  include/asm/cpu.h \
  include/asm/cpu-info.h \
    $(wildcard include/config/sgi/ip27.h) \
  include/asm/cache.h \
    $(wildcard include/config/mips/l1/cache/shift.h) \
  include/asm-mips/mach-generic/kmalloc.h \
    $(wildcard include/config/dma/coherent.h) \
  include/asm-mips/mach-cavium-octeon/cpu-feature-overrides.h \
  include/asm/interrupt.h \
    $(wildcard include/config/irq/cpu.h) \
  include/asm/hazards.h \
    $(wildcard include/config/cpu/rm9000.h) \
    $(wildcard include/config/cpu/r10000.h) \
    $(wildcard include/config/cpu/sb1.h) \
  include/asm/war.h \
    $(wildcard include/config/sgi/ip22.h) \
    $(wildcard include/config/sni/rm200/pci.h) \
    $(wildcard include/config/cpu/r5432.h) \
    $(wildcard include/config/sb1/pass/1/workarounds.h) \
    $(wildcard include/config/sb1/pass/2/workarounds.h) \
    $(wildcard include/config/mips/malta.h) \
    $(wildcard include/config/mips/atlas.h) \
    $(wildcard include/config/mips/sead.h) \
    $(wildcard include/config/cpu/tx49xx.h) \
    $(wildcard include/config/momenco/jaguar/atx.h) \
    $(wildcard include/config/pmc/yosemite.h) \
    $(wildcard include/config/momenco/ocelot/3.h) \
  include/asm/thread_info.h \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/page/size/8kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
    $(wildcard include/config/debug/stack/usage.h) \
  include/asm/processor.h \
    $(wildcard include/config/cavium/octeon/cvmseg/size.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/asm/cachectl.h \
  include/asm/mipsregs.h \
    $(wildcard include/config/cpu/vr41xx.h) \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/fast/access/to/thread/pointer.h) \
  include/linux/linkage.h \
  include/asm/linkage.h \
  include/asm/prefetch.h \
  include/asm/system.h \
    $(wildcard include/config/cpu/has/sync.h) \
    $(wildcard include/config/cpu/has/wb.h) \
  include/asm/addrspace.h \
    $(wildcard include/config/cpu/r4300.h) \
    $(wildcard include/config/cpu/r4x00.h) \
    $(wildcard include/config/cpu/r5000.h) \
    $(wildcard include/config/cpu/rm7000.h) \
    $(wildcard include/config/cpu/nevada.h) \
    $(wildcard include/config/cpu/r8000.h) \
    $(wildcard include/config/cpu/sb1a.h) \
  include/asm-mips/mach-generic/spaces.h \
    $(wildcard include/config/dma/noncoherent.h) \
  include/asm/dsp.h \
  include/asm/ptrace.h \
  include/asm/isadep.h \
    $(wildcard include/config/cpu/r3000.h) \
    $(wildcard include/config/cpu/tx39xx.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/printk.h) \
  /usr/local/cavium/tools-gcc-4.1/bin/../lib/gcc/mips64-octeon-linux-gnu/4.1.1/include/stdarg.h \
  include/linux/stringify.h \
  include/linux/spinlock_types.h \
  include/asm/spinlock_types.h \
  include/asm/spinlock.h \
  include/linux/spinlock_api_smp.h \
  include/asm/atomic.h \
  include/asm-generic/atomic.h \
  include/linux/param.h \
  include/asm/param.h \
  include/asm-mips/mach-cavium-octeon/param.h \
    $(wildcard include/config/cavium/octeon/simulator.h) \
  include/linux/sem.h \
    $(wildcard include/config/sysvipc.h) \
  include/linux/ipc.h \
  include/asm/ipcbuf.h \
  include/asm/sembuf.h \
  include/asm/compat.h \
  include/asm/page.h \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/limited/dma.h) \
  include/asm-generic/page.h \
  include/asm/siginfo.h \
  include/asm-generic/siginfo.h \
  include/linux/string.h \
  include/asm/string.h \
  include/linux/sched.h \
    $(wildcard include/config/detect/softlockup.h) \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/inotify.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/pm.h) \
  include/linux/capability.h \
  include/asm/current.h \
  include/linux/timex.h \
    $(wildcard include/config/time/interpolation.h) \
  include/asm/timex.h \
  include/asm-mips/mach-generic/timex.h \
  include/linux/jiffies.h \
  include/linux/calc64.h \
  include/asm/div64.h \
  include/linux/rbtree.h \
  include/linux/cpumask.h \
  include/linux/bitmap.h \
  include/linux/errno.h \
  include/asm/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/nodemask.h \
  include/linux/numa.h \
    $(wildcard include/config/flatmem.h) \
  include/asm/numnodes.h \
  include/asm/semaphore.h \
  include/linux/wait.h \
  include/linux/list.h \
  include/linux/prefetch.h \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/asm/mmu.h \
  include/asm/cputime.h \
  include/asm-generic/cputime.h \
  include/linux/smp.h \
  include/asm/smp.h \
    $(wildcard include/config/mips/mt/smp.h) \
    $(wildcard include/config/mips/mt/smtc.h) \
  include/linux/signal.h \
  include/asm/signal.h \
    $(wildcard include/config/trad/signals.h) \
    $(wildcard include/config/binfmt/irix.h) \
  include/asm/sigcontext.h \
  include/linux/securebits.h \
  include/linux/fs_struct.h \
  include/linux/completion.h \
  include/linux/pid.h \
  include/linux/percpu.h \
  include/linux/slab.h \
    $(wildcard include/config/.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/debug/slab.h) \
    $(wildcard include/config/nk/kmalloc/debug.h) \
  include/linux/gfp.h \
    $(wildcard include/config/dma/is/dma32.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
  include/linux/cache.h \
    $(wildcard include/config/x86.h) \
    $(wildcard include/config/sparc64.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/parisc.h) \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include/linux/memory_hotplug.h \
  include/linux/notifier.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
  include/asm/topology.h \
  include/asm-mips/mach-generic/topology.h \
  include/asm-generic/topology.h \
  include/asm/sparsemem.h \
  include/linux/kmalloc_sizes.h \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/large/allocs.h) \
  include/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
  include/linux/rcupdate.h \
  include/linux/auxvec.h \
  include/asm/auxvec.h \
  include/linux/resource.h \
  include/asm/resource.h \
  include/asm-generic/resource.h \
  include/linux/timer.h \
  include/linux/hrtimer.h \
    $(wildcard include/config/no/idle/hz.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/aio.h \
  include/linux/workqueue.h \
  include/linux/aio_abi.h \
  include/linux/mm.h \
    $(wildcard include/config/sysctl.h) \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/shmem.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/debug/pagealloc.h) \
  include/linux/prio_tree.h \
  include/linux/fs.h \
    $(wildcard include/config/dnotify.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/fs/xip.h) \
    $(wildcard include/config/migration.h) \
    $(wildcard include/config/security.h) \
  include/linux/limits.h \
  include/linux/ioctl.h \
  include/asm/ioctl.h \
  include/linux/kdev_t.h \
  include/linux/dcache.h \
    $(wildcard include/config/profiling.h) \
  include/linux/kobject.h \
    $(wildcard include/config/net.h) \
  include/linux/sysfs.h \
    $(wildcard include/config/sysfs.h) \
  include/linux/kref.h \
  include/linux/radix-tree.h \
  include/linux/mutex.h \
  include/linux/quota.h \
  include/linux/dqblk_xfs.h \
  include/linux/dqblk_v1.h \
  include/linux/dqblk_v2.h \
  include/linux/nfs_fs_i.h \
  include/linux/nfs.h \
  include/linux/sunrpc/msg_prot.h \
  include/linux/fcntl.h \
  include/asm/fcntl.h \
  include/asm-generic/fcntl.h \
  include/linux/err.h \
  include/asm/pgtable.h \
    $(wildcard include/config/cpu/mips32/r1.h) \
  include/asm/pgtable-64.h \
  include/asm-generic/pgtable-nopud.h \
  include/asm/pgtable-bits.h \
    $(wildcard include/config/mips/uncached.h) \
  include/asm/io.h \
    $(wildcard include/config/swap/io/space.h) \
  include/asm-mips/mach-generic/ioremap.h \
  include/asm-mips/mach-generic/mangle-port.h \
  include/asm-generic/pgtable.h \
  include/linux/page-flags.h \
    $(wildcard include/config/swap.h) \
  include/linux/interrupt.h \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/generic/irq/probe.h) \
  include/linux/hardirq.h \
    $(wildcard include/config/preempt/bkl.h) \
    $(wildcard include/config/virt/cpu/accounting.h) \
  include/linux/smp_lock.h \
    $(wildcard include/config/lock/kernel.h) \
  include/asm/hardirq.h \
  include/linux/irq.h \
    $(wildcard include/config/s390.h) \
    $(wildcard include/config/irq/release/method.h) \
    $(wildcard include/config/generic/pending/irq.h) \
    $(wildcard include/config/irqbalance.h) \
    $(wildcard include/config/pci/msi.h) \
    $(wildcard include/config/auto/irq/affinity.h) \
  include/asm/irq.h \
    $(wildcard include/config/i8259.h) \
  include/asm-mips/mach-generic/irq.h \
  include/asm/hw_irq.h \
  include/linux/profile.h \
  include/linux/irq_cpustat.h \

arch/mips/kernel/asm-offsets.s: $(deps_arch/mips/kernel/asm-offsets.s)

$(deps_arch/mips/kernel/asm-offsets.s):

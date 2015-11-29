

/*
 * Automatically generated C config: don't edit
 * Linux kernel version: 2.6.16.26
 * Mon Mar 24 14:14:24 2014
 */



/*
 * Machine selection
 */
/*
 * CPU selection
 */
/*
 * Kernel type
 */
/*
 * Code maturity level options
 */
/*
 * General setup
 */
/*
 * Loadable module support
 */
/*
 * Block layer
 */
/*
 * IO Schedulers
 */
/*
 * Bus options (PCI, PCMCIA, EISA, ISA, TC)
 */
/*
 * PCCARD (PCMCIA/CardBus) support
 */
/*
 * PC-card bridges
 */
/*
 * PCI Hotplug Support
 */
/*
 * Executable file formats
 */
/*
 * Networking
 */
/*
 * Networking options
 */
/*
 * IP: Virtual Server Configuration
 */
/*
 * Core Netfilter Configuration
 */
/*
 * IP: Netfilter Configuration
 */
/*
 * IPv6: Netfilter Configuration (EXPERIMENTAL)
 */
/*
 * Bridge: Netfilter Configuration
 */
/*
 * DCCP Configuration (EXPERIMENTAL)
 */
/*
 * SCTP Configuration (EXPERIMENTAL)
 */
/*
 * TIPC Configuration (EXPERIMENTAL)
 */
/*
 * QoS and/or fair queueing
 */
/*
 * Queueing/Scheduling
 */
/*
 * Classification
 */
/*
 * Network testing
 */
/*
 * Device Drivers
 */
/*
 * Generic Driver Options
 */
/*
 * Connector - unified userspace <-> kernelspace linker
 */
/*
 * Memory Technology Devices (MTD)
 */
/*
 * User Modules And Translation Layers
 */
/*
 * RAM/ROM/Flash chip drivers
 */
/*
 * Mapping drivers for chip access
 */
/*
 * Self-contained MTD device drivers
 */
/*
 * Disk-On-Chip Device Drivers
 */
/*
 * NAND Flash Device Drivers
 */
/*
 * OneNAND Flash Device Drivers
 */
/*
 * Parallel port support
 */
/*
 * Plug and Play support
 */
/*
 * Block devices
 */
/*
 * ATA/ATAPI/MFM/RLL support
 */
/*
 * Please see Documentation/ide.txt for help/info on IDE drives
 */
/*
 * IDE chipset support/bugfixes
 */
/*
 * SCSI device support
 */
/*
 * Old CD-ROM drivers (not SCSI, not IDE)
 */
/*
 * Multi-device support (RAID and LVM)
 */
/*
 * Fusion MPT device support
 */
/*
 * IEEE 1394 (FireWire) support
 */
/*
 * I2O device support
 */
/*
 * Network device support
 */
/*
 * ARCnet devices
 */
/*
 * PHY device support
 */
/*
 * Ethernet (10 or 100Mbit)
 */
/*
 * Tulip family network device support
 */
/*
 * Ethernet (1000 Mbit)
 */
/*
 * Ethernet (10000 Mbit)
 */
/*
 * Token Ring devices
 */
/*
 * Wireless LAN (non-hamradio)
 */
/*
 * Obsolete Wireless cards support (pre-802.11)
 */
/*
 * Wireless 802.11b ISA/PCI cards support
 */
/*
 * Prism GT/Duette 802.11(a/b/g) PCI/Cardbus support
 */
/*
 * Wan interfaces
 */
/*
 * ISDN subsystem
 */
/*
 * Telephony Support
 */
/*
 * Input device support
 */
/*
 * Userland interfaces
 */
/*
 * Input Device Drivers
 */
/*
 * Hardware I/O ports
 */
/*
 * Character devices
 */
/*
 * Serial drivers
 */
/*
 * Non-8250 serial port support
 */
/*
 * IPMI
 */
/*
 * Watchdog Cards
 */
/*
 * Ftape, the floppy tape device driver
 */
/*
 * TPM devices
 */
/*
 * I2C support
 */
/*
 * I2C Algorithms
 */
/*
 * I2C Hardware Bus support
 */
/*
 * Miscellaneous I2C Chip support
 */
/*
 * SPI support
 */
/*
 * Dallas's 1-wire bus
 */
/*
 * Hardware Monitoring support
 */
/*
 * Misc devices
 */
/*
 * Multimedia Capabilities Port drivers
 */
/*
 * Multimedia devices
 */
/*
 * Digital Video Broadcasting Devices
 */
/*
 * Graphics support
 */
/*
 * Console display driver support
 */
/*
 * Logo configuration
 */
/*
 * Sound
 */
/*
 * USB support
 */
/*
 * NOTE: USB_STORAGE enables SCSI, and 'SCSI disk support'
 */
/*
 * USB Gadget Support
 */
/*
 * MMC/SD Card support
 */
/*
 * InfiniBand support
 */
/*
 * EDAC - error detection and reporting (RAS) (EXPERIMENTAL)
 */
/*
 * File systems
 */
/*
 * CD-ROM/DVD Filesystems
 */
/*
 * DOS/FAT/NT Filesystems
 */
/*
 * Pseudo filesystems
 */
/*
 * Miscellaneous filesystems
 */
/*
 * Network File Systems
 */
/*
 * Partition Types
 */
/*
 * Native Language Support
 */
/*
 * Profiling support
 */
/*
 * Kernel hacking
 */
/*
 * Security options
 */
/*
 * Cryptographic options
 */
/*
 * Hardware crypto devices
 */
/*
 * Library routines
 */
/*
 *
 *
 * --> come from model-userlandconf.h
 */
/*
 * feature setup
*/
/* purpose     : 0013121    author : David    date : 2010-08-04    	       */
/* description : Change CONFIG_NK_NUM_WAN from 4 to 7 to fix WAN 6~7 issue     */
/*#define CONFIG_NK_IPSEC_RW_CONN 1*/
/*
* nk define
*/
/*# Ryoko : support CPLD*/
/*# Ryoko : support PPTP Server*/
/* ip & mac binding*/
/*prevent arp attack*/
/*session limit*/
/* traffic statistics */
/* transparent bridge */
/* dos enhancement */
/*#define CONFIG_NK_DOS_ENHANCEMENT*/
/* multiple subnet */
/* Qno ddns */
/*#define CONFIG_NK_DDNS_QDNS 1*/
/* support WAN interface MAC address filter */
/* content filter*/
/* When disable remote management, let SSL portal UI and Qnokey page come in */
/* IPSec Multiple Passthrough */
/* restrict applications : msn, bt, QQ, skype */
/*trenchen : support HA*/
/*#define CONFIG_NK_HA 1*/
/* Share Architecture, Dynamic LAN/WAN Numbers, First for 1450 -- incifer 2009/04 */
/* Some varibales are pre declared, you can use it to re declare it -- incifer 2009/06 */
/*support Line Drop*/
/*#define CONFIG_NK_LINE_DROP 1*/
/* support IP balance enhancement -- incifer 2008/11 */
/*#define CONFIG_IPBALANCE_ENHANCE 1*/
/*#define CONFIG_NK_SB_ENHANCEMENT 1*/
/* support CN5020 dual-core. Hope it supports CN5230 multi-core eventually too */
/*--> Rain 20091022, solve IPSec crash over dual wan with multi-core CPU */
/* support CN5020 dual-core. SOLVE_MULTI_CORE_MULTI_WAN_CRASH */
/* QRTG log_cpu_info.htm log_wan_info.htm*/
/*#define CONFIG_NK_QRTG_SYS_INF 1*/
/*support Multiple MTU add by jason.huang 2010/01/18*/
/* selena : support HTTPS Control */
/* selena : if support EesyAccess, originally defined in *.config */
/* Paul : support port trigger */
/* Paul : support local DNS database */
/* Paul : support DHCP Relay */
/* selena : support HTTPS Control */
/* selena : if support EesyAccess, originally defined in *.config */
/**
 * New Switch Arch -- incifer 2010/01
 **/
/*20100428 trenchen : support pptp server using mppe*/
/*purpose     : Load_U_v4  author : Ben date : 2010-08-04*/
/*description : support Load_U v4 */
/*purpose     : CONFIG_NK_NEW_SYSLOG_DESIGN author : FRANK date : 2012-4-13*/
/*description : Support CONFIG_NK_NEW_SYSLOG_DESIGN*/
/* #define CONFIG_NK_BACKUP_DB 1 */
/* This file is no longer in use and kept only for backward compatibility.
 * autoconf.h is now included via -imacros on the commandline
 */
/*
 * Automatically generated C config: don't edit
 * Linux kernel version: 2.6.16.26
 * Mon Mar 24 14:14:24 2014
 */
/*
 * Machine selection
 */
/*
 * CPU selection
 */
/*
 * Kernel type
 */
/*
 * Code maturity level options
 */
/*
 * General setup
 */
/*
 * Loadable module support
 */
/*
 * Block layer
 */
/*
 * IO Schedulers
 */
/*
 * Bus options (PCI, PCMCIA, EISA, ISA, TC)
 */
/*
 * PCCARD (PCMCIA/CardBus) support
 */
/*
 * PC-card bridges
 */
/*
 * PCI Hotplug Support
 */
/*
 * Executable file formats
 */
/*
 * Networking
 */
/*
 * Networking options
 */
/*
 * IP: Virtual Server Configuration
 */
/*
 * Core Netfilter Configuration
 */
/*
 * IP: Netfilter Configuration
 */
/*
 * IPv6: Netfilter Configuration (EXPERIMENTAL)
 */
/*
 * Bridge: Netfilter Configuration
 */
/*
 * DCCP Configuration (EXPERIMENTAL)
 */
/*
 * SCTP Configuration (EXPERIMENTAL)
 */
/*
 * TIPC Configuration (EXPERIMENTAL)
 */
/*
 * QoS and/or fair queueing
 */
/*
 * Queueing/Scheduling
 */
/*
 * Classification
 */
/*
 * Network testing
 */
/*
 * Device Drivers
 */
/*
 * Generic Driver Options
 */
/*
 * Connector - unified userspace <-> kernelspace linker
 */
/*
 * Memory Technology Devices (MTD)
 */
/*
 * User Modules And Translation Layers
 */
/*
 * RAM/ROM/Flash chip drivers
 */
/*
 * Mapping drivers for chip access
 */
/*
 * Self-contained MTD device drivers
 */
/*
 * Disk-On-Chip Device Drivers
 */
/*
 * NAND Flash Device Drivers
 */
/*
 * OneNAND Flash Device Drivers
 */
/*
 * Parallel port support
 */
/*
 * Plug and Play support
 */
/*
 * Block devices
 */
/*
 * ATA/ATAPI/MFM/RLL support
 */
/*
 * Please see Documentation/ide.txt for help/info on IDE drives
 */
/*
 * IDE chipset support/bugfixes
 */
/*
 * SCSI device support
 */
/*
 * Old CD-ROM drivers (not SCSI, not IDE)
 */
/*
 * Multi-device support (RAID and LVM)
 */
/*
 * Fusion MPT device support
 */
/*
 * IEEE 1394 (FireWire) support
 */
/*
 * I2O device support
 */
/*
 * Network device support
 */
/*
 * ARCnet devices
 */
/*
 * PHY device support
 */
/*
 * Ethernet (10 or 100Mbit)
 */
/*
 * Tulip family network device support
 */
/*
 * Ethernet (1000 Mbit)
 */
/*
 * Ethernet (10000 Mbit)
 */
/*
 * Token Ring devices
 */
/*
 * Wireless LAN (non-hamradio)
 */
/*
 * Obsolete Wireless cards support (pre-802.11)
 */
/*
 * Wireless 802.11b ISA/PCI cards support
 */
/*
 * Prism GT/Duette 802.11(a/b/g) PCI/Cardbus support
 */
/*
 * Wan interfaces
 */
/*
 * ISDN subsystem
 */
/*
 * Telephony Support
 */
/*
 * Input device support
 */
/*
 * Userland interfaces
 */
/*
 * Input Device Drivers
 */
/*
 * Hardware I/O ports
 */
/*
 * Character devices
 */
/*
 * Serial drivers
 */
/*
 * Non-8250 serial port support
 */
/*
 * IPMI
 */
/*
 * Watchdog Cards
 */
/*
 * Ftape, the floppy tape device driver
 */
/*
 * TPM devices
 */
/*
 * I2C support
 */
/*
 * I2C Algorithms
 */
/*
 * I2C Hardware Bus support
 */
/*
 * Miscellaneous I2C Chip support
 */
/*
 * SPI support
 */
/*
 * Dallas's 1-wire bus
 */
/*
 * Hardware Monitoring support
 */
/*
 * Misc devices
 */
/*
 * Multimedia Capabilities Port drivers
 */
/*
 * Multimedia devices
 */
/*
 * Digital Video Broadcasting Devices
 */
/*
 * Graphics support
 */
/*
 * Console display driver support
 */
/*
 * Logo configuration
 */
/*
 * Sound
 */
/*
 * USB support
 */
/*
 * NOTE: USB_STORAGE enables SCSI, and 'SCSI disk support'
 */
/*
 * USB Gadget Support
 */
/*
 * MMC/SD Card support
 */
/*
 * InfiniBand support
 */
/*
 * EDAC - error detection and reporting (RAS) (EXPERIMENTAL)
 */
/*
 * File systems
 */
/*
 * CD-ROM/DVD Filesystems
 */
/*
 * DOS/FAT/NT Filesystems
 */
/*
 * Pseudo filesystems
 */
/*
 * Miscellaneous filesystems
 */
/*
 * Network File Systems
 */
/*
 * Partition Types
 */
/*
 * Native Language Support
 */
/*
 * Profiling support
 */
/*
 * Kernel hacking
 */
/*
 * Security options
 */
/*
 * Cryptographic options
 */
/*
 * Hardware crypto devices
 */
/*
 * Library routines
 */
/*
 * DO NOT MODIFY.
 *
 * This file was generated by /netklass/GPL/LRT2x4/v1.0.2.06/Publication_Tarball/LRT2x4_v1.0.2.06_GPL_tarball_20140321/LRT2x4_v1.0.2.06_GPL_tarball/LRT2x4_v1.0.2.06_GPL/LRT2x4_v1.0.2.06_GPL_source/linux/kernel_2.6/linux/Kbuild
 *
 */
/* MIPS pt_regs offsets. */
/* MIPS task_struct offsets. */
/* MIPS thread_info offsets. */
/* MIPS specific thread_struct offsets. */
/* Linux sigcontext offsets. */
/* Linux 32-bit sigcontext offsets. */
/* Linux signal numbers. */
/* Linux irq_cpustat_t offsets. */
/* Octeon specific octeon_cop2_state offsets. */
/* Size of struct page  */
/* Linux mm_struct offsets. */
/* Align . to a 8 byte boundary equals to maximum function alignment. */
/* sched.text is aling to function alignment to secure we have same
 * address even at second ld pass when generating System.map */
/* spinlock.text is aling to function alignment to secure we have same
 * address even at second ld pass when generating System.map */
  /* DWARF debug sections.
		Symbols in the DWARF debugging sections are relative to
		the beginning of the section so we begin them at 0.  */
  /* Stabs debugging sections.  */
OUTPUT_ARCH(mips)
ENTRY(kernel_entry)
jiffies = jiffies_64;
SECTIONS
{
  . = 0xffffffff81100000;
  /* read-only */
  _text = .; /* Text and read-only data */
  .text : {
    *(.text)
    . = ALIGN(8); __sched_text_start = .; *(.sched.text) __sched_text_end = .;
    . = ALIGN(8); __lock_text_start = .; *(.spinlock.text) __lock_text_end = .;
    *(.fixup)
    *(.gnu.warning)
  } =0
  _etext = .; /* End of text section */
  . = ALIGN(16); /* Exception table */
  __start___ex_table = .;
  __ex_table : { *(__ex_table) }
  __stop___ex_table = .;
  __start___dbe_table = .; /* Exception table for data bus errors */
  __dbe_table : { *(__dbe_table) }
  __stop___dbe_table = .;
  . = ALIGN(4096); __start_rodata = .; .rodata : AT(ADDR(.rodata) - 0) { *(.rodata) *(.rodata.*) *(__vermagic) } .rodata1 : AT(ADDR(.rodata1) - 0) { *(.rodata1) } .pci_fixup : AT(ADDR(.pci_fixup) - 0) { __start_pci_fixups_early = .; *(.pci_fixup_early) __end_pci_fixups_early = .; __start_pci_fixups_header = .; *(.pci_fixup_header) __end_pci_fixups_header = .; __start_pci_fixups_final = .; *(.pci_fixup_final) __end_pci_fixups_final = .; __start_pci_fixups_enable = .; *(.pci_fixup_enable) __end_pci_fixups_enable = .; } .rio_route : AT(ADDR(.rio_route) - 0) { __start_rio_route_ops = .; *(.rio_route_ops) __end_rio_route_ops = .; } __ksymtab : AT(ADDR(__ksymtab) - 0) { __start___ksymtab = .; *(__ksymtab) __stop___ksymtab = .; } __ksymtab_gpl : AT(ADDR(__ksymtab_gpl) - 0) { __start___ksymtab_gpl = .; *(__ksymtab_gpl) __stop___ksymtab_gpl = .; } __kcrctab : AT(ADDR(__kcrctab) - 0) { __start___kcrctab = .; *(__kcrctab) __stop___kcrctab = .; } __kcrctab_gpl : AT(ADDR(__kcrctab_gpl) - 0) { __start___kcrctab_gpl = .; *(__kcrctab_gpl) __stop___kcrctab_gpl = .; } __ksymtab_strings : AT(ADDR(__ksymtab_strings) - 0) { *(__ksymtab_strings) } __end_rodata = .; . = ALIGN(4096); __param : AT(ADDR(__param) - 0) { __start___param = .; *(__param) __stop___param = .; }
  /* writeable */
  .data : { /* Data */
    . = . + 0; /* for CONFIG_MAPPED_KERNEL */
    *(.data.init_task)
    *(.data)
    CONSTRUCTORS
  }
  _gp = . + 0x8000;
  .lit8 : { *(.lit8) }
  .lit4 : { *(.lit4) }
  /* We want the small data sections together, so single-instruction offsets
     can access them all, and initialized data all before uninitialized, so
     we can shorten the on-disk segment size.  */
  .sdata : { *(.sdata) }
  . = ALIGN(0x1000);
  __nosave_begin = .;
  .data_nosave : { *(.data.nosave) }
  . = ALIGN(0x1000);
  __nosave_end = .;
  . = ALIGN(32);
  .data.cacheline_aligned : { *(.data.cacheline_aligned) }
  _edata = .; /* End of data section */
  /* will be freed after init */
  . = ALIGN(0x1000); /* Init code and data */
  __init_begin = .;
  .init.text : {
 _sinittext = .;
 *(.init.text)
 _einittext = .;
  }
  .init.data : { *(.init.data) }
  . = ALIGN(16);
  __setup_start = .;
  .init.setup : { *(.init.setup) }
  __setup_end = .;
  __initcall_start = .;
  .initcall.init : {
 *(.initcall1.init)
 *(.initcall2.init)
 *(.initcall3.init)
 *(.initcall4.init)
 *(.initcall5.init)
 *(.initcall6.init)
 *(.initcall7.init)
  }
  __initcall_end = .;
  __con_initcall_start = .;
  .con_initcall.init : { *(.con_initcall.init) }
  __con_initcall_end = .;
  .security_initcall.init : AT(ADDR(.security_initcall.init) - 0) { __security_initcall_start = .; *(.security_initcall.init) __security_initcall_end = .; }
    /* .exit.text is discarded at runtime, not link time, to deal with
     references from .rodata */
  .exit.text : { *(.exit.text) }
  . = ALIGN(0x1000);
  __initramfs_start = .;
  .init.ramfs : { *(.init.ramfs) }
  __initramfs_end = .;
  . = ALIGN(32);
  __per_cpu_start = .;
  .data.percpu : { *(.data.percpu) }
  __per_cpu_end = .;
  . = ALIGN(0x1000);
  __init_end = .;
  /* freed after init ends here */
  __bss_start = .; /* BSS */
  .sbss : {
    *(.sbss)
    *(.scommon)
  }
  .bss : {
    *(.bss)
    *(COMMON)
  }
  __bss_stop = .;
  _end = . ;
  /* Sections to be discarded */
  /DISCARD/ : {
        *(.exit.data)
        *(.exitcall.exit)
 /* ABI crap starts here */
 *(.comment)
 *(.MIPS.options)
 *(.note)
 *(.options)
 *(.pdr)
 *(.reginfo)
 *(.mdebug*)
  }
  /* This is the MIPS specific mdebug section.  */
  .mdebug : { *(.mdebug) }
  /* These are needed for ELF backends which have not yet been
     converted to the new style linker.  */
  .stab 0 : { *(.stab) }
  .stabstr 0 : { *(.stabstr) }
  .debug 0 : { *(.debug) } .line 0 : { *(.line) } .debug_srcinfo 0 : { *(.debug_srcinfo) } .debug_sfnames 0 : { *(.debug_sfnames) } .debug_aranges 0 : { *(.debug_aranges) } .debug_pubnames 0 : { *(.debug_pubnames) } .debug_info 0 : { *(.debug_info .gnu.linkonce.wi.*) } .debug_abbrev 0 : { *(.debug_abbrev) } .debug_line 0 : { *(.debug_line) } .debug_frame 0 : { *(.debug_frame) } .debug_str 0 : { *(.debug_str) } .debug_loc 0 : { *(.debug_loc) } .debug_macinfo 0 : { *(.debug_macinfo) } .debug_weaknames 0 : { *(.debug_weaknames) } .debug_funcnames 0 : { *(.debug_funcnames) } .debug_typenames 0 : { *(.debug_typenames) } .debug_varnames 0 : { *(.debug_varnames) }
  /* These must appear regardless of  .  */
  .gptab.sdata : { *(.gptab.data) *(.gptab.sdata) }
  .gptab.sbss : { *(.gptab.bss) *(.gptab.sbss) }
  .comment : { *(.comment) }
  .note : { *(.note) }
}

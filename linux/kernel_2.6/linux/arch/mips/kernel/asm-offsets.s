	.file	1 "asm-offsets.c"
	.section .mdebug.abi64
	.previous
#APP
		.macro	_ssnop					
	sll	$0, $0, 1				
	.endm						
							
	.macro	_ehb					
	sll	$0, $0, 3				
	.endm						

		.macro	irq_enable_hazard			
	.endm						
							
	.macro	irq_disable_hazard			
	.endm						

		.macro	back_to_back_c0_hazard			
	.endm						

		.macro	local_irq_enable				
	.set	push						
	.set	reorder						
	.set	noat						
	.set	mips32r2					
	ei							
	.set	mips0						
	irq_enable_hazard					
	.set	pop						
	.endm
		.macro	local_irq_disable
	.set	push						
	.set	noat						
	.set	mips32r2					
	di							
	.set	mips0						
	irq_disable_hazard					
	.set	pop						
	.endm							

		.macro	local_save_flags flags				
	.set	push						
	.set	reorder						
	mfc0	\flags, $12					
	.set	pop						
	.endm							

		.macro	local_irq_save result				
	.set	push						
	.set	reorder						
	.set	noat						
	.set	mips32r2					
	di	\result					
	andi	\result, 1					
	.set	mips0						
	irq_disable_hazard					
	.set	pop						
	.endm							

		.macro	local_irq_restore flags				
	.set	push						
	.set	noreorder					
	.set	noat						
	.set	mips32r2					
	beqz	\flags, 1f					
	 di							
	ei							
	.set	mips0						
1:								
	irq_disable_hazard					
	.set	pop						
	.endm							

#NO_APP
	.text
	.align	2
	.align	3
	.globl	output_ptreg_defines
	.ent	output_ptreg_defines
	.type	output_ptreg_defines, @function
output_ptreg_defines:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
@@@/* MIPS pt_regs offsets. */
	
@@@#define PT_R0     0
	
@@@#define PT_R1     8
	
@@@#define PT_R2     16
	
@@@#define PT_R3     24
	
@@@#define PT_R4     32
	
@@@#define PT_R5     40
	
@@@#define PT_R6     48
	
@@@#define PT_R7     56
	
@@@#define PT_R8     64
	
@@@#define PT_R9     72
	
@@@#define PT_R10    80
	
@@@#define PT_R11    88
	
@@@#define PT_R12    96
	
@@@#define PT_R13    104
	
@@@#define PT_R14    112
	
@@@#define PT_R15    120
	
@@@#define PT_R16    128
	
@@@#define PT_R17    136
	
@@@#define PT_R18    144
	
@@@#define PT_R19    152
	
@@@#define PT_R20    160
	
@@@#define PT_R21    168
	
@@@#define PT_R22    176
	
@@@#define PT_R23    184
	
@@@#define PT_R24    192
	
@@@#define PT_R25    200
	
@@@#define PT_R26    208
	
@@@#define PT_R27    216
	
@@@#define PT_R28    224
	
@@@#define PT_R29    232
	
@@@#define PT_R30    240
	
@@@#define PT_R31    248
	
@@@#define PT_LO     272
	
@@@#define PT_HI     264
	
@@@#define PT_EPC    296
	
@@@#define PT_BVADDR 280
	
@@@#define PT_STATUS 256
	
@@@#define PT_CAUSE  288
	
@@@#define PT_MPL    304
	
@@@#define PT_MTP    328
	
@@@#define PT_SIZE   352
	
@@@
#NO_APP
	j	$31
	.end	output_ptreg_defines
	.align	2
	.align	3
	.globl	output_task_defines
	.ent	output_task_defines
	.type	output_task_defines, @function
output_task_defines:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
@@@/* MIPS task_struct offsets. */
	
@@@#define TASK_STATE         0
	
@@@#define TASK_THREAD_INFO   8
	
@@@#define TASK_FLAGS         24
	
@@@#define TASK_MM            200
	
@@@#define TASK_PID           260
	
@@@#define TASK_STRUCT_SIZE   2432
	
@@@
#NO_APP
	j	$31
	.end	output_task_defines
	.align	2
	.align	3
	.globl	output_thread_info_defines
	.ent	output_thread_info_defines
	.type	output_thread_info_defines, @function
output_thread_info_defines:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
@@@/* MIPS thread_info offsets. */
	
@@@#define TI_TASK            0
	
@@@#define TI_EXEC_DOMAIN     8
	
@@@#define TI_FLAGS           16
	
@@@#define TI_CPU             32
	
@@@#define TI_PRE_COUNT       36
	
@@@#define TI_ADDR_LIMIT      40
	
@@@#define TI_RESTART_BLOCK   48
	
@@@#define TI_TP_VALUE	   24
	
@@@#define _THREAD_SIZE_ORDER 0x2
	
@@@#define _THREAD_SIZE       0x4000
	
@@@#define _THREAD_MASK       0x3fff
	
@@@
#NO_APP
	j	$31
	.end	output_thread_info_defines
	.align	2
	.align	3
	.globl	output_thread_defines
	.ent	output_thread_defines
	.type	output_thread_defines, @function
output_thread_defines:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
@@@/* MIPS specific thread_struct offsets. */
	
@@@#define THREAD_REG16   768
	
@@@#define THREAD_REG17   776
	
@@@#define THREAD_REG18   784
	
@@@#define THREAD_REG19   792
	
@@@#define THREAD_REG20   800
	
@@@#define THREAD_REG21   808
	
@@@#define THREAD_REG22   816
	
@@@#define THREAD_REG23   824
	
@@@#define THREAD_REG29   832
	
@@@#define THREAD_REG30   840
	
@@@#define THREAD_REG31   848
	
@@@#define THREAD_STATUS  856
	
@@@#define THREAD_FPU     864
	
@@@#define THREAD_BVADDR  1160
	
@@@#define THREAD_BUADDR  1168
	
@@@#define THREAD_ECODE   1176
	
@@@#define THREAD_TRAPNO  1184
	
@@@#define THREAD_MFLAGS  1192
	
@@@#define THREAD_TRAMP   1200
	
@@@#define THREAD_OLDCTX  1208
	
@@@
#NO_APP
	j	$31
	.end	output_thread_defines
	.align	2
	.align	3
	.globl	output_thread_fpu_defines
	.ent	output_thread_fpu_defines
	.type	output_thread_fpu_defines, @function
output_thread_fpu_defines:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
@@@#define THREAD_FPR0    864
	
@@@#define THREAD_FPR1    872
	
@@@#define THREAD_FPR2    880
	
@@@#define THREAD_FPR3    888
	
@@@#define THREAD_FPR4    896
	
@@@#define THREAD_FPR5    904
	
@@@#define THREAD_FPR6    912
	
@@@#define THREAD_FPR7    920
	
@@@#define THREAD_FPR8    928
	
@@@#define THREAD_FPR9    936
	
@@@#define THREAD_FPR10   944
	
@@@#define THREAD_FPR11   952
	
@@@#define THREAD_FPR12   960
	
@@@#define THREAD_FPR13   968
	
@@@#define THREAD_FPR14   976
	
@@@#define THREAD_FPR15   984
	
@@@#define THREAD_FPR16   992
	
@@@#define THREAD_FPR17   1000
	
@@@#define THREAD_FPR18   1008
	
@@@#define THREAD_FPR19   1016
	
@@@#define THREAD_FPR20   1024
	
@@@#define THREAD_FPR21   1032
	
@@@#define THREAD_FPR22   1040
	
@@@#define THREAD_FPR23   1048
	
@@@#define THREAD_FPR24   1056
	
@@@#define THREAD_FPR25   1064
	
@@@#define THREAD_FPR26   1072
	
@@@#define THREAD_FPR27   1080
	
@@@#define THREAD_FPR28   1088
	
@@@#define THREAD_FPR29   1096
	
@@@#define THREAD_FPR30   1104
	
@@@#define THREAD_FPR31   1112
	
@@@#define THREAD_FCR31   1120
	
@@@
#NO_APP
	j	$31
	.end	output_thread_fpu_defines
	.align	2
	.align	3
	.globl	output_sc_defines
	.ent	output_sc_defines
	.type	output_sc_defines, @function
output_sc_defines:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
@@@/* Linux sigcontext offsets. */
	
@@@#define SC_REGS       0
	
@@@#define SC_FPREGS     256
	
@@@#define SC_MDHI       512
	
@@@#define SC_MDLO       544
	
@@@#define SC_PC         576
	
@@@#define SC_FPC_CSR    584
	
@@@
#NO_APP
	j	$31
	.end	output_sc_defines
	.align	2
	.align	3
	.globl	output_sc32_defines
	.ent	output_sc32_defines
	.type	output_sc32_defines, @function
output_sc32_defines:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
@@@/* Linux 32-bit sigcontext offsets. */
	
@@@#define SC32_FPREGS     272
	
@@@#define SC32_FPC_CSR    532
	
@@@#define SC32_FPC_EIR    536
	
@@@
#NO_APP
	j	$31
	.end	output_sc32_defines
	.align	2
	.align	3
	.globl	output_signal_defined
	.ent	output_signal_defined
	.type	output_signal_defined, @function
output_signal_defined:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
@@@/* Linux signal numbers. */
	
@@@#define _SIGHUP     0x1
	
@@@#define _SIGINT     0x2
	
@@@#define _SIGQUIT    0x3
	
@@@#define _SIGILL     0x4
	
@@@#define _SIGTRAP    0x5
	
@@@#define _SIGIOT     0x6
	
@@@#define _SIGABRT    0x6
	
@@@#define _SIGEMT     0x7
	
@@@#define _SIGFPE     0x8
	
@@@#define _SIGKILL    0x9
	
@@@#define _SIGBUS     0xa
	
@@@#define _SIGSEGV    0xb
	
@@@#define _SIGSYS     0xc
	
@@@#define _SIGPIPE    0xd
	
@@@#define _SIGALRM    0xe
	
@@@#define _SIGTERM    0xf
	
@@@#define _SIGUSR1    0x10
	
@@@#define _SIGUSR2    0x11
	
@@@#define _SIGCHLD    0x12
	
@@@#define _SIGPWR     0x13
	
@@@#define _SIGWINCH   0x14
	
@@@#define _SIGURG     0x15
	
@@@#define _SIGIO      0x16
	
@@@#define _SIGSTOP    0x17
	
@@@#define _SIGTSTP    0x18
	
@@@#define _SIGCONT    0x19
	
@@@#define _SIGTTIN    0x1a
	
@@@#define _SIGTTOU    0x1b
	
@@@#define _SIGVTALRM  0x1c
	
@@@#define _SIGPROF    0x1d
	
@@@#define _SIGXCPU    0x1e
	
@@@#define _SIGXFSZ    0x1f
	
@@@
#NO_APP
	j	$31
	.end	output_signal_defined
	.align	2
	.align	3
	.globl	output_irq_cpustat_t_defines
	.ent	output_irq_cpustat_t_defines
	.type	output_irq_cpustat_t_defines, @function
output_irq_cpustat_t_defines:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
@@@/* Linux irq_cpustat_t offsets. */
	
@@@#define IC_SOFTIRQ_PENDING 0
	
@@@#define IC_IRQ_CPUSTAT_T   128
	
@@@
#NO_APP
	j	$31
	.end	output_irq_cpustat_t_defines
	.align	2
	.align	3
	.globl	output_octeon_cop2_state_defines
	.ent	output_octeon_cop2_state_defines
	.type	output_octeon_cop2_state_defines, @function
output_octeon_cop2_state_defines:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
@@@/* Octeon specific octeon_cop2_state offsets. */
	
@@@#define OCTEON_CP2_CRC_IV       0
	
@@@#define OCTEON_CP2_CRC_LENGTH   8
	
@@@#define OCTEON_CP2_CRC_POLY     16
	
@@@#define OCTEON_CP2_LLM_DAT      24
	
@@@#define OCTEON_CP2_3DES_IV      40
	
@@@#define OCTEON_CP2_3DES_KEY     48
	
@@@#define OCTEON_CP2_3DES_RESULT  72
	
@@@#define OCTEON_CP2_AES_INP0     80
	
@@@#define OCTEON_CP2_AES_IV       88
	
@@@#define OCTEON_CP2_AES_KEY      104
	
@@@#define OCTEON_CP2_AES_KEYLEN   136
	
@@@#define OCTEON_CP2_AES_RESULT   144
	
@@@#define OCTEON_CP2_GFM_MULT     344
	
@@@#define OCTEON_CP2_GFM_POLY     360
	
@@@#define OCTEON_CP2_GFM_RESULT   368
	
@@@#define OCTEON_CP2_HSH_DATW     160
	
@@@#define OCTEON_CP2_HSH_IVW      280
	
@@@#define THREAD_CP2              1280
	
@@@#define THREAD_CVMSEG           1664
	
@@@
#NO_APP
	j	$31
	.end	output_octeon_cop2_state_defines
	.align	2
	.align	3
	.globl	output_mm_defines
	.ent	output_mm_defines
	.type	output_mm_defines, @function
output_mm_defines:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
#APP
	
@@@/* Size of struct page  */
	
@@@#define STRUCT_PAGE_SIZE   56
	
@@@
	
@@@/* Linux mm_struct offsets. */
	
@@@#define MM_USERS      80
	
@@@#define MM_PGD        72
	
@@@#define MM_CONTEXT    696
	
@@@
	
@@@#define _PAGE_SIZE     0x1000
	
@@@#define _PAGE_SHIFT    0xc
	
@@@
	
@@@#define _PGD_T_SIZE    0x8
	
@@@#define _PMD_T_SIZE    0x8
	
@@@#define _PTE_T_SIZE    0x8
	
@@@
#NO_APP
	li	$2,63			# 0x3f
	li	$3,8			# 0x8
#APP
		.set	push						
	.set	mips64						
	dclz	$3, $3						
	.set	pop						

#NO_APP
	subu	$2,$2,$3
#APP
	
@@@#define _PGD_T_LOG2    $2
#NO_APP
	li	$2,63			# 0x3f
	li	$3,8			# 0x8
#APP
		.set	push						
	.set	mips64						
	dclz	$3, $3						
	.set	pop						

#NO_APP
	subu	$2,$2,$3
#APP
	
@@@#define _PMD_T_LOG2    $2
#NO_APP
	li	$2,63			# 0x3f
	li	$3,8			# 0x8
#APP
		.set	push						
	.set	mips64						
	dclz	$3, $3						
	.set	pop						

#NO_APP
	subu	$2,$2,$3
#APP
	
@@@#define _PTE_T_LOG2    $2
	
@@@
	
@@@#define _PMD_SHIFT     0x15
	
@@@#define _PGDIR_SHIFT   0x1e
	
@@@
	
@@@#define _PGD_ORDER     0x1
	
@@@#define _PMD_ORDER     0x0
	
@@@#define _PTE_ORDER     0x0
	
@@@
	
@@@#define _PTRS_PER_PGD  0x400
	
@@@#define _PTRS_PER_PMD  0x200
	
@@@#define _PTRS_PER_PTE  0x200
	
@@@
#NO_APP
	j	$31
	.end	output_mm_defines
	.ident	"GCC: (GNU) 4.1.1 (Cavium Networks Version: 1_5_0, build 14)"

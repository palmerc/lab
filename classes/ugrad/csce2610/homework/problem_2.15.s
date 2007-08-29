	.file	1 "problem_2.15.c"
	.section .mdebug.abi32
	.previous
	.abicalls
	.text
	.align	2
	.globl	set_array
	.ent	set_array
	.type	set_array, @function
set_array:
	.frame	$fp,88,$31		# vars= 48, regs= 3/0, args= 16, gp= 8
	.mask	0xc0010000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	reorder
	addiu	$sp,$sp,-88
	sw	$31,80($sp)
	sw	$fp,76($sp)
	sw	$16,72($sp)
	move	$fp,$sp
	.cprestore	16
	sw	$4,88($fp)
	sw	$0,24($fp)
$L2:
	lw	$2,24($fp)
	slt	$2,$2,10
	beq	$2,$0,$L1
	lw	$2,24($fp)
	sll	$3,$2,2
	addiu	$2,$fp,24
	addu	$16,$3,$2
	lw	$4,88($fp)
	lw	$5,24($fp)
	jal	compare
	sw	$2,8($16)
	lw	$2,24($fp)
	addiu	$2,$2,1
	sw	$2,24($fp)
	b	$L2
$L1:
	move	$sp,$fp
	lw	$31,80($sp)
	lw	$fp,76($sp)
	lw	$16,72($sp)
	addiu	$sp,$sp,88
	j	$31
	.end	set_array
	.align	2
	.globl	compare
	.ent	compare
	.type	compare, @function
compare:
	.frame	$fp,40,$31		# vars= 8, regs= 2/0, args= 16, gp= 8
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	reorder
	addiu	$sp,$sp,-40
	sw	$31,36($sp)
	sw	$fp,32($sp)
	move	$fp,$sp
	.cprestore	16
	sw	$4,40($fp)
	sw	$5,44($fp)
	lw	$4,40($fp)
	lw	$5,44($fp)
	jal	sub
	bltz	$2,$L6
	li	$2,1			# 0x1
	sw	$2,24($fp)
	b	$L5
$L6:
	sw	$0,24($fp)
$L5:
	lw	$2,24($fp)
	move	$sp,$fp
	lw	$31,36($sp)
	lw	$fp,32($sp)
	addiu	$sp,$sp,40
	j	$31
	.end	compare
	.align	2
	.globl	sub
	.ent	sub
	.type	sub, @function
sub:
	.frame	$fp,8,$31		# vars= 0, regs= 1/0, args= 0, gp= 0
	.mask	0x40000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	reorder
	addiu	$sp,$sp,-8
	sw	$fp,0($sp)
	move	$fp,$sp
	sw	$4,8($fp)
	sw	$5,12($fp)
	lw	$3,8($fp)
	lw	$2,12($fp)
	subu	$2,$3,$2
	move	$sp,$fp
	lw	$fp,0($sp)
	addiu	$sp,$sp,8
	j	$31
	.end	sub
	.align	2
	.globl	main
	.ent	main
	.type	main, @function
main:
	.frame	$fp,32,$31		# vars= 0, regs= 2/0, args= 16, gp= 8
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	reorder
	addiu	$sp,$sp,-32
	sw	$31,28($sp)
	sw	$fp,24($sp)
	move	$fp,$sp
	.cprestore	16
	li	$4,4			# 0x4
	jal	set_array
	move	$sp,$fp
	lw	$31,28($sp)
	lw	$fp,24($sp)
	addiu	$sp,$sp,32
	j	$31
	.end	main
	.ident	"GCC: (GNU) 3.4.2"

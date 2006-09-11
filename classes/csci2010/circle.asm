	.file	"circle.cpp"
	.version	"01.01"
gcc2_compiled.:
.globl __throw
.section	.rodata
	.align 32
.LC1:
	.string	"Input the radius of the circle."
.LC2:
	.string	"The area of the circle is: "
	.align 4
.LC0:
	.long 0x40490fdb
.text
	.align 4
.globl main
	.type	 main,@function
main:
.LFB1:
	pushl %ebp
.LCFI0:
	movl %esp,%ebp
.LCFI1:
	subl $24,%esp
.LCFI2:
	flds .LC0
	fstps -8(%ebp)
	addl $-8,%esp
	pushl $endl__FR7ostream
	addl $-8,%esp
	pushl $.LC1
	pushl $cout
.LCFI3:
	call __ls__7ostreamPCc
	addl $16,%esp
	movl %eax,%eax
	pushl %eax
	call __ls__7ostreamPFR7ostream_R7ostream
	addl $16,%esp
	addl $-8,%esp
	leal -4(%ebp),%eax
	pushl %eax
	pushl $cin
	call __rs__7istreamRi
	addl $16,%esp
	addl $-8,%esp
	pushl $.LC2
	pushl $cout
	call __ls__7ostreamPCc
	addl $16,%esp
	addl $-8,%esp
	movl -4(%ebp),%eax
	imull -4(%ebp),%eax
	movl %eax,-12(%ebp)
	fildl -12(%ebp)
	flds .LC0
	fmulp %st,%st(1)
	subl $4,%esp
	fstps (%esp)
	pushl $cout
	call __ls__7ostreamf
	addl $16,%esp
	addl $-8,%esp
	pushl $endl__FR7ostream
	pushl $cout
	call __ls__7ostreamPFR7ostream_R7ostream
	addl $16,%esp
	xorl %eax,%eax
	jmp .L271
	xorl %eax,%eax
	jmp .L271
	.p2align 4,,7
.L271:
	leave
	ret
.LFE1:
.Lfe1:
	.size	 main,.Lfe1-main

.section	.eh_frame,"aw",@progbits
__FRAME_BEGIN__:
	.4byte	.LLCIE1
.LSCIE1:
	.4byte	0x0
	.byte	0x1
	.byte	0x0
	.byte	0x1
	.byte	0x7c
	.byte	0x8
	.byte	0xc
	.byte	0x4
	.byte	0x4
	.byte	0x88
	.byte	0x1
	.align 4
.LECIE1:
	.set	.LLCIE1,.LECIE1-.LSCIE1
	.4byte	.LLFDE1
.LSFDE1:
	.4byte	.LSFDE1-__FRAME_BEGIN__
	.4byte	.LFB1
	.4byte	.LFE1-.LFB1
	.byte	0x4
	.4byte	.LCFI0-.LFB1
	.byte	0xe
	.byte	0x8
	.byte	0x85
	.byte	0x2
	.byte	0x4
	.4byte	.LCFI1-.LCFI0
	.byte	0xd
	.byte	0x5
	.byte	0x4
	.4byte	.LCFI3-.LCFI1
	.byte	0x2e
	.byte	0x10
	.align 4
.LEFDE1:
	.set	.LLFDE1,.LEFDE1-.LSFDE1
	.ident	"GCC: (GNU) 2.95.4 20011002 (Debian prerelease)"

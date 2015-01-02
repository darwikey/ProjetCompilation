	.file	"test.c"
	.section	.rodata
.LC0:
	.string	"%f = %f + %f + %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	leal	4(%esp), %ecx
	.cfi_def_cfa 1, 0
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	.cfi_escape 0x10,0x5,0x2,0x75,0
	movl	%esp, %ebp
	pushl	%ecx
	.cfi_escape 0xf,0x3,0x75,0x7c,0x6
	subl	$12324, %esp
	movl	$0, -12(%ebp)
	jmp	.L2
.L3:
	fildl	-12(%ebp)
	fstps	-12316(%ebp)
	movl	-12316(%ebp), %edx
	movl	-12(%ebp), %eax
	movl	%edx, -8204(%ebp,%eax,4)
	movl	-12(%ebp), %eax
	movl	-8204(%ebp,%eax,4), %eax
	movl	-12(%ebp), %edx
	movl	%eax, -4108(%ebp,%edx,4)
	movl	-12(%ebp), %eax
	flds	-4108(%ebp,%eax,4)
	movl	-12(%ebp), %eax
	flds	-8204(%ebp,%eax,4)
	faddp	%st, %st(1)
	fildl	-12(%ebp)
	fstps	-12316(%ebp)
	flds	-12316(%ebp)
	faddp	%st, %st(1)
	fstps	-12316(%ebp)
	movl	-12316(%ebp), %edx
	movl	-12(%ebp), %eax
	movl	%edx, -12300(%ebp,%eax,4)
	movl	-12(%ebp), %eax
	flds	-8204(%ebp,%eax,4)
	movl	-12(%ebp), %eax
	flds	-4108(%ebp,%eax,4)
	movl	-12(%ebp), %eax
	flds	-12300(%ebp,%eax,4)
	fxch	%st(2)
	pushl	-12(%ebp)
	leal	-8(%esp), %esp
	fstpl	(%esp)
	leal	-8(%esp), %esp
	fstpl	(%esp)
	leal	-8(%esp), %esp
	fstpl	(%esp)
	pushl	$.LC0
	call	printf
	addl	$32, %esp
	addl	$1, -12(%ebp)
.L2:
	cmpl	$1023, -12(%ebp)
	jle	.L3
	movl	$0, %eax
	movl	-4(%ebp), %ecx
	.cfi_def_cfa 1, 0
	leave
	.cfi_restore 5
	leal	-4(%ecx), %esp
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.9.2 20141101 (Red Hat 4.9.2-1)"
	.section	.note.GNU-stack,"",@progbits

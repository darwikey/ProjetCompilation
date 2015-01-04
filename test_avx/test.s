.text

LC0:
	.string "%f\n"
	.globl  main

LC1:
	.float 1.2

main:
	pushl %ebp
	movl  %esp, %ebp
	subl  $1024, %esp
	fld   LC1
	fstps -8(%ebp)
	movl  $0, -4(%ebp)
	jmp cond

loop:	
	vmovss -8(%ebp), %xmm0
	vmovss -8(%ebp), %xmm1
	vaddps  %xmm0, %xmm1, %xmm2
	vmovss %xmm2, -8(%ebp)

	flds -8(%ebp)
	fstpl (%esp)
	pushl $LC0
	call printf

	addl $1, -4(%ebp)
	
cond:
	cmpl $20, -4(%ebp)
	jle  loop
	movl $0, %eax
	leave
	ret

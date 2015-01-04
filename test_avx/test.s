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
	vmovups -8(%ebp), %ymm0
	vmovups -8(%ebp), %ymm1
	vaddps  %ymm0, %ymm1, %ymm2
	vmovups %ymm2, -8(%ebp)

	flds -8(%ebp)
	fstpl (%esp)
	pushl $LC0
	call printf

	addl $1, -4(%ebp)

cond:
	cmpl $1023, -4(%ebp)
	jle  loop
	movl $0, %eax
	leave
	ret

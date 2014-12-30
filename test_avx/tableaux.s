	.text

LC0:
	.string "a[%d] = %d\n"
	.globl  main

main:
	pushl %ebp
	movl  %esp, %ebp
	subl  $12304, %esp
	movl  $0, -4(%ebp)
	jmp   cond
	
loop:
	movl -4(%ebp), %eax
	movl -4(%ebp), %edx
	movl %edx, -12292(%ebp, %eax, 4)
	movl %edx, -8196(%ebp, %eax, 4)
	movl -8196(%ebp, %eax, 4), %eax
	addl %eax, %edx
	movl -4(%ebp), %eax
	movl %edx, -4100(%ebp, %eax, 4)

	subl  $4, %esp
	pushl %edx
	pushl %eax
	pushl $LC0
	call  printf
	
	addl $1, -4(%ebp)

cond:
	cmpl $1023, -4(%ebp)
	jle  loop
	movl $0, %eax
	leave
	ret

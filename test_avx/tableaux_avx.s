	.text

LC0:
	.string "a[%d] = %d\n"
	.globl  main

main:
	pushl %ebp
	movl  %esp, %ebp
	subl  $12304, %esp
	movl  $0, -4(%ebp)
	jmp   init
	
loop_init:
	movl -4(%ebp), %eax
	movl -4(%ebp), %edx
	movl %edx, -12292(%ebp, %eax, 4)
	movl %edx, -8196(%ebp, %eax, 4)	
	addl $1, -4(%ebp)

init:
	cmpl $1023, -4(%ebp)
	jle  loop_init
	movl  $0, -4(%ebp)
	jmp  cond_vec

add_vec:
	movl     -4(%ebp), %eax
	vmovups  -12292(%ebp, %eax, 4), %ymm0
	#vmovups  -8196(%ebp, %eax, 4), %ymm1
	#vaddps   %ymm0, %ymm1, %ymm2
	#vmovups  %ymm2, -4100(%ebp, %eax, 4)

	movl  -4100(%ebp, %eax, 4), %edx
  subl  $4, %esp
	pushl %edx
	pushl %eax
	pushl $LC0
	call  printf
	
	addl     $32, -4(%ebp)

cond_vec:
	cmpl $1023, -4(%ebp)
	jle  add_vec
	movl $0, %eax
	leave
	ret

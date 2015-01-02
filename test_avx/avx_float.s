.text

LC0:
	.string "c[%d] = %f\n"
	.globl  main

LC1:
	.float 1.2

main:
	pushl %ebp
	movl  %esp, %ebp
	subl  $12304, %esp
	movl  $0, -4(%ebp)
	jmp   cond

loop:	

	movl  -4(%ebp), %eax        
	fld   LC1
	fstps -12292(%ebp, %eax, 4)
	movl  -12292(%ebp, %eax, 4), %edx
	movl  %edx, -8196(%ebp, %eax, 4)
	addl $1, -4(%ebp)           

cond:
	cmpl $1023, -4(%ebp)
	jle  loop
	movl $0, -4(%ebp)
	jmp  cond_vec

add_vec:
	movl -4(%ebp), %eax
	vmovups  -12292(%ebp, %eax, 4), %ymm0
	vmovups  -8196(%ebp, %eax, 4), %ymm1
	vaddps   %ymm0, %ymm1, %ymm2
	vmovups  %ymm2, -4100(%ebp, %eax, 4)

	flds  -4100(%ebp, %eax, 4)
	fstpl (%esp)
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

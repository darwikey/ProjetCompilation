.text

LC0:
	.string "a[%d] = %f\n"
	.globl  main

main:
	pushl %ebp
	movl  %esp, %ebp
	subl  $4100, %esp
	movl  $0, -4(%ebp)
	jmp   cond

loop:	

	movl  -4(%ebp), %eax         # Index du tableau
	fildl -4(%ebp)               # Charge dans le float stack %st(0..)
	fstps -4100(%ebp, %eax, 4)   # Sauvegarde du float dans un tableau
	flds  -4100(%ebp, %eax, 4)   # Charge dans le float stack
	fstpl (%esp)                 # Sauvegarde à %esp pour printf 
	pushl %eax                   # Index du tableau pour printf
	push  $LC0                   # Chaine de caractères
	call  printf                 # Appel à printf
	addl $1, -4(%ebp)            # Incrementation de l'index

cond:
	cmpl $1023, -4(%ebp)
	jle  loop
	movl $0, %eax
	leave
	ret

        .text
LC0:
        .ascii "%d  Alex est con\12\0"
.globl main
main:
        pushl   %ebp
        movl    %esp, %ebp
		
        movl    $0, %ecx    #compteur a zero
        
loope:
        pushl   %ecx        #parametres pour printf
        pushl   $LC0
        call    printf

        addl    $4, %esp    #on vire la valeur retournee   
        popl    %ecx        #on recupère notre compteur

        addl    $1, %ecx
        
        pushl   %ecx        #on sauvegarde le compteur
        
        movl    %ecx, %eax
        subl    $10, %eax
	jnz loope           #on s'arrete de compter lorsque notre compteur - 10 == 0
		
        leave
        ret

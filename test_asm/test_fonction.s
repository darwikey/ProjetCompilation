        .text
STR_PRINT:
        .ascii "mon resultat : %d\12\0"

# ***Convention cdecl***
# passage de parametre de DROITE a GAUCHE par la pile
# netoyage de la pile par la fonction appelante
# c'est a dire que on empile les arguments avant "call"
# puis qu'on depile avec add %esp, LaTailleDeMesArgumentEnOctet

.globl _f
_f:                                 #fonction multipliant les 3 parametres
        pushl   %ebp
        movl    %esp, %ebp          # ebp pointe sur l adresse de retour

        pushl   $4
        
        movl    8(%ebp), %eax       # arg 1
        imul    12(%ebp), %eax      # arg 2
        imul    16(%ebp), %eax      # arg 3
        imul    -4(%ebp), %eax
        
        leave
        ret
       
.globl _printint       
_printint:
        pushl   %ebp
        movl    %esp, %ebp        

        pushl   8(%ebp)       # arg 
        pushl   $STR_PRINT
        call    _printf
        addl    $8, %esp
        
        leave
        ret

		
        
.globl _main
_main:
        pushl   %ebp
        movl    %esp, %ebp
		
        pushl   $5              # arg 3
        pushl   $11             # arg 2
        pushl   $2              # arg 1
        call    _f
        addl    $12, %esp       # on vire les parametres de la pile
        
        
        pushl   %eax
        call    _printint       #affiche le resultat de f
        addl    $4, %esp
        
        leave
        ret

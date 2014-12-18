        .text
STR0:
        .ascii "Alex est con\12\0"
STR1:
        .ascii "Xavier est con\12\0"
STR_PRINT:
        .ascii "%d\12\0"

.globl _main
_main:
        pushl   %ebp
        movl    %esp, %ebp
		
        pushl	$10 #       a
        pushl	$5  #       b
		
        movl    $0, %eax
        movl    (%esp), %ecx
        cmp     4(%esp), %ecx
        
        jg      end_expr      # a >= b
        movl    $1, %eax
end_expr:
        
        pushl   %eax
        call    _printint   
        addl    $4, %esp
		
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
        
.text
main:
    lui a0, 0b1010
    lui a1, 2
    
    sll a2, a0, a1  
    srl a3, a0, a1  
    sra a4, a0, a1  
    
    lui a0, -8
    sra a5, a0, a1  
    
    lui a7, 10
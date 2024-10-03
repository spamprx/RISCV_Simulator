.text
    lui t0, 0
    addi t0, t0, 1
    lui t1, 0
    addi t1, t1, 31
    lui t2, 0
    addi t2, t2, 32
    lui t3, 0xFFFFF
    addi t3, t3, 0xFFF
    
    sll a0, t0, t1
    
    sll a1, t0, t2
    
    sra a2, t3, t1
    srl a3, t3, t1
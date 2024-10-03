.text
    lui t0, 0xFFFFF
    addi t0, t0, 0xFFF
    lw a0, 0(t0)
    
    lui t1, 0x10
    sw zero, 0(t1)
    
    lui t2, 0x1
    addi t2, t2, 1
    lw a1, 0(t2)
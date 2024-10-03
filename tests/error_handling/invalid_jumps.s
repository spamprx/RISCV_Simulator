.text
    lui t0, 0xFFFFF
    addi t0, t0, 0xFFF
    jalr ra, 0(t0)
    
    lui t1, 0x1
    addi t1, t1, 1
    jalr ra, 0(t1)
    
    lui ra, 0xFFFFF
    addi ra, ra, 0xFFF
    jalr x0,0(x1)
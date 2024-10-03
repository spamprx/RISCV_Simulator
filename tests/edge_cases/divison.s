.data
    .word 0x7FFFFFFF
    .word 0x80000000

.text
    lui t0, 0x10
    lw t1, 0(t0)
    lw t2, 4(t0)
    
    lui t3, 0
    div a0, t1, t3
    
    lui t4, 0xFFFFF
    addi t4, t4, 0xFFF
    div a1, t2, t4
    
    rem a2, t1, t2
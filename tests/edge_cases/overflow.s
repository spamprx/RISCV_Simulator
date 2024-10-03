.data
    .word 0x7FFFFFFF
    .word 0x80000000

.text
    lui t0, 0x10
    lw t1, 0(t0)
    lw t2, 4(t0)
    
    addi a0, t1, 1
    
    addi a1, t2, -1
    
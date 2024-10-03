.data
    .word 64, 34, 25, 12, 22, 11, 90
    .word 7

.text
    lui t0, 0x10
    lw t1, 28(t0)
    addi t1, t1, -1
    
outer_loop:
    lui t2, 0
    mv t3, t0
    
inner_loop:
    lw t4, 0(t3)
    lw t5, 4(t3)
    ble t4, t5, no_swap
    
    sw t5, 0(t3)
    sw t4, 4(t3)
    
no_swap:
    addi t3, t3, 4
    addi t2, t2, 1
    blt t2, t1, inner_loop
    
    addi t1, t1, -1
    bnez t1, outer_loop
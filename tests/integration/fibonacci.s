.data
    .dword 40
.text
    lui t0, 0x10
    lui t1, 0
    addi t1, t1, 10
    lui t2, 0
    lui t3, 0
    addi t3, t3, 1
    
    sw t2, 0(t0)
    sw t3, 4(t0)
    addi t0, t0, 8
    addi t1, t1, -2
    
fib_loop:
    add t4, t2, t3
    sw t4, 0(t0)
    mv t2, t3
    mv t3, t4
    addi t0, t0, 4
    addi t1, t1, -1
    bnez t1, fib_loop
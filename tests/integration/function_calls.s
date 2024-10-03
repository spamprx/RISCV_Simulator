.data
    .word 0

.text
    lui a0, 0
    addi a0, a0, 5
    lui a1, 0
    addi a1, a1, 7
    jal ra, add_and_multiply
    
    lui t0, 0x10
    sw a0, 0(t0)

add_and_multiply:
    add t0, a0, a1
    jalr x0,0(x1)
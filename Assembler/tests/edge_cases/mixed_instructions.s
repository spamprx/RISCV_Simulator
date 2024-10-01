; File: tests/edge_cases/mixed_instructions.s
    lui x1, 0x12345
    addi x1, x1, 0x678
    sw x1, 12(sp)
    lw x2, 12(sp)
    beq x1, x2, equal
    sub x3, x1, x2
equal:
    add x4, x1, x2
; File: tests/integration/branch_and_jump.s
    addi x1, x0, 5
    addi x2, x0, 10
loop:
    beq x1, x2, end
    addi x1, x1, 1
    j loop
end:
    add x3, x1, x2
addi x2, x0, 5
addi x3, x0, 10
add x1, x2, x3
sw x1, 4(x0)
lw x2, 0(x0)
jalr x0, 0(x0)
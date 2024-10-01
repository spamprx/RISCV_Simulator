; File: tests/integration/load_store.s
    addi x1, x0, 42
    sw x1, 0(x2)
    lw x3, 0(x2)
    addi x4, x3, 10
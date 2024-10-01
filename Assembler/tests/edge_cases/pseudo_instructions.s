; File: tests/edge_cases/pseudo_instructions.s
    li x1, 0x12345678    ; Large immediate load
    mv x2, x1            ; Move pseudo-instruction
    j label              ; Jump pseudo-instruction
label:
    nop
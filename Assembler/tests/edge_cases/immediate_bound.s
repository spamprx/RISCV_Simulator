; File: tests/edge_cases/immediate_bounds.s
addi x1, x0, 2047    ; Maximum positive 12-bit immediate
addi x2, x0, -2048   ; Minimum negative 12-bit immediate
lui x3, 0xFFFFF      ; Maximum 20-bit immediate for lui
.text
    .word 0xFFFFFFFF
    
    j data_section

.data
    .word 0x12345678

.text
data_section:
    lw t0, 0x10000
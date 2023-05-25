.globl rdps2
.align 4

rdps2:
    lui a0, 0x20
    lw  a0, 0x00(a0)
    ret

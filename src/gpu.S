.globl ccsrdpx
.globl ccsrdln
.globl ccsrdrct
.globl ccsrdspr
.align 4

# csr register bits:
# csr[0] = ready
# csr[1] = start
# csr[2] = -
# csr[3] = -
# csr[4] = draw pixel
# csr[5] = draw line
# csr[6] = draw rectangle
# csr[7] = draw sprite

# check csr ready bit and draw pixel
# int ccsrdpx(uint32_t x, uint32_t y, uint32_t color)
# a0 = x
# a1 = y
# a2 = color
ccsrdpx:
    lui     a3, 0x40000
    lw      a4, 0(a3)
    and     a4, a4, 0b1         # check ready bit
    bnez    a4, dpx             # if ready, draw pixel
    li      a0, 1               # else return 1
    ret
dpx:
    sll     a1, a1, 11          # px_addr = y << 11 | x
    or      a1, a1, a0
    sw      a0, 0x4(a3)         # write px_addr
    sw      a2, 0x8(a3)         # write color
    or      a4, a4, 0b00010011  # set command
    sw      a4, 0(a3)           # write csr
    li      a0, 0
    ret

# check csr ready bit and draw line
# int ccsrdln(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color)
# a0 = x0
# a1 = y0
# a2 = x1
# a3 = y1
# a4 = color
ccsrdln:
    lui     a5, 0x40000
    lw      a6, 0(a5)
    and     a6, a6, 0b1         # check ready bit
    bnez    a6, dln             # if ready, draw line
    li      a0, 1               # else return 1
    ret
dln:
    sll     a1, a1, 11          # px_addr = y0 << 11 | x0
    or      a1, a1, a0
    sw      a1, 0x4(a5)         # write px_addr0
    sll     a3, a3, 11          # px_addr = y1 << 11 | x1
    or      a3, a3, a2
    sw      a3, 0x8(a5)         # write px_addr1
    sw      a4, 0x1c(a5)        # write color
    or      a4, a4, 0b00100011  # set command
    sw      a4, 0(a3)           # write csr
    li      a0, 0
    ret

# check csr ready bit and draw rectangle
# int ccsrdrct(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color)
# a0 = x0
# a1 = y0
# a2 = x1
# a3 = y1
# a4 = color
ccsrdrct:
    ret
drct:
    ret

# check csr ready bit and draw sprite
# int ccsrdsp(___)
ccsrdspr:
    ret
dspr:
    ret
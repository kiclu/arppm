.globl gpu_init
.globl ccsrdpx
.globl ccsrdlnoct0
.globl ccsrdlnoct1
.globl ccsrdlnoct2
.globl ccsrdlnoct3
.globl ccsrdrct
.globl ccsrdspr
.align 4

# csr register bits:
# csr[0] = ready
# csr[1] = draw pixel
# csr[2] = draw line (octant 0)
# csr[3] = draw line (octant 1)
# csr[4] = draw line (octant 2)
# csr[5] = draw line (octant 3)
# csr[6] = draw rectangle
# csr[7] = draw sprite

gpu_init:
    lui     a1, 0x10
    li      a0, 0x01
    sw      a0, 0x0(a1)
    lw      a0, 0x0(a1)
    ret

# check csr ready bit and draw pixel
# int ccsrdpx(uint32_t x, uint32_t y, uint32_t color)
# a0 = x
# a1 = y
# a2 = color
ccsrdpx:
    lui     a3, 0x10
    lw      a4, 0x0(a3)         # read csr
    and     a4, a4, 0b1         # check ready bit
    bnez    a4, dpx             # if ready then draw pixel
    li      a0, 1               # else return 1
    ret
dpx:
    sll     a1, a1, 11          # px_addr = y << 11 | x
    or      a1, a1, a0
    sw      a1, 0x4(a3)         # write px_addr
    sw      a2, 0xC(a3)         # write color
    or      a4, a4, 0x02        # set command
    sw      a4, 0x0(a3)         # write csr
    li      a0, 0
    ret

# check csr ready bit and draw line (octant 0)
# int ccsrdlnoct0(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color)
# a0 = x0
# a1 = y0
# a2 = x1
# a3 = y1
# a4 = color
ccsrdlnoct0:
    lui     a5, 0x10
    lw      a6, 0x0(a5)         # read csr
    and     a6, a6, 0b1         # check ready bit
    bnez    a6, dlnoct0         # if ready then draw line
    li      a0, 1               # else return 1
    ret
dlnoct0:
    sll     a1, a1, 11          # px_addr0 = y0 << 11 | x0
    or      a1, a1, a0
    sw      a1, 0x4(a5)         # write px_addr0
    sll     a3, a3, 11          # px_addr1 = y1 << 11 | x1
    or      a3, a3, a2
    sw      a3, 0x8(a5)         # write px_addr1
    sw      a4, 0xC(a5)         # write color
    or      a6, a6, 0x04        # set command
    sw      a6, 0x0(a5)         # write csr
    li      a0, 0
    ret

# check csr ready bit and draw line (octant 1)
# int ccsrdlnonct1(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color)
# a0 = x0
# a1 = y0
# a2 = x1
# a3 = y1
# a4 = color
ccsrdlnoct1:
    lui     a5, 0x10
    lw      a6, 0x0(a5)         # read csr
    and     a6, a6, 0b1         # check ready bit
    bnez    a6, dlnoct1         # if ready then draw line
    li      a0, 1               # else return 1
    ret
dlnoct1:
    sll     a1, a1, 11          # px_addr0 = y0 << 11 | x0
    or      a1, a1, a0
    sw      a1, 0x4(a5)         # write px_addr0
    sll     a3, a3, 11          # px_addr1 = y1 << 11 | x1
    or      a3, a3, a2
    sw      a3, 0x8(a5)         # write px_addr1
    sw      a4, 0xC(a5)         # write color
    or      a6, a6, 0x08        # set command
    sw      a6, 0x0(a5)         # write csr
    li      a0, 0
    ret

# check csr ready bit and draw line (octant 2)
# int ccsrdlnoct2(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color)
# a0 = x0
# a1 = y0
# a2 = x1
# a3 = y1
# a4 = color
ccsrdlnoct2:
    lui     a5, 0x10
    lw      a6, 0x0(a5)         # read csr
    and     a6, a6, 0b1         # check ready bit
    bnez    a6, dlnoct2         # if ready then draw line
    li      a0, 1               # else return 1
    ret
dlnoct2:
    sll     a1, a1, 11          # px_addr0 = y0 << 11 | x0
    or      a1, a1, a0
    sw      a1, 0x4(a5)         # write px_addr0
    sll     a3, a3, 11          # px_addr1 = y1 << 11 | x1
    or      a3, a3, a2
    sw      a3, 0x8(a5)         # write px_addr1
    sw      a4, 0xC(a5)         # write color
    or      a6, a6, 0x10        # set command
    sw      a6, 0x0(a5)         # write csr
    li      a0, 0
    ret

# check csr ready bit and draw line (octant 3)
# int ccsrdlnoct3(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color)
# a0 = x0
# a1 = y0
# a2 = x1
# a3 = y1
# a4 = color
ccsrdlnoct3:
    lui     a5, 0x10
    lw      a6, 0x0(a5)         # read csr
    and     a6, a6, 0b1         # check ready bit
    bnez    a6, dlnoct3         # if ready then draw line
    li      a0, 1               # else return 1
    ret
dlnoct3:
    sll     a1, a1, 11          # px_addr0 = y0 << 11 | x0
    or      a1, a1, a0
    sw      a1, 0x4(a5)         # write px_addr0
    sll     a3, a3, 11          # px_addr1 = y1 << 11 | x1
    or      a3, a3, a2
    sw      a3, 0x8(a5)         # write px_addr1
    sw      a4, 0xC(a5)         # write color
    or      a6, a6, 0x20        # set command
    sw      a6, 0x0(a5)         # write csr
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
    lui     a5, 0x10
    lw      a6, 0x0(a5)         # read csr
    and     a6, a6, 0b1         # check ready bit
    bnez    a6, drct            # if ready then draw rectangle
    li      a0, 1               # else return 1
    ret
drct:
    sll     a1, a1, 11          # px_addr0 = y0 << 11 | x0
    or      a1, a1, a0
    sw      a1, 0x4(a5)         # write px_addr0
    sll     a3, a3, 11          # px_addr1 = y1 << 11 | x1
    or      a3, a3, a2
    sw      a3, 0x8(a5)         # write px_addr1
    sw      a4, 0xC(a5)         # write color
    or      a6, a6, 0x40        # set command
    sw      a6, 0x0(a5)         # write csr
    ret

# check csr ready bit and draw sprite
# int ccsrdsp(uint32_t x, uint32_t y, uint32_t color, sprite_t* sprite)
# a0 = x
# a1 = y
# a2 = color
# a3 = &sprite
ccsrdspr:
    lui     a4, 0x10
    lw      a5, 0x0(a4)         # read csr
    and     a5, a5, 0b1         # check ready bit
    bnez    a5, dspr            # if ready then draw sprite
    li      a0, 1               # else return 1
    ret
dspr:
    sll     a1, a1, 11          # px_addr = y0 << 1 | x0
    or      a1, a1, a0
    sw      a1, 0x4(a4)         # write px_addr
    sw      a2, 0xC(a4)         # write color
    
    lw      a6, 0x00(a3)        # read sprite[0]
    sw      a6, 0x10(a4)        # write sprite[0]

    lw      a6, 0x04(a3)        # read sprite[1]
    sw      a6, 0x14(a4)        # write sprite[1]

    lw      a6, 0x08(a3)        # read sprite[2]
    sw      a6, 0x18(a4)        # write sprite[2]

    lw      a6, 0x0C(a3)        # read sprite[3]
    sw      a6, 0x1C(a4)        # write sprite[3]

    lw      a6, 0x10(a3)        # read sprite[4]
    sw      a6, 0x20(a4)        # write sprite[4]

    lw      a6, 0x14(a3)        # read sprite[5]
    sw      a6, 0x24(a4)        # write sprite[5]

    lw      a6, 0x18(a3)        # read sprite[6]
    sw      a6, 0x28(a4)        # write sprite[6]

    lw      a6, 0x1C(a3)        # read sprite[7]
    sw      a6, 0x2C(a4)        # write sprite[7]
    
    or      a5, a5, 0x80        # set command
    sw      a5, 0x0(a4)         # write csr
    ret

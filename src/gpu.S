.globl gpu_init
.globl ccsrdpx
.globl ccsrdln
.globl ccsrdrct
.globl ccsrdspr
.globl hdaenable
.globl hdadisable
.align 4

# gpu memory mapped register addresses:
# csr       = 0x10000
# coord0    = 0x10004
# coord1    = 0x10008
# color     = 0x1000C
# spr0      = 0x10010
# spr1      = 0x10014
# spr2      = 0x10018
# spr3      = 0x1001C
# spr4      = 0x10020
# spr5      = 0x10024
# spr6      = 0x10028
# spr7      = 0x1002C

# csr register bits:
# csr[0] = ready
# csr[1] = hdae
# csr[2] = /
# csr[3] = /
# csr[4] = dpx
# csr[5] = dln
# csr[6] = drct
# csr[7] = dspr

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
    li      a4, 0x10            # set command
    sw      a4, 0x0(a3)         # write csr
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
    lui     a5, 0x10
    lw      a6, 0x0(a5)         # read csr
    and     a6, a6, 0b1         # check ready bit
    bnez    a6, dln             # if ready then draw line
    li      a0, 1               # else return 1
    ret
dln:
    sll     a1, a1, 11          # px_addr0 = y0 << 11 | x0
    or      a1, a1, a0
    sw      a1, 0x4(a5)         # write px_addr0
    sll     a3, a3, 11          # px_addr1 = y1 << 11 | x1
    or      a3, a3, a2
    sw      a3, 0x8(a5)         # write px_addr1
    sw      a4, 0xC(a5)         # write color
    li      a6, 0x20            # set command
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
    li      a6, 0x40            # set command
    sw      a6, 0x0(a5)         # write csr
    li      a0, 0
    ret

# check csr ready bit and draw sprite
# int ccsrdspr(uint32_t x, uint32_t y, uint32_t color, sprite_t* sprite)
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
    
    li      a5, 0x80            # set command
    sw      a5, 0x0(a4)         # write csr
    li      a0, 0
    ret

# enable horizontal draw area
# void hdaenable()
hdaenable:
    li      a0, 0x10
    lw      a1, 0x0(a0)         # read csr
    or      a1, a1, 0b10        # set hdae bit to 1
    sw      a1, 0x0(a0)         # write csr
    ret

# disable horizontal draw area
# void hdadisable()
hdadisable:
    li      a0, 0x10
    lw      a1, 0x0(a0)         # read csr
    xor     a1, a1, 0b10        # set hdae bit to 0
    sw      a1, 0x0(a0)         # write csr
    ret

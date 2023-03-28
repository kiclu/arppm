#ifndef _ARP_GPU_H
#define _ARP_GPU_H

#define GPU_SCREEN_WIDTH    800
#define GPU_SCREEN_HEIGHT   600

#define GPU_CSR_ADDR        0x10000

#define GPU_COORD0_ADDR     0x10004
#define GPU_COORD1_ADDR     0x10008

#define GPU_COLOR_ADDR      0x1000C

#define GPU_SPR0_ADDR       0x10010
#define GPU_SPR1_ADDR       0x10014
#define GPU_SPR2_ADDR       0x10018
#define GPU_SPR3_ADDR       0x1001C
#define GPU_SPR4_ADDR       0x10020
#define GPU_SPR5_ADDR       0x10024
#define GPU_SPR6_ADDR       0x10028
#define GPU_SPR7_ADDR       0x1002C

#include<stdint.h>
#include<sprite.h>

// check csr ready bit and draw pixel
extern int ccsrdpx(uint32_t x, uint32_t y, uint16_t color);

// check csr ready bit and draw line (octant 0)
extern int ccsrdlnoct0(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color);

// check csr ready bit and draw line (octant 1)
extern int ccsrdlnoct1(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color);

// check csr ready bit and draw line (octant 2)
extern int ccsrdlnoct2(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color);

// check csr ready bit and draw line (octant 3)
extern int ccsrdlnoct3(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color);

// check csr ready bit draw rectangle
extern int ccsrdrct(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color);

// check csr ready bit and draw sprite
extern int ccsrdspr(uint32_t x, uint32_t y, uint16_t color, const sprite_t* sprite);

// wrapper functions

// check csr ready bit and draw line
int ccsrdln(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color);

// check csr ready bit and draw horizontal line
int ccsrdhln(uint32_t x0, uint32_t x1, uint32_t y, uint16_t color);

// check csr ready bit and draw vertical line
int ccsrdvln(uint32_t x, uint32_t y0, uint32_t y1, uint16_t color);


#endif//_ARP_GPU_H

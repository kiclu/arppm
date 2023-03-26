#ifndef _ARP_GPU_H
#define _ARP_GPU_H

#define GPU_SCREEN_WIDTH    800
#define GPU_SCREEN_HEIGHT   600

#define GPU_CSR_ADDR        0xF000
#define GPU_COORD0_ADDR     0xF004
#define GPU_COORD1_ADDR     0xF008
#define GPU_COLOR_ADDR      0xF00C
#define GPU_SPRITE0_ADDR    0xF010
#define GPU_SPRITE1_ADDR    0xF014
#define GPU_SPRITE2_ADDR    0xF018
#define GPU_SPRITE3_ADDR    0xF01C
#define GPU_SPRITE4_ADDR    0xF020
#define GPU_SPRITE5_ADDR    0xF024
#define GPU_SPRITE6_ADDR    0xF028
#define GPU_SPRITE7_ADDR    0xF02C

#include<stdint.h>

typedef struct {
    uint32_t data[8];
} sprite_t;

// check csr ready bit and draw pixel
extern int ccsrdpx(uint32_t x, uint32_t y, uint16_t color);

// check csr ready bit and draw line
extern int ccsrdln(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color);

// check csr ready bit draw rectangle
extern int ccsrdrct(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color);

// check csr ready bit and draw sprite
extern int ccsrdspr(uint32_t x, uint32_t y, uint16_t color, const sprite_t* sprite);

extern void gpu_init();

#endif//_ARP_GPU_H
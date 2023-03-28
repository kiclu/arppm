#include<gpu.h>

static void swap(uint32_t* a, uint32_t* b){
    uint32_t t = *a;
    *a = *b;
    *b = t;
}

int ccsrdln(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color){
    return 1; // TODO
}

int ccsrdhln(uint32_t x0, uint32_t x1, uint32_t y, uint16_t color){
    if(x0 > x1) swap(&x0, &x1);
    return ccsrdlnoct0(x0, y, x1, y, color);
}

int ccsrdvln(uint32_t x, uint32_t y0, uint32_t y1, uint16_t color){
    return 1; // TODO
}

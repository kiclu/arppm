#include<gpu.h>

static void swap(uint32_t* a, uint32_t* b){
    uint32_t t = *a;
    *a = *b;
    *b = t;
}

int ccsrdln(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint16_t color){
    if(y0 > y1){
        swap(&x0, &x1);
        swap(&y0, &y1);
    }

    register int32_t dx = x1 - x0;
    register int32_t dy = y1 - y0;
    register int32_t absdx = dx > 0 ? dx : -dx;

    if(absdx >= dy){    // low
        if(dx > 0) return ccsrdlnoct0(x0, y0, x1, y1, color);
        else return ccsrdlnoct3(x1, y1, x0, y0, color);
    }
    else{               // high
        if(dx > 0) return ccsrdlnoct1(x0, y0, x1, y1, color);
        else return ccsrdlnoct2(x1, y1, x0, y0, color);
    }
}

int ccsrdhln(uint32_t x0, uint32_t x1, uint32_t y, uint16_t color){
    if(x0 > x1) swap(&x0, &x1);
    return ccsrdlnoct0(x0, y, x1, y, color);
}

int ccsrdvln(uint32_t x, uint32_t y0, uint32_t y1, uint16_t color){
    if(y0 > y1) swap(&y0, &y1);
    return ccsrdlnoct1(x, y0, x, y1, color);
}

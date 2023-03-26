#include<stdint.h>

#include"../h/gpu.h"
#include"../h/riscv.h"

const sprite_t pacman[] = {
    {
        0x07C00000, 0x3FF81FF0,
        0x0FFC3FF8, 0x003C01FC,
        0x0FFC01FC, 0x3FF83FF8,
        0x07C01FF0, 0x00000000
    }
};

[[ noreturn ]] void main(){
    gpu_init();
    while(ccsrdspr(0, 0, 0x0FF, &pacman[0]));
    for(;;);
}

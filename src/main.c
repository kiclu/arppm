#include<gpu.h>
#include<riscv.h>

extern sprite_t pacman[4][2];
extern sprite_t ghost[4][2];

[[ noreturn ]] void main(){
    ccsrdspr(0, 0, 0x0FF, &pacman[RIGHT][1]);
}

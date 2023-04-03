#include<gpu.h>
#include<riscv.h>
#include<dbgif.h>
#include<pm.h>

extern sprite_t pacman[4][2];
extern sprite_t ghost[4][2];

[[ noreturn ]] void main(){
    // dbgif_print_hex(0xFFFF, 0);
    run();

    for(;;);
}

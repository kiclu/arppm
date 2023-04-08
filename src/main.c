#include<gpu.h>
#include<riscv.h>
#include<dbgif.h>
#include<pm.h>

[[ noreturn ]] void main(){
    run();

    for(;;);
}

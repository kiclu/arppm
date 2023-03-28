#include<riscv.h>

void __attribute__((optimize("O0"))) wait_ms(uint32_t ms){
    for(register uint32_t i = 0; i < (ms << 10) + (ms << 9); i++){
        asm volatile("nop");
    }
}

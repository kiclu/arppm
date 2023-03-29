#include<dbgif.h>

void dbgif_print_hex(uint32_t val, uint8_t reg){
    uint32_t data = 0;
    for(uint32_t i = 0; i < 4; i++){
        switch(val & 0xF){
            case 0x0: data |= 0x3F; break;
            case 0x1: data |= 0x06; break;
            case 0x2: data |= 0x5B; break;
            case 0x3: data |= 0x4F; break;
            case 0x4: data |= 0x66; break;
            case 0x5: data |= 0x6D; break;
            case 0x6: data |= 0x7D; break;
            case 0x7: data |= 0x07; break;
            case 0x8: data |= 0x7F; break;
            case 0x9: data |= 0x6F; break;
            case 0xA: data |= 0x77; break;
            case 0xB: data |= 0x7C; break;
            case 0xC: data |= 0x39; break;
            case 0xD: data |= 0x5E; break;
            case 0xE: data |= 0x79; break;
            case 0xF: data |= 0x71; break;
        }
        if(i != 3) data <<= 8;
        val >>= 4;
    }
    *(uint32_t*)DBGIF_REG0_ADDR = data;
}

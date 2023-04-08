#ifndef _ARP_DBGIF_H
#define _ARP_DBGIF_H

#define DBGIF_REG0_ADDR     0x30000

#include<stdint.h>

void dbgif_print_hex(uint32_t val, uint8_t reg);

#endif//_ARP_7SEG_H

#ifndef _ARP_SPRITE_H
#define _ARP_SPRITE_H

#include<stdint.h>

enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

typedef struct {
    uint32_t data[8];
} sprite_t;

#endif//_ARP_SPRITE_H

#ifndef _ARP_PM_H
#define _ARP_PM_H

#include<stdint.h>

#define pacman_color 0xFF0

typedef struct {
    uint32_t pos_x;
    uint32_t pos_y;
    uint32_t dir;
    uint32_t newdir;
    uint32_t fr;
    uint32_t stuck;
} pacman_t;

typedef struct {
    uint32_t pos_x;
    uint32_t pos_y;
    uint32_t dir;
    uint32_t newdir;
    uint32_t fr;
    uint32_t color;
    uint32_t stuck;
    uint32_t is_released;
} ghost_t;

void run();

#endif//_ARP_PM_H

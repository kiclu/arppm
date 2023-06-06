#ifndef _ARP_MAP_H
#define _ARP_MAP_H

#include<stdint.h>

#define background_color 0x001

void clear_screen();
void draw_map();

#define map_line_count 265
#define map_line_color 0x11C

typedef struct{
    uint32_t x0;
    uint32_t y0;
    uint32_t x1;
    uint32_t y1;
} line_t;

void draw_map_lines();
void draw_door();

#define map_rect_count 19
#define map_rect_color 0x11C

typedef struct{
    uint32_t x0;
    uint32_t y0;
    uint32_t x1;
    uint32_t y1;
} rect_t;

void draw_map_rects();

#define map_dot_count 693
#define map_dot_color 0xFFF

typedef struct{
    uint32_t x;
    uint32_t y;
} dot_t;

uint32_t check_dot_eaten(uint32_t idx);
void set_dot_eaten(uint32_t idx);
void clear_dot_eaten();

void draw_map_dots();

#endif//_ARP_MAP_H

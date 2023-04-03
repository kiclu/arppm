#include<pm.h>
#include<gpu.h>
#include<sprite.h>
#include<dbgif.h>
#include<riscv.h>

#include<stdint.h>

const uint32_t background_color = 0x000;

static void clear_screen(){
    while(ccsrdrct(0, 0, GPU_SCREEN_WIDTH, GPU_SCREEN_HEIGHT, background_color));
}

typedef struct {
    uint32_t x0;
    uint32_t y0;
    uint32_t x1;
    uint32_t y1;
} line_t;

const uint32_t map_line_count = 90;
const uint32_t map_line_color = 0x00E;
const line_t map_line[] = {
    // upper border [9]
    {15, 291, 79, 291},
    {79, 259, 79, 291},
    {15, 259, 79, 259},
    {15, 19, 15, 259},
    {15, 19, 783, 19},
    {783, 19, 783, 259},
    {719, 259, 783, 259},
    {719, 259, 719, 291},
    {719, 291, 783, 291},

    // lower border [9]
    {15, 307, 79, 307},
    {79, 307, 79, 339},
    {15, 339, 79, 339},
    {15, 339, 15, 579},
    {15, 579, 783, 579},
    {783, 339, 783, 579},
    {719, 339, 783, 339},
    {719, 307, 719, 339},
    {719, 307, 783, 307},

    // center outline [12]
    {271, 227, 271, 371},
    {271, 371, 527, 371},
    {527, 227, 527, 371},
    {271, 227, 527, 227},

    {319, 387, 319, 403},
    {463, 387, 463, 403},
    {319, 387, 463, 387},
    {319, 403, 463, 403},

    {367, 195, 367, 211},
    {431, 195, 431, 211},
    {367, 195, 431, 195},
    {367, 211, 431, 211},

    // lower left blocks [30]
    {31, 355, 31, 435},
    {79, 355, 79, 435},
    {31, 355, 79, 355},
    {31, 435, 79, 435},
    
    {31, 451, 31, 483},
    {79, 451, 79, 483},
    {31, 451, 79, 451},
    {31, 483, 79, 483},

    {31, 499, 31, 531},
    {79, 499, 79, 531},
    {31, 499, 79, 499},
    {31, 531, 79, 531},

    {95, 499, 95, 547},
    {31, 547, 31, 563},
    {111, 499, 111, 547},
    {143, 547, 143, 563},
    {95, 499, 111, 499},
    {31, 547, 95, 547},
    {111, 547, 143, 547},
    {31, 563, 143, 563},

    {239, 307, 239, 403},
    {239, 403, 303, 403},
    {303, 387, 303, 403},
    {255, 387, 303, 387},
    {255, 307, 255, 387},
    {239, 307, 255, 307},

    {95, 307, 95, 483},
    {111, 307, 111, 483},
    {95, 307, 111, 307},
    {95, 483, 111, 483},

    // lower right blocks [30]
    {719, 355, 719, 435},
    {719, 435, 767, 435},
    {767, 355, 767, 435},
    {719, 355, 767, 355},

    {719, 451, 719, 483},
    {719, 483, 767, 483},
    {767, 451, 767, 483},
    {719, 451, 767, 451},

    {719, 499, 719, 531},
    {719, 531, 767, 531},
    {767, 499, 767, 531},
    {719, 499, 767, 499},

    {687, 499, 687, 547},
    {703, 499, 703, 547},
    {655, 547, 655, 563},
    {767, 547, 767, 563},
    {687, 499, 703, 499},
    {655, 547, 687, 547},
    {703, 547, 767, 547},
    {655, 563, 767, 563},

    {479, 387, 543, 387},
    {479, 387, 479, 403},
    {479, 403, 559, 403},
    {559, 307, 559, 403},
    {543, 307, 559, 307},
    {543, 307, 543, 387},

    {687, 307, 687, 483},
    {703, 307, 703, 483},
    {687, 307, 703, 307},
    {687, 483, 703, 483}
};

void ccsrdlnl(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color){
    int32_t dx = x1 - x0;
    int32_t dy = y1 - y0;
    int32_t yi = 1;
    if(dy < 0){
        yi = -1;
        dy = -dy;
    }
    int32_t D = 2*dy - dx;
    uint32_t y = y0;

    for(uint32_t x = x0; x <= x1; ++x){
        while(ccsrdpx(x, y, color));
        if(D > 0){
            y += yi;
            D -= 2*dx;
        }
        D += 2*dy;
    }
}

void ccsrdlnh(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color){
    int32_t dx = x1 - x0;
    int32_t dy = y1 - y0;
    int32_t xi = 1;
    if(dx < 0){
        xi = -1;
        dx = -dx;
    }
    int32_t D = 2*dx - dy;
    uint32_t x = x0;

    for(uint32_t y = y0; y <= y1; ++y){
        while(ccsrdpx(x, y, color));
        if(D > 0){
            x += xi;
            D -= 2*dy;
        }
        D += 2*dx;
    }
}

int ccsrdln1(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color){
    int32_t dx = x1 - x0 > 0 ? x1 - x0 : x0 - x1;
    int32_t dy = y1 - y0 > 0 ? y1 - y0 : y0 - y1;
    if(dy < dx){
        if(x0 > x1) ccsrdlnl(x1, y1, x0, y0, color);
        else ccsrdlnl(x0, y0, x1, y1, color);
    }
    else{
        if(y0 > y1) ccsrdlnh(x1, y1, x0, y0, color);
        else ccsrdlnh(x0, y0, x1, y1, color);
    }
    return 0;
}

static void draw_map_lines(){
    for(uint32_t i = 0; i < map_line_count; ++i){
        while(ccsrdln1(
            map_line[i].x0,
            map_line[i].y0,
            map_line[i].x1,
            map_line[i].y1,
            map_line_color
        ));

        while(ccsrdpx(map_line[i].x0, map_line[i].y0, 0x000));
        while(ccsrdpx(map_line[i].x1, map_line[i].y1, 0x000));
    }
}

typedef struct {
    uint32_t x;
    uint32_t y;
} dot_t;

const uint32_t map_dot_count = 2;
const uint32_t map_dot_color = 0xFFF;
const dot_t map_dots[] = {
    {22, 26},
    {38, 26}
};

static void draw_map_dots(){
    for(uint32_t i = 0; i < map_dot_count; ++i){
        while(ccsrdrct(
            map_dots[i].x,
            map_dots[i].y,
            map_dots[i].x + 2,
            map_dots[i].y + 2,
            map_dot_color
        ));
    }
}

extern sprite_t pacman_sprite[4][2];
extern sprite_t ghost_sprite[2];
extern sprite_t eyes_sprite[4];
extern sprite_t pupils_sprite[4];

static void draw_hp_counter(){
    while(ccsrdspr(15, 584, 0xFF0, &pacman_sprite[RIGHT][1]));
    while(ccsrdspr(31, 584, 0xFF0, &pacman_sprite[RIGHT][1]));
    while(ccsrdspr(47, 584, 0xFF0, &pacman_sprite[RIGHT][1]));
}

static void draw_map(){
    draw_map_lines();
    draw_map_dots();

    draw_hp_counter();
}

static void draw_debug_grid(){
    for(uint32_t i = 19; i <= 579; i += 16){
        while(ccsrdhln(15, 783, i, 0x444));
    }
    for(uint32_t j = 15; j <= 783; j += 16){
        while(ccsrdvln(j, 19, 579, 0x444));
    }

    while(ccsrdhln(0, 800, 0, 0xF00));
    while(ccsrdhln(0, 800, 600, 0xF00));

    while(ccsrdvln(0, 0, 600, 0xF00));
    while(ccsrdvln(800, 0, 600, 0xF00));
}

typedef struct {
    uint32_t pos_x;
    uint32_t pos_y;
    uint32_t dir;
    uint32_t fr;
} pacman_t;

typedef struct {
    uint32_t pos_x;
    uint32_t pos_y;
    uint32_t dir;
    uint32_t fr;
    uint32_t color;
} ghost_t;

// int ccsrdspr1(uint32_t x, uint32_t y, uint32_t color, const sprite_t* sprite){
//     sprite_t tmp = *sprite;
//     for(uint32_t i = 0; i < 8; ++i){
//         for(uint32_t j = 0; j < 2; ++j){
//             for(uint32_t k = 0; k < 16; ++k){
//                 if(tmp.data[i] & 0b1){
//                     while(ccsrdpx(x + k, y + (i << 1) + j, color));
//                 }
//                 tmp.data[i] >>= 1;
//             }
//         }
//     }
//     return 0;
// }

static void clear_ghost(const ghost_t* ghost){

}

static void draw_ghost(const ghost_t* ghost){
    while(ccsrdspr(
        ghost->pos_x, ghost->pos_y, ghost->color, &ghost_sprite[ghost->fr]
    ));
    while(ccsrdspr(
        ghost->pos_x, ghost->pos_y, 0xFFF, &eyes_sprite[ghost->dir]
    ));
    while(ccsrdspr(
        ghost->pos_x, ghost->pos_y, 0x00F, &pupils_sprite[ghost->dir]
    ));
}

static void draw_debug_sprites(){
    for(uint32_t i = 0; i < 4; ++i){
        for(uint32_t j = 0; j < 2; ++j){
            while(ccsrdspr(
                (j + 1) * 16,
                36 + i * 16,
                0xFF0,
                &pacman_sprite[i][j]
            ));
        }
    }
    uint32_t ghost_colors[] = {0xF00, 0xFCF, 0x0FF, 0xFC5};
    for(uint32_t i = 0; i < 4; ++i){
        for(uint32_t dir = 0; dir < 4; ++dir){
            for(uint32_t fr = 0; fr < 2; ++fr){
                while(ccsrdspr(
                    16 + dir * 32 + fr * 16,
                    100 + i * 16,
                    ghost_colors[i],
                    &ghost_sprite[fr]
                ));
                while(ccsrdspr(
                    16 + dir * 32 + fr * 16,
                    100 + i * 16,
                    0xFFF,
                    &eyes_sprite[dir]
                ));
                while(ccsrdspr(
                    16 + dir * 32 + fr * 16,
                    100 + i * 16,
                    0x00F,
                    &pupils_sprite[dir]
                ));
            }
        }
    }
}

void run(){
    clear_screen();
    // draw_debug_grid();
    draw_map();

    draw_debug_sprites();
    for(;;);

    pacman_t pm = {392, 372, RIGHT, 0};
    ghost_t ghost[4] = {
        {16, 36, RIGHT, 0, 0xF00},
        {16, 52, RIGHT, 0, 0xFCF},
        {16, 68, RIGHT, 0, 0x0FF},
        {16, 84, RIGHT, 0, 0xFC5}
    };
    for(uint32_t i = 0; i < 4; ++i) draw_ghost(&ghost[i]);
    while(ccsrdspr(pm.pos_x, pm.pos_y, 0xFF0, &pacman_sprite[pm.dir][1]));
    for(uint32_t i = 0; i < 20000000; ++i) asm volatile("nop");
    while(ccsrdspr(pm.pos_x, pm.pos_y, 0x000, &pacman_sprite[pm.dir][1]));
    for(uint32_t st = 0;; ++st){
skip:
        // check for collision
        switch(pm.dir){
            case UP: {

            } break;
            case DOWN: {

            } break;
            case LEFT: {

            } break;
            case RIGHT: {
                for(uint32_t i = 0; i < map_line_count; ++i){
                    if(pm.pos_x + 16 == map_line[i].x0 && map_line[i].x0 == map_line[i].x1 && pm.pos_y >= map_line[i].y0 && pm.pos_y < map_line[i].y1){
                        while(ccsrdspr(pm.pos_x, pm.pos_y, 0xFF0, &pacman_sprite[pm.dir][1]));
                        goto skip;
                    }
                }
                ++pm.pos_x;
            } break;
        }

        // right tunnel check
        if(pm.pos_x > 785 && pm.dir == RIGHT) pm.pos_x = 10;
        // left tunnel check
        if(pm.pos_x < 10 && pm.dir == LEFT) pm.pos_x = 785;

        while(ccsrdspr(pm.pos_x, pm.pos_y, 0xFF0, &pacman_sprite[pm.dir][pm.fr]));
        // poll for new keystrokes here
        for(uint32_t i = 0; i < 25000; ++i) asm volatile("nop");
        while(ccsrdspr(pm.pos_x, pm.pos_y, 0x000, &pacman_sprite[pm.dir][pm.fr]));
        pm.fr = (st >> 4) & 0b1;
    }

    // while(ccsrdrct(5, 5, 795, 595, 0xF00));
    for(;;){}
}

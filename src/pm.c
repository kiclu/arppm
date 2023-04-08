#include<pm.h>
#include<gpu.h>
#include<sprite.h>
#include<dbgif.h>
#include<riscv.h>

#include<stdint.h>

static uint32_t time = 0;

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

const uint32_t map_line_count = 178;
const uint32_t map_line_color = 0x11C;
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
    {479, 387, 479, 403},
    {319, 387, 479, 387},
    {319, 403, 479, 403},

    {367, 195, 367, 211},
    {431, 195, 431, 211},
    {367, 195, 431, 195},
    {367, 211, 431, 211},

    // upper left blocks []
    {31, 227, 31, 243},
    {95, 243, 95, 291},
    {111, 227, 111, 291},
    {31, 227, 111, 227},
    {31, 243, 95, 243},
    {95, 291, 111, 291},

    {239, 195, 239, 291},
    {255, 211, 255, 291},
    {351, 195, 351, 211},
    {239, 195, 351, 195},
    {255, 211, 351, 211},
    {239, 291, 255, 291},

    // upper right blocks []
    {687, 227, 687, 291},
    {703, 243, 703, 291},
    {767, 227, 767, 243},
    {687, 227, 767, 227},
    {703, 243, 767, 243},
    {687, 291, 703, 291},

    {447, 195, 447, 211},
    {543, 211, 543, 291},
    {559, 195, 559, 291},
    {447, 195, 559, 195},
    {447, 211, 543, 211},
    {543, 291, 559, 291},

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

    {127, 307, 127, 403},
    {223, 307, 223, 403},
    {127, 307, 223, 307},
    {127, 403, 223, 403},

    {127, 419, 127, 483},
    {223, 419, 223, 483},
    {127, 419, 223, 419},
    {127, 483, 223, 483},

    {239, 419, 239, 483},
    {303, 419, 303, 483},
    {239, 419, 303, 419},
    {239, 483, 303, 483},

    {127, 499, 127, 531},
    {143, 499, 143, 531},
    {127, 499, 143, 499},
    {127, 531, 143, 531},

    {159, 499, 159, 563},
    {303, 499, 303, 563},
    {159, 499, 303, 499},
    {159, 563, 303, 563},

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

    {495, 387, 543, 387},
    {495, 387, 495, 403},
    {495, 403, 559, 403},
    {559, 307, 559, 403},
    {543, 307, 559, 307},
    {543, 307, 543, 387},

    {687, 307, 687, 483},
    {703, 307, 703, 483},
    {687, 307, 703, 307},
    {687, 483, 703, 483},

    {575, 307, 575, 403},
    {671, 307, 671, 403},
    {575, 307, 671, 307},
    {575, 403, 671, 403},

    {495, 419, 495, 483},
    {559, 419, 559, 483},
    {495, 419, 559, 419},
    {495, 483, 559, 483},

    {575, 419, 575, 483},
    {671, 419, 671, 483},
    {575, 419, 671, 419},
    {575, 483, 671, 483},

    {495, 499, 495, 563},
    {639, 499, 639, 563},
    {495, 499, 639, 499},
    {495, 563, 639, 563},

    {655, 499, 655, 531},
    {671, 499, 671, 531},
    {655, 499, 671, 499},
    {655, 531, 671, 531},

    // lower center blocks
    {351, 531, 351, 563},
    {447, 531, 447, 563},
    {351, 531, 447, 531},
    {351, 563, 447, 563},
    
    {319, 499, 319, 563},
    {335, 515, 335, 563},
    {463, 515, 463, 563},
    {479, 499, 479, 563},
    {319, 499, 479, 499},
    {335, 515, 463, 515},
    {319, 563, 335, 563},
    {463, 563, 479, 563},

    {319, 419, 319, 483},
    {335, 419, 335, 467},
    {463, 419, 463, 467},
    {479, 419, 479, 483},
    {319, 419, 335, 419},
    {463, 419, 479, 419},
    {335, 467, 463, 467},
    {319, 483, 479, 483},

    {351, 419, 351, 451},
    {447, 419, 447, 451},
    {351, 419, 447, 419},
    {351, 451, 447, 451}
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

        while(ccsrdpx(map_line[i].x0, map_line[i].y0, background_color));
        while(ccsrdpx(map_line[i].x1, map_line[i].y1, background_color));
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

static void draw_hp_counter(uint32_t hp){
    switch(hp){
        case 3: while(ccsrdspr(47, 584, 0xFF0, &pacman_sprite[RIGHT][1]));
        case 2: while(ccsrdspr(31, 584, 0xFF0, &pacman_sprite[RIGHT][1]));
        case 1: while(ccsrdspr(15, 584, 0xFF0, &pacman_sprite[RIGHT][1]));
        // case 0: print game over
    }
}

static void draw_map(){
    draw_map_lines();
    draw_map_dots();
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
    uint32_t newdir;
    uint32_t fr;
    uint32_t stuck;
} pacman_t;

const uint32_t pacman_color = 0xFF0;

static void draw_pacman(const pacman_t* pm){
    while(ccsrdspr(
        pm->pos_x,
        pm->pos_y,
        pacman_color,
        &pacman_sprite[pm->dir][pm->fr]
    ));
}

static void clear_pacman(const pacman_t* pm){
    if(pm->stuck) return;
    while(ccsrdspr(
        pm->pos_x,
        pm->pos_y,
        background_color,
        &pacman_sprite[pm->dir][pm->fr]
    ));
}

static void check_pm_stuck(pacman_t* pm){
    uint32_t newdir_valid = 1;
    switch(pm->newdir){
        case UP: {
            for(uint32_t i = 0; i < map_line_count; ++i){
                if(map_line[i].y0 != map_line[i].y1) continue;
                if(pm->pos_y != map_line[i].y0 + 1) continue;
                if(pm->pos_x + 15 <= map_line[i].x0) continue;
                if(pm->pos_x > map_line[i].x1) continue;
                newdir_valid = 0;
            }
        } break;
        case DOWN: {
            for(uint32_t i = 0; i < map_line_count; ++i){
                if(map_line[i].y0 != map_line[i].y1) continue;
                if(pm->pos_y + 15 != map_line[i].y0) continue;
                if(pm->pos_x + 15 <= map_line[i].x0) continue;
                if(pm->pos_x > map_line[i].x1) continue;
                newdir_valid = 0;
            }
        } break;
        case LEFT: {
            for(uint32_t i = 0; i < map_line_count; ++i){
                if(map_line[i].x0 != map_line[i].x1) continue;
                if(pm->pos_x != map_line[i].x0 + 1) continue;
                if(pm->pos_y + 15 <= map_line[i].y0) continue;
                if(pm->pos_y > map_line[i].y1) continue;
                newdir_valid = 0;
            }
        }
        case RIGHT: {
            for(uint32_t i = 0; i < map_line_count; ++i){
                if(map_line[i].x0 != map_line[i].x1) continue;
                if(pm->pos_x + 15 != map_line[i].x0) continue;
                if(pm->pos_y + 15 <= map_line[i].y0) continue;
                if(pm->pos_y > map_line[i].y1) continue;
                newdir_valid = 0;
            }
        } break;
    }
    if(newdir_valid){
        pm->dir = pm->newdir;
        pm->stuck = 0;
    }

    switch(pm->dir){
        case UP: {
            for(uint32_t i = 0; i < map_line_count; ++i){
                if(map_line[i].y0 != map_line[i].y1) continue;
                if(pm->pos_y != map_line[i].y0 + 1) continue;
                if(pm->pos_x + 15 <= map_line[i].x0) continue;
                if(pm->pos_x > map_line[i].x1) continue;
                pm->stuck = 1;
                return;
            }
        } break;
        case DOWN: {
            for(uint32_t i = 0; i < map_line_count; ++i){
                if(map_line[i].y0 != map_line[i].y1) continue;
                if(pm->pos_y + 15 != map_line[i].y0) continue;
                if(pm->pos_x + 15 <= map_line[i].x0) continue;
                if(pm->pos_x > map_line[i].x1) continue;
                pm->stuck = 1;
                return;
            }
        } break;
        case LEFT: {
            for(uint32_t i = 0; i < map_line_count; ++i){
                if(map_line[i].x0 != map_line[i].x1) continue;
                if(pm->pos_x != map_line[i].x0 + 1) continue;
                if(pm->pos_y + 15 <= map_line[i].y0) continue;
                if(pm->pos_y > map_line[i].y1) continue;
                pm->stuck = 1;
                return;
            }
        } break;
        case RIGHT: {
            for(uint32_t i = 0; i < map_line_count; ++i){
                if(map_line[i].x0 != map_line[i].x1) continue;
                if(pm->pos_x + 15 != map_line[i].x0) continue;
                if(pm->pos_y + 15 <= map_line[i].y0) continue;
                if(pm->pos_y > map_line[i].y1) continue;
                pm->stuck = 1;
                return;
            }
        } break;
    }
}

static void update_pacman(pacman_t* pm){
    check_pm_stuck(pm);
    if(pm->stuck){
        pm->fr = 1;
        return;
    }

    switch(pm->dir){
        case UP:    --pm->pos_y; break;
        case DOWN:  ++pm->pos_y; break;
        case LEFT:  --pm->pos_x; break;
        case RIGHT: ++pm->pos_x; break;
    }

    // right tunnel check
    if(pm->pos_x > 785 && pm->dir == RIGHT) pm->pos_x = 10;
    // left tunnel check
    if(pm->pos_x < 10 && pm->dir == LEFT) pm->pos_x = 785;

    pm->fr = (time >> 4) & 0b1;
}

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

static void draw_ghosts(const ghost_t* ghosts){
    for(uint32_t i = 0; i < 4; ++i){
        while(ccsrdspr(
            ghosts[i].pos_x,
            ghosts[i].pos_y,
            ghosts[i].color,
            &ghost_sprite[ghosts[i].fr]
        ));

        while(ccsrdspr(
            ghosts[i].pos_x,
            ghosts[i].pos_y,
            0xFFF,
            &eyes_sprite[ghosts[i].dir]
        ));

        while(ccsrdspr(
            ghosts[i].pos_x,
            ghosts[i].pos_y,
            0x00F,
            &pupils_sprite[ghosts[i].dir]
        ));
    }
}

static void clear_ghosts(const ghost_t* ghosts){
    for(uint32_t i = 0; i < 4; ++i){
        while(ccsrdspr(
            ghosts[i].pos_x,
            ghosts[i].pos_y,
            background_color,
            &ghost_sprite[ghosts[i].fr]
        ));

        while(ccsrdspr(
            ghosts[i].pos_x,
            ghosts[i].pos_y, 
            background_color,
            &eyes_sprite[ghosts[i].dir]
        ));

        while(ccsrdspr(
            ghosts[i].pos_x,
            ghosts[i].pos_y,
            background_color,
            &pupils_sprite[ghosts[i].dir]
        ));
    }
}

static int check_pm_eaten(const pacman_t* pm, const ghost_t* ghosts){
    for(uint32_t i = 0; i < 4; ++i){
        int32_t dx = pm->pos_x - ghosts[i].pos_x;
        int32_t dy = pm->pos_y - ghosts[i].pos_y;
        if(dx < 0) dx = ghosts[i].pos_x - pm->pos_x;
        if(dy < 0) dy = ghosts[i].pos_y - pm->pos_y;

        if(dx == 0 && dy < 10) return 1;
        if(dy == 0 && dx < 10) return 1;
    }
    return 0;
}

static void update_ghosts(ghost_t* ghosts, const pacman_t* pm){
    for(uint32_t i = 0; i < 4; ++i){
        if(!ghosts[i].is_released){
            if(ghosts[i].pos_y == 420) ghosts[i].dir = DOWN;
            if(ghosts[i].pos_y == 435) ghosts[i].dir = UP;
        }

        switch(ghosts[i].dir){
            case UP:    --ghosts[i].pos_y; break;
            case DOWN:  ++ghosts[i].pos_y; break;
            case LEFT:  --ghosts[i].pos_x; break;
            case RIGHT: ++ghosts[i].pos_x; break;
        }

        ghosts[i].fr = (time >> 4) & 0b1;
    }
}

void run(){
    for(uint32_t hp = 3; hp > 0; --hp){
        clear_screen();
        draw_map();
        draw_hp_counter(hp);

        pacman_t pm = {392, 372, RIGHT, RIGHT, 1, 0};
        ghost_t ghosts[4] = {
            {368, 420, RIGHT, RIGHT, 1, 0xF00, 0, 0},  // blinky
            {384, 436, RIGHT, RIGHT, 1, 0xFCF, 0, 0},  // pinky
            {400, 420, RIGHT, RIGHT, 1, 0x0FF, 0, 0},  // inky
            {416, 436, RIGHT, RIGHT, 1, 0xFC5, 0, 0}   // clyde
        };

        draw_pacman(&pm);
        draw_ghosts(ghosts);

        // start delay
        for(uint32_t i = 0; i < 20000000; ++i) asm volatile("nop");

        clear_pacman(&pm);
        clear_ghosts(ghosts);

        for(;;){
            // draw sprites
            draw_pacman(&pm);
            draw_ghosts(ghosts);

            // poll for new keystrokes here
            for(uint32_t i = 0; i < 5000; ++i) asm volatile("nop");

            // clear sprites
            clear_pacman(&pm);
            clear_ghosts(ghosts);

            // update pacman
            update_pacman(&pm);

            // update ghosts
            // update_ghosts(ghosts, &pm);

            // check if eaten
            if(check_pm_eaten(&pm, ghosts)){
                // death animation
                draw_pacman(&pm);
                draw_ghosts(ghosts);
                for(uint32_t i = 0; i < 20000000; ++i) asm volatile("nop");
                break;
            }

            ++time;
        }
    }

    clear_screen();
    draw_map();

    for(;;);
}

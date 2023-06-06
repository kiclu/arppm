#include<pm.h>
#include<gpu.h>
#include<sprite.h>
#include<dbgif.h>
#include<riscv.h>
#include<ps2.h>
#include<map.h>

#include<stdint.h>

static uint32_t time = 0;

extern const line_t map_line[map_line_count];
extern const dot_t map_dots[map_dot_count];

extern sprite_t pacman_sprite[4][2];
extern sprite_t ghost_sprite[2];
extern sprite_t eyes_sprite[4];
extern sprite_t pupils_sprite[4];

static void draw_ghosts(const ghost_t* ghosts){
    draw_door();
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

static void check_ghost_stuck(ghost_t* ghost){
    switch(ghost->dir){
        case UP: {
            for(uint32_t i = 0; i < map_line_count; ++i){
                if(map_line[i].y0 != map_line[i].y1) continue;
                if(ghost->pos_y != map_line[i].y0 + 1) continue;
                if(ghost->pos_x + 15 <= map_line[i].x0) continue;
                if(ghost->pos_x > map_line[i].x1) continue;
                ghost->stuck = 1;
                return;
            }
        } break;
        case DOWN: {
            for(uint32_t i = 0; i < map_line_count; ++i){
                if(map_line[i].y0 != map_line[i].y1) continue;
                if(ghost->pos_y + 15 != map_line[i].y0) continue;
                if(ghost->pos_x + 15 <= map_line[i].x0) continue;
                if(ghost->pos_x > map_line[i].x1) continue;
                ghost->stuck = 1;
                return;
            }
        } break;
        case LEFT: {
            for(uint32_t i = 0; i < map_line_count; ++i){
                if(map_line[i].x0 != map_line[i].x1) continue;
                if(ghost->pos_x != map_line[i].x0 + 1) continue;
                if(ghost->pos_y + 15 <= map_line[i].y0) continue;
                if(ghost->pos_y > map_line[i].y1) continue;
                ghost->stuck = 1;
                return;
            }
        } break;
        case RIGHT: {
            for(uint32_t i = 0; i < map_line_count; ++i){
                if(map_line[i].x0 != map_line[i].x1) continue;
                if(ghost->pos_x + 15 != map_line[i].x0) continue;
                if(ghost->pos_y + 15 <= map_line[i].y0) continue;
                if(ghost->pos_y > map_line[i].y1) continue;
                ghost->stuck = 1;
                return;
            }
        } break;
    }
    ghost->stuck = 0;
}

static uint32_t release_counter_init[] = {50, 1000, 1500, 2000};
static uint32_t release_counter[] = {50, 1000, 1500, 2000};

static void update_ghosts(ghost_t* ghosts, const pacman_t* pm){
    if(time & 0b11 == 0) return;
    for(uint32_t i = 0; i < 4; ++i){
        if(release_counter[i] == 0) ghosts[i].is_released = 1;
        else --release_counter[i];

        if(ghosts[i].is_released){
            check_ghost_stuck(&ghosts[i]);
            if(ghosts[i].stuck){
                ghosts[i].dir += time - (i << 1) - pm->newdir;
                ghosts[i].dir &= 0b11;
            }
        }
        else{
            if(ghosts[i].pos_y == 420) ghosts[i].dir = DOWN;
            if(ghosts[i].pos_y == 435) ghosts[i].dir = UP;
        }

        if(!ghosts[i].stuck){
            switch(ghosts[i].dir){
                case UP:    --ghosts[i].pos_y; break;
                case DOWN:  ++ghosts[i].pos_y; break;
                case LEFT:  --ghosts[i].pos_x; break;
                case RIGHT: ++ghosts[i].pos_x; break;
            }

            ghosts[i].fr = (time >> 4) & 0b1;
        }
    }
}

static inline void draw_pacman(const pacman_t* pm){
    while(ccsrdspr(
        pm->pos_x,
        pm->pos_y,
        pacman_color,
        &pacman_sprite[pm->dir][pm->fr]
    ));
}

static inline void clear_pacman(const pacman_t* pm){
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
            if(pm->pos_y + 15 == 419 && pm->pos_x + 15 > 351 && pm->pos_x < 448)
                newdir_valid = 0;
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
        } break;
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
        clear_pacman(pm);
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

static inline void draw_hp_counter(uint32_t hp){
    switch(hp){
        case 3: while(ccsrdspr(47, 584, 0xFF0, &pacman_sprite[RIGHT][1]));
        case 2: while(ccsrdspr(31, 584, 0xFF0, &pacman_sprite[RIGHT][1]));
        case 1: while(ccsrdspr(15, 584, 0xFF0, &pacman_sprite[RIGHT][1]));
        // case 0: print game over
    }
}

static uint32_t score = 0;
static uint32_t score_prev = 0;

extern sprite_t digit_sprite[16];

void update_score(){
    // clear old score
    while(ccsrdspr(
        15,
        7,
        background_color,
        &digit_sprite[(score_prev >> 12) & 0xF]
    ));
    while(ccsrdspr(
        24,
        7,
        background_color,
        &digit_sprite[(score_prev >> 8) & 0xF]
    ));
    while(ccsrdspr(
        33,
        7,
        background_color,
        &digit_sprite[(score_prev >> 4) & 0xF]
    ));
    while(ccsrdspr(
        42,
        7,
        background_color,
        &digit_sprite[score_prev & 0xF]
    ));

    // draw new score
    while(ccsrdspr(
        15,
        7,
        0xFFF,
        &digit_sprite[(score >> 12) & 0xF]
    ));
    while(ccsrdspr(
        24,
        7,
        0xFFF,
        &digit_sprite[(score >> 8) & 0xF]
    ));
    while(ccsrdspr(
        33,
        7,
        0xFFF,
        &digit_sprite[(score >> 4) & 0xF]
    ));
    while(ccsrdspr(
        42,
        7,
        0xFFF,
        &digit_sprite[score & 0xF]
    ));

    score_prev = score;
}

static uint32_t dot_clear_queue[64];
static uint32_t dot_clear_queue_i = 0;

void dot_clear(){
    for(uint32_t i = 0; i < dot_clear_queue_i; ++i){
        while(ccsrdrct(
            map_dots[dot_clear_queue[i]].x,
            map_dots[dot_clear_queue[i]].y,
            map_dots[dot_clear_queue[i]].x + 2,
            map_dots[dot_clear_queue[i]].y + 2,
            0x000
        ));
    }
    dot_clear_queue_i = 0;
}


static uint32_t dot_restore_queue[64];
static uint32_t dot_restore_queue_i = 0;

void dot_restore(){
    for(uint32_t i = 0; i < dot_restore_queue_i; ++i){
        while(ccsrdrct(
            map_dots[dot_restore_queue[i]].x,
            map_dots[dot_restore_queue[i]].y,
            map_dots[dot_restore_queue[i]].x + 2,
            map_dots[dot_restore_queue[i]].y + 2,
            0xFFF
        ));
    }
    dot_restore_queue_i = 0;
}

static void update_dots(ghost_t* ghosts, pacman_t* pm){
    for(register uint32_t i = 0; i < map_dot_count; ++i){
        // check if dot in 8 pixel radius of pacman
        // if true, set dot eaten
        if(
            pm->pos_x + 16 > map_dots[i].x &&
            pm->pos_x      < map_dots[i].x &&
            pm->pos_y + 16 > map_dots[i].y &&
            pm->pos_y      < map_dots[i].y &&
            !check_dot_eaten(i)
        ){
            set_dot_eaten(i);
            score += 0x10;
            dot_clear_queue[dot_clear_queue_i++] = i;
        }

        // check if ghost has passed any dots that are not eaten
        // if true, add them to dot_restore_queue
        for(uint32_t k = 0; k < 4; ++k){
            if(
                ghosts[k].pos_x - 8  < map_dots[i].x &&
                ghosts[k].pos_x + 32 > map_dots[i].x && 
                ghosts[k].pos_y - 8  < map_dots[i].y &&
                ghosts[k].pos_y + 32 > map_dots[i].y &&
                !check_dot_eaten(i)
            ){
                dot_restore_queue[dot_restore_queue_i++] = i;
            }
        }
    }
}

static uint32_t restart = 0;
static inline uint32_t check_restart(){
    if(restart){
        dot_clear_queue_i = 0;
        dot_restore_queue_i = 0;

        clear_dot_eaten();

        restart = 0;
        return 1;
    }
    return 0;
}

static void update_newdir(pacman_t* pm){
    // fetch ps2 buffer contents
    uint32_t ps2_buf = rdps2();

    // find first valid key
    uint32_t mask = 0xFF000000;
    for(uint32_t sh = 24; ; sh -= 8){
        if((ps2_buf & mask) >> (sh + 8) == 0xF0){
            sh >>= 8;
        }
        switch((ps2_buf & mask) >> sh){
            case 0x00: return;
            case 0x1B: pm->newdir = DOWN;   return;
            case 0x1C: pm->newdir = LEFT;   return;
            case 0x1D: pm->newdir = UP;     return;
            case 0x23: pm->newdir = RIGHT;  return;
            case 0x2D: restart = 1; return;
            default:   mask >>= 8;
        }
    }
}

void run(){
run:
    for(uint32_t hp = 3; hp > 0; --hp){
        clear_screen();
        draw_map();
        draw_hp_counter(hp);
        update_score();

        // initialize player and ghosts
        pacman_t pm = {384, 372, RIGHT, RIGHT, 1, 0};
        ghost_t ghosts[4] = {
        //   x      y       dir     newdir  fr  color   stuck   is_released
            {368,   420,    DOWN,   DOWN,   0,  0xF00,  0,      0},  // blinky
            {384,   436,    UP,     UP,     0,  0xFCF,  0,      0},  // pinky
            {400,   420,    DOWN,   DOWN,   1,  0x0FF,  0,      0},  // inky
            {416,   436,    UP,     UP,     1,  0xFC5,  0,      0}   // clyde
        };

        for(uint32_t i = 0; i < 4; ++i){
            release_counter[i] = release_counter_init[i];
        }

        draw_pacman(&pm);
        draw_ghosts(ghosts);

        // start delay
        for(uint32_t i = 0; i < 20000000; ++i) asm volatile("nop");

        clear_pacman(&pm);
        clear_ghosts(ghosts);

        for(;;){
            // draw sprites
            dot_restore();
            draw_pacman(&pm);
            draw_ghosts(ghosts);

            // update score
            update_dots(ghosts, &pm);
            update_score();

            for(uint32_t i = 0; i < 4600; ++i) asm volatile("nop"); // 4600

            // clear sprites
            clear_pacman(&pm);
            clear_ghosts(ghosts);
            dot_clear();

            // fetch keyboard inputs and decode them
            update_newdir(&pm);

            if(check_restart()) goto run;

            // update pacman
            update_pacman(&pm);

            // update ghosts
            update_ghosts(ghosts, &pm);

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

    for(;;){
        pacman_t pm;
        update_newdir(&pm);
        if(check_restart()) goto run;
    }
}

#include<pm.h>
#include<gpu.h>
#include<sprite.h>
#include<dbgif.h>
#include<riscv.h>
#include<ps2.h>

#include<stdint.h>

static uint32_t time = 0;

const uint32_t background_color = 0x000;

static void clear_screen(){
    while(ccsrdrct(0, 0, GPU_SCREEN_WIDTH, GPU_SCREEN_HEIGHT, background_color));
}

typedef struct{
    uint32_t x0;
    uint32_t y0;
    uint32_t x1;
    uint32_t y1;
} line_t;

const uint32_t map_line_count = 265;
const uint32_t map_line_color = 0x11C;
const line_t map_line[] = {
    // upper border [9]
    { 15, 291,  79, 291},
    { 79, 259,  79, 291},
    { 15, 259,  79, 259},
    { 15,  19,  15, 259},
    { 15,  19, 783,  19},
    {783,  19, 783, 259},
    {719, 259, 783, 259},
    {719, 259, 719, 291},
    {719, 291, 783, 291},

    // lower border [9]
    { 15, 307,  79, 307},
    { 79, 307,  79, 339},
    { 15, 339,  79, 339},
    { 15, 339,  15, 579},
    { 15, 579, 783, 579},
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
    { 31,  35,  31, 211},
    { 47,  35,  47, 195},
    {223, 195, 223, 211},
    { 31,  35,  47,  35},
    { 47, 195, 223, 195},
    { 31, 211, 223, 211},

    {127, 227, 127, 291},
    {223, 227, 223, 291},
    {127, 227, 223, 227},
    {127, 291, 223, 291},

    { 31, 227,  31, 243},
    { 95, 243,  95, 291},
    {111, 227, 111, 291},
    { 31, 227, 111, 227},
    { 31, 243,  95, 243},
    { 95, 291, 111, 291},

    {239, 195, 239, 291},
    {255, 211, 255, 291},
    {351, 195, 351, 211},
    {239, 195, 351, 195},
    {255, 211, 351, 211},
    {239, 291, 255, 291},

    { 63,  35,  63,  83},
    {175,  35, 175,  83},
    { 63,  35, 175,  35},
    { 63,  83, 175,  83},

    {191,  35, 191,  83},
    {223,  35, 223,  83},
    {191,  35, 223,  35},
    {191,  83, 223,  83},

    { 63,  99,  63, 179},
    {111,  99, 111, 179},
    { 63,  99, 111,  99},
    { 63, 179, 111, 179},

    {127,  99, 127, 179},
    {223,  99, 223, 179},
    {127,  99, 223,  99},
    {127, 179, 223, 179},

    {239,  67, 239,  83},
    {319,  67, 319,  83},
    {239,  67, 319,  67},
    {239,  83, 319,  83},

    {239,  99, 239, 179},
    {319,  99, 319, 179},
    {239,  99, 319,  99},
    {239, 179, 319, 179},

    {239,  35, 239,  51},
    {335,  51, 335, 179},
    {351,  35, 351, 179},
    {239,  35, 351,  35},
    {239,  51, 335,  51},
    {335, 179, 351, 179},

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

    {575, 195, 575, 211},
    {751,  35, 751, 195},
    {767,  35, 767, 211},
    {751,  35, 767,  35},
    {575, 195, 751, 195},
    {575, 211, 767, 211},

    {575, 227, 575, 291},
    {671, 227, 671, 291},
    {575, 227, 671, 227},
    {575, 291, 671, 291},

    {575,  35, 575,  83},
    {607,  35, 607,  83},
    {575,  35, 607,  35},
    {575,  83, 607,  83},

    {623,  35, 623,  83},
    {735,  35, 735,  83},
    {623,  35, 735,  35},
    {623,  83, 735,  83},

    {575,  99, 575, 179},
    {671,  99, 671, 179},
    {575,  99, 671,  99},
    {575, 179, 671, 179},

    {687,  99, 687, 179},
    {735,  99, 735, 179},
    {687,  99, 735,  99},
    {687, 179, 735, 179},

    {479,  67, 479,  83},
    {559,  67, 559,  83},
    {479,  67, 559,  67},
    {479,  83, 559,  83},

    {479,  99, 479, 179},
    {559,  99, 559, 179},
    {479,  99, 559,  99},
    {479, 179, 559, 179},

    {447,  35, 447, 179},
    {463,  51, 463, 179},
    {559,  35, 559,  51},
    {447,  35, 559,  35},
    {463,  51, 559,  51},
    {447, 179, 463, 179},

    // upper centre blocks
    {367,  35, 367,  99},
    {431,  35, 431,  99},
    {367,  35, 431,  35},
    {367,  99, 431,  99},

    {367, 115, 367, 179},
    {431, 115, 431, 179},
    {367, 115, 431, 115},
    {367, 179, 431, 179},

    // lower left blocks [30]
    { 31, 355,  31, 435},
    { 79, 355,  79, 435},
    { 31, 355,  79, 355},
    { 31, 435,  79, 435},
    
    { 31, 451,  31, 483},
    { 79, 451,  79, 483},
    { 31, 451,  79, 451},
    { 31, 483,  79, 483},

    { 31, 499,  31, 531},
    { 79, 499,  79, 531},
    { 31, 499,  79, 499},
    { 31, 531,  79, 531},

    { 95, 499,  95, 547},
    { 31, 547,  31, 563},
    {111, 499, 111, 547},
    {143, 547, 143, 563},
    { 95, 499, 111, 499},
    { 31, 547,  95, 547},
    {111, 547, 143, 547},
    { 31, 563, 143, 563},

    {239, 307, 239, 403},
    {239, 403, 303, 403},
    {303, 387, 303, 403},
    {255, 387, 303, 387},
    {255, 307, 255, 387},
    {239, 307, 255, 307},

    { 95, 307,  95, 483},
    {111, 307, 111, 483},
    { 95, 307, 111, 307},
    { 95, 483, 111, 483},

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
    {351, 451, 447, 451}
};

static void draw_door(){
    while(ccsrdln(352, 419, 446, 419, 0x666));
}

static void draw_map_lines(){
    draw_door();
    for(uint32_t i = 0; i < map_line_count; ++i){
        while(ccsrdln(
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

typedef struct{
    uint32_t x0;
    uint32_t y0;
    uint32_t x1;
    uint32_t y1;
} rect_t;

const uint32_t map_rect_count = 19;
const uint32_t map_rect_color = 0x009;
const rect_t map_rect[] = {
    // A
    {328, 258, 335, 265},
    {320, 266, 343, 273},
    {312, 274, 327, 281},
    {336, 274, 351, 281},
    {304, 282, 319, 329},
    {344, 282, 359, 329},
    {320, 298, 343, 305},
    // R
    {368, 258, 415, 265},
    {376, 266, 391, 321},
    {408, 266, 423, 289},
    {392, 290, 415, 297},
    {400, 298, 415, 305},
    {408, 306, 423, 329},
    {368, 322, 391, 329},
    // P
    {432, 258, 479, 265},
    {440, 266, 455, 321},
    {472, 266, 487, 289},
    {456, 290, 479, 297},
    {432, 322, 463, 329}
};

static void draw_map_rects(){
    for(uint32_t i = 0; i < map_rect_count; ++i){
        while(ccsrdrct(
            map_rect[i].x0 + 6,
            map_rect[i].y0 + 8,
            map_rect[i].x1 + 6,
            map_rect[i].y1 + 8,
            map_rect_color
        ));
    }
}

typedef struct{
    uint32_t x;
    uint32_t y;
} dot_t;

const uint32_t map_dot_count = 681;
const uint32_t map_dot_color = 0xFFF;
const dot_t map_dots[] = {
    {22,   26}, { 38,  26}, { 54,  26}, { 70,  26}, { 86,  26}, {102,  26},
    {118,  26}, {134,  26}, {150,  26}, {166,  26}, {182,  26}, {198,  26}, 
    {214,  26}, {230,  26}, {246,  26}, {262,  26}, {278,  26}, {294,  26},
    {310,  26}, {326,  26}, {342,  26}, {358,  26}, {374,  26}, {390,  26},
    {406,  26}, {422,  26}, {438,  26}, {454,  26}, {470,  26}, {486,  26},
    {502,  26}, {518,  26}, {534,  26}, {550,  26}, {566,  26}, {582,  26},
    {598,  26}, {614,  26}, {630,  26}, {646,  26}, {662,  26}, {678,  26},
    {694,  26}, {710,  26}, {726,  26}, {742,  26}, {758,  26}, {774,  26},
    {22,   42}, { 54,  42}, {182,  42}, {230,  42}, {358,  42}, {438,  42},
    {566,  42}, {614,  42}, {742,  42}, {774,  42}, { 22,  58}, { 54,  58},
    {182,  58}, {230,  58}, {246,  58}, {262,  58}, {278,  58}, {294,  58},
    {310,  58}, {326,  58}, {358,  58}, {438,  58}, {470,  58}, {486,  58},
    {502,  58}, {518,  58}, {534,  58}, {550,  58}, {566,  58}, {614,  58},
    {742,  58}, {774,  58}, { 22,  74}, { 54,  74}, {182,  74}, {230,  74},
    {326,  74}, {358,  74}, {438,  74}, {470,  74}, {566,  74}, {614,  74},
    {742,  74}, {774,  74}, { 22,  90}, { 54,  90}, { 70,  90}, { 86,  90},
    {102,  90}, {118,  90}, {134,  90}, {150,  90}, {166,  90}, {182,  90},
    {198,  90}, {214,  90}, {230,  90}, {246,  90}, {262,  90}, {278,  90},
    {294,  90}, {310,  90}, {326,  90}, {358,  90}, {438,  90}, {470,  90},
    {486,  90}, {502,  90}, {518,  90}, {534,  90}, {550,  90}, {566,  90},
    {582,  90}, {598,  90}, {614,  90}, {630,  90}, {646,  90}, {662,  90},
    {678,  90}, {694,  90}, {710,  90}, {726,  90}, {742,  90}, {774,  90},
    {22,  106}, { 54, 106}, {118, 106}, {230, 106}, {326, 106}, {358, 106},
    {438, 106}, {470, 106}, {566, 106}, {678, 106}, {742, 106}, {774, 106},
    { 22, 122}, { 54, 122}, {118, 122}, {230, 122}, {326, 122}, {358, 122},
    {438, 122}, {470, 122}, {566, 122}, {678, 122}, {742, 122}, {774, 122},
    { 22, 138}, { 54, 138}, {118, 138}, {230, 138}, {326, 138}, {358, 138},
    {438, 138}, {470, 138}, {566, 138}, {678, 138}, {742, 138}, {774, 138},
    { 22, 154}, { 54, 154}, {118, 154}, {230, 154}, {326, 154}, {358, 154},
    {438, 154}, {470, 154}, {566, 154}, {678, 154}, {742, 154}, {774, 154},
    { 22, 170}, { 54, 170}, {118, 170}, {230, 170}, {326, 170}, {358, 170},
    {438, 170}, {470, 170}, {566, 170}, {678, 170}, {742, 170}, {774, 170},
    { 22, 186}, { 54, 186}, { 70, 186}, { 86, 186}, {102, 186}, {118, 186},
    {134, 186}, {150, 186}, {166, 186}, {182, 186}, {198, 186}, {214, 186},
    {230, 186}, {246, 186}, {262, 186}, {278, 186}, {294, 186}, {310, 186},
    {326, 186}, {342, 186}, {358, 186}, {374, 186}, {390, 186}, {406, 186},
    {422, 186}, {438, 186}, {454, 186}, {470, 186}, {486, 186}, {502, 186},
    {518, 186}, {534, 186}, {550, 186}, {566, 186}, {582, 186}, {598, 186},
    {614, 186}, {630, 186}, {646, 186}, {662, 186}, {678, 186}, {694, 186},
    {710, 186}, {726, 186}, {742, 186}, {774, 186}, { 22, 202}, {230, 202},
    {358, 202}, {438, 202}, {566, 202}, {774, 202}, { 22, 218}, { 38, 218},
    { 54, 218}, { 70, 218}, { 86, 218}, {102, 218}, {118, 218}, {134, 218},
    {150, 218}, {166, 218}, {182, 218}, {198, 218}, {214, 218}, {230, 218},
    {262, 218}, {278, 218}, {294, 218}, {310, 218}, {326, 218}, {342, 218},
    {358, 218}, {374, 218}, {390, 218}, {406, 218}, {422, 218}, {438, 218},
    {454, 218}, {470, 218}, {486, 218}, {502, 218}, {518, 218}, {534, 218},
    {566, 218}, {582, 218}, {598, 218}, {614, 218}, {630, 218}, {646, 218},
    {662, 218}, {678, 218}, {694, 218}, {710, 218}, {726, 218}, {742, 218},
    {758, 218}, {774, 218}, {118, 234}, {230, 234}, {262, 234}, {534, 234},
    {566, 234}, {678, 234}, {774, 234}, { 22, 250}, { 38, 250}, { 54, 250},
    { 70, 250}, { 86, 250}, {118, 250}, {230, 250}, {262, 250}, {534, 250},
    {566, 250}, {678, 250}, {710, 250}, {726, 250}, {742, 250}, {758, 250},
    {774, 250}, { 86, 266}, {118, 266}, {230, 266}, {262, 266}, {534, 266},
    {566, 266}, {678, 266}, {710, 266}, { 86, 282}, {118, 282}, {230, 282},
    {262, 282}, {534, 282}, {566, 282}, {678, 282}, {710, 282}, { 86, 298},
    {102, 298}, {118, 298}, {134, 298}, {150, 298}, {166, 298}, {182, 298},
    {198, 298}, {214, 298}, {230, 298}, {246, 298}, {262, 298}, {534, 298},
    {550, 298}, {566, 298}, {582, 298}, {598, 298}, {614, 298}, {630, 298},
    {646, 298}, {662, 298}, {678, 298}, {694, 298}, {710, 298}, { 86, 314},
    {118, 314}, {230, 314}, {262, 314}, {534, 314}, {566, 314}, {678, 314},
    {710, 314}, { 86, 330}, {118, 330}, {230, 330}, {262, 330}, {534, 330},
    {566, 330}, {678, 330}, {710, 330}, { 22, 346}, { 38, 346}, { 54, 346},
    { 70, 346}, { 86, 346}, {118, 346}, {230, 346}, {262, 346}, {534, 346},
    {566, 346}, {678, 346}, {710, 346}, {726, 346}, {742, 346}, {758, 346},
    {774, 346}, { 22, 362}, { 86, 362}, {118, 362}, {230, 362}, {262, 362},
    {534, 362}, {566, 362}, {678, 362}, {710, 362}, {774, 362}, { 22, 378},
    { 86, 378}, {118, 378}, {230, 378}, {262, 378}, {278, 378}, {294, 378},
    {310, 378}, {326, 378}, {342, 378}, {358, 378}, {374, 378}, { 22, 233},
    {406, 378}, {422, 378}, {438, 378}, {454, 378}, {470, 378}, {486, 378},
    {502, 378}, {518, 378}, {534, 378}, {566, 378}, {678, 378}, {710, 378},
    {774, 378}, { 22, 394}, { 86, 394}, {118, 394}, {230, 394}, {310, 394},
    {486, 394}, {566, 394}, {678, 394}, {710, 394}, {774, 394}, { 22, 410},
    { 86, 410}, {118, 410}, {134, 410}, {150, 410}, {166, 410}, {182, 410},
    {198, 410}, {214, 410}, {230, 410}, {246, 410}, {262, 410}, {278, 410},
    {294, 410}, {310, 410}, {326, 410}, {342, 410}, {358, 410}, {374, 410},
    {390, 410}, {406, 410}, {422, 410}, {438, 410}, {454, 410}, {470, 410},
    {486, 410}, {502, 410}, {518, 410}, {534, 410}, {550, 410}, {566, 410},
    {582, 410}, {598, 410}, {614, 410}, {630, 410}, {646, 410}, {662, 410},
    {678, 410}, {710, 410}, {774, 410}, { 22, 426}, { 86, 426}, {118, 426},
    {230, 426}, {310, 426}, {342, 426}, {454, 426}, {486, 426}, {566, 426},
    {678, 426}, {710, 426}, {774, 426}, { 22, 442}, { 38, 442}, { 54, 442},
    { 70, 442}, { 86, 442}, {118, 442}, {230, 442}, {310, 442}, {342, 442},
    {454, 442}, {486, 442}, {566, 442}, {678, 442}, {710, 442}, {726, 442},
    {742, 442}, {758, 442}, {774, 442}, { 22, 458}, { 86, 458}, {118, 458},
    {230, 458}, {310, 458}, {342, 458}, {358, 458}, {374, 458}, {390, 458},
    {406, 458}, {422, 458}, {438, 458}, {454, 458}, {486, 458}, {566, 458},
    {678, 458}, {710, 458}, {774, 458}, { 22, 474}, { 86, 474}, {118, 474},
    {230, 474}, {310, 474}, {486, 474}, {566, 474}, {678, 474}, {710, 474},
    {774, 474}, { 22, 490}, { 38, 490}, { 54, 490}, { 70, 490}, { 86, 490},
    {102, 490}, {118, 490}, {134, 490}, {150, 490}, {166, 490}, {182, 490},
    {198, 490}, {214, 490}, {230, 490}, {246, 490}, {262, 490}, {278, 490},
    {294, 490}, {310, 490}, {326, 490}, {342, 490}, {358, 490}, {374, 490},
    {390, 490}, {406, 490}, {422, 490}, {438, 490}, {454, 490}, {470, 490},
    {486, 490}, {502, 490}, {518, 490}, {534, 490}, {550, 490}, {566, 490},
    {582, 490}, {598, 490}, {614, 490}, {630, 490}, {646, 490}, {662, 490},
    {678, 490}, {694, 490}, {710, 490}, {726, 490}, {742, 490}, {758, 490},
    {774, 490}, { 22, 506}, { 86, 506}, {118, 506}, {150, 506}, {310, 506},
    {486, 506}, {646, 506}, {678, 506}, {710, 506}, {774, 506}, { 22, 522},
    { 86, 522}, {118, 522}, {150, 522}, {310, 522}, {342, 522}, {358, 522},
    {374, 522}, {390, 522}, {406, 522}, {422, 522}, {438, 522}, {454, 522},
    {486, 522}, {646, 522}, {678, 522}, {710, 522}, {774, 522}, { 22, 538},
    { 38, 538}, { 54, 538}, { 70, 538}, { 86, 538}, {118, 538}, {134, 538},
    {150, 538}, {310, 538}, {342, 538}, {454, 538}, {486, 538}, {646, 538},
    {662, 538}, {678, 538}, {710, 538}, {726, 538}, {742, 538}, {758, 538},
    {774, 538}, { 22, 554}, {150, 554}, {310, 554}, {342, 554}, {454, 554},
    {486, 554}, {646, 554}, {774, 554}, { 22, 570}, { 38, 570}, { 54, 570},
    { 70, 570}, { 86, 570}, {102, 570}, {118, 570}, {134, 570}, {150, 570},
    {166, 570}, {182, 570}, {198, 570}, {214, 570}, {230, 570}, {246, 570},
    {262, 570}, {278, 570}, {294, 570}, {310, 570}, {326, 570}, {342, 570},
    {358, 570}, {374, 570}, {390, 570}, {406, 570}, {422, 570}, {438, 570},
    {454, 570}, {470, 570}, {486, 570}, {502, 570}, {518, 570}, {534, 570},
    {550, 570}, {566, 570}, {582, 570}, {598, 570}, {614, 570}, {630, 570},
    {646, 570}, {662, 570}, {678, 570}, {694, 570}, {710, 570}, {726, 570},
    {742, 570}, {758, 570}, {774, 570}
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
    draw_map_rects();
}

// TODO: remove
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

static int restart = 0;
int check_restart(){
    if(restart){
        restart = 0;
        return 1;
    }
    return 0;
}

static void update_newdir(pacman_t* pm){
    // fetch ps2 buffer contents
    uint32_t ps2_buf = rdps2();

    if(ps2_buf) dbgif_print_hex(ps2_buf >> 16, 0);

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

        // disable horizontal draw area to minimize sprite tearing
        // hdadisable();

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


            for(uint32_t i = 0; i < 4600; ++i) asm volatile("nop");

            // clear sprites
            clear_pacman(&pm);
            clear_ghosts(ghosts);

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
        if(check_restart()) goto run;
    }
}

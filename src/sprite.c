#include<gpu.h>
#include<sprite.h>

const sprite_t pacman[4][2] = {
    // direction UP
    {
        {
            0x01010101, 0x01010101, 0x01010101, 0x01010101,
            0x01010101, 0x01010101, 0x01010101, 0x01010101
        },
        {
            0x01010101, 0x01010101, 0x01010101, 0x01010101,
            0x01010101, 0x01010101, 0x01010101, 0x01010101
        }
    },
    // direction DOWN
    {
        {
            0x01010101, 0x01010101, 0x01010101, 0x01010101,
            0x01010101, 0x01010101, 0x01010101, 0x01010101
        },
        {
            0x01010101, 0x01010101, 0x01010101, 0x01010101,
            0x01010101, 0x01010101, 0x01010101, 0x01010101
        }
    },
    // direction LEFT
    {
        {
            0x01010101, 0x01010101, 0x01010101, 0x01010101,
            0x01010101, 0x01010101, 0x01010101, 0x01010101
        },
        {
            0x01010101, 0x01010101, 0x01010101, 0x01010101,
            0x01010101, 0x01010101, 0x01010101, 0x01010101
        }
    },
    // direction RIGHT
    {
        {
            0x01010101, 0x01010101, 0x01010101, 0x01010101,
            0x01010101, 0x01010101, 0x01010101, 0x01010101
        },
        {
            0x00000000, 0x07C00000, 0x3FF81FF0, 0x0FFC3FF8,
            0x003C01FC, 0x0FFC01FC, 0x3FF83FF8, 0x07C01FF0
        }
    }
};

const sprite_t ghost[4][2] = {
    // direction UP
    {
        {
            0x01010101, 0x01010101, 0x01010101, 0x01010101,
            0x01010101, 0x01010101, 0x01010101, 0x01010101
        },
        {
            0x01010101, 0x01010101, 0x01010101, 0x01010101,
            0x01010101, 0x01010101, 0x01010101, 0x01010101
        }
    },
    // direction DOWN
    {
        {
            0x01010101, 0x01010101, 0x01010101, 0x01010101,
            0x01010101, 0x01010101, 0x01010101, 0x01010101
        },
        {
            0x01010101, 0x01010101, 0x01010101, 0x01010101,
            0x01010101, 0x01010101, 0x01010101, 0x01010101
        }
    },
    // direction LEFT
    {
        {
            0x01010101, 0x01010101, 0x01010101, 0x01010101,
            0x01010101, 0x01010101, 0x01010101, 0x01010101
        },
        {
            0x01010101, 0x01010101, 0x01010101, 0x01010101,
            0x01010101, 0x01010101, 0x01010101, 0x01010101
        }
    },
    // direction RIGHT
    {
        {
            0x01010101, 0x01010101, 0x01010101, 0x01010101,
            0x01010101, 0x01010101, 0x01010101, 0x01010101
        },
        {
            0x01010101, 0x01010101, 0x01010101, 0x01010101,
            0x01010101, 0x01010101, 0x01010101, 0x01010101
        }
    }
};
#ifndef _ARP_PS2_H
#define _ARP_PS2_H

#include<stdint.h>

enum scancode{
    ESC     = 0x76,
    F1      = 0x05,
    F2      = 0x06,
    F3      = 0x04,
    F4      = 0x0C,
    F5      = 0x03,
    F6      = 0x0B,
    F7      = 0x83,
    F8      = 0x0A,
    F9      = 0x01,
    F10     = 0x09,
    F11     = 0x78,
    F12     = 0x07,

    ONE     = 0x16,
    TWO     = 0x1E,
    THREE   = 0x26,
    FOUR    = 0x25,
    FIVE    = 0x2E,
    SIX     = 0x36,
    SEVEN   = 0x3D,
    EIGHT   = 0x3E,
    NINE    = 0x46,
    ZERO    = 0x45,
    MINUS   = 0x4E,
    EQUALS  = 0x55,

    Q       = 0x15,
    W       = 0x1D,
    E       = 0x24,
    R       = 0x2D,
    T       = 0x2C,
    Y       = 0x35,
    U       = 0x3C,
    I       = 0x43,
    O       = 0x44,
    P       = 0x4D,

    A       = 0x1C,
    S       = 0x1B,
    D       = 0x23,
    F       = 0x2B,
    G       = 0x34,
    H       = 0x33,
    J       = 0x3B,
    K       = 0x42,
    L       = 0x4B,

    Z       = 0x1A,
    X       = 0x22,
    C       = 0x21,
    V       = 0x2A,
    B       = 0x32,
    N       = 0x31,
    M       = 0x3A
};

uint32_t rdps2();

#endif//_ARP_PS2_H

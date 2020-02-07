#ifndef EMULATOR_H
#define EMULATOR_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "screen.h"
#include "registers.h"
#include "config.h"

struct chip8
{
    struct chip8_screen screen;
    struct chip8_registers registers;
    char memory[CHIP8_MEMORY_SIZE];

    // Holds return addresses for subroutine calls
    unsigned short stack[CHIP8_STACK_SIZE];
};

int emulate();
#endif
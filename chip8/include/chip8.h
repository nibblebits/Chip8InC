#ifndef CHIP8_H
#define CHIP8_H

#include <stdbool.h>
#include <stdlib.h>

#include "screen.h"
#include "registers.h"
#include "keyboard.h"
#include "simulator.h"
#include "cmemory.h"

#include "config.h"

typedef char(*CHIP8_WAIT_FOR_KEY_FUNC)();
typedef void(*CHIP8_RENDER_FUNC)();
typedef bool(*CHIP8_POLL_EVENTS_FUNC)();


struct chip8
{
    struct chip8_screen screen;
    struct chip8_registers registers;
    struct chip8_keyboard keyboard;
    char memory[CHIP8_MEMORY_SIZE];

    // Holds return addresses for subroutine calls
    unsigned short stack[CHIP8_STACK_SIZE];

    // This function pointer is invoked when chip8 should wait for a key
    CHIP8_WAIT_FOR_KEY_FUNC wait_for_key;

    // Called when we are ready to draw to the screen
    CHIP8_RENDER_FUNC render;

};

void chip8_init(struct chip8* chip8, CHIP8_WAIT_FOR_KEY_FUNC wait_for_key_press, CHIP8_RENDER_FUNC render_func);
void chip8_load(struct chip8* chip8, char *program, size_t length);
bool chip8_loop(struct chip8* chip8);
#endif
#include "cmemory.h"
#include "emulator.h"
#include <stdbool.h>

static bool chip8_is_memory_out_of_bounds(unsigned short addr)
{
    return addr > CHIP8_MEMORY_SIZE;
}

unsigned short chip8_read_short(struct chip8 *chip8, unsigned short addr)
{
    if (chip8_is_memory_out_of_bounds(addr))
        return 0;

    unsigned char b1 = chip8_read_byte(chip8, addr);
    unsigned char b2 = chip8_read_byte(chip8, addr+1);
    return b1 << 8 | b2;
}

unsigned char chip8_read_byte(struct chip8 *chip8, unsigned short addr)
{
    if (chip8_is_memory_out_of_bounds(addr))
        return 0;

    return chip8->memory[addr];
}
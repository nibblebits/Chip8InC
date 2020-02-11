#include "chip8.h"
#include <stdio.h>
#include <memory.h>
#include <unistd.h>


// This is the default sprite set that represents sprites in the CHIP8, seek manual for more information
const char chip8_default_sprite_set[] = {0xf0, 0x90, 0x90, 0x90, 0xf0,
                                         0x20, 0x60, 0x20, 0x20, 0x70,
                                         0xf0, 0x10, 0xf0, 0x80, 0xf0,
                                         0xf0, 0x10, 0xf0, 0x10, 0xf0,
                                         0x90, 0x90, 0xf0, 0x10, 0x10,
                                         0xf0, 0x80, 0xf0, 0x10, 0xf0,
                                         0xf0, 0x80, 0xf0, 0x90, 0xf0,
                                         0xf0, 0x10, 0x20, 0x40, 0x40,
                                         0xf0, 0x90, 0xf0, 0x90, 0xf0,
                                         0xf0, 0x90, 0xf0, 0x10, 0xf0,
                                         0xf0, 0x90, 0xf0, 0x90, 0x90,
                                         0xe0, 0x90, 0xe0, 0x90, 0xe0,
                                         0xf0, 0x80, 0x80, 0x80, 0xf0,
                                         0xe0, 0x90, 0x90, 0x90, 0xe0,
                                         0xf0, 0x80, 0xf0, 0x80, 0xf0,
                                         0xf0, 0x80, 0xf0, 0x80, 0x80};

void chip8_init(struct chip8* chip8, CHIP8_WAIT_FOR_KEY_FUNC wait_for_key, CHIP8_RENDER_FUNC render_func)
{

    memset(chip8, 0, sizeof(chip8));

    // Let's copy in the sprite set
    memcpy(&chip8->memory[0x00], &chip8_default_sprite_set, sizeof(chip8_default_sprite_set));

    // Chip8 should start running from 0x200
    chip8_set_PC(&chip8->registers, CHIP8_PROGRAM_LOAD_ADDRESS);

    chip8->wait_for_key = wait_for_key;
    chip8->render = render_func;
}

bool chip8_loop(struct chip8* chip8)
{
    // Delays can happen
    if (chip8->registers.delay_timer > 0)
    {
        chip8_set_delay_timer(&chip8->registers, chip8->registers.delay_timer - 1);
        return true;
    }

    if (chip8->registers.sound_timer > 0)
    {
        chip8_set_sound_timer(&chip8->registers, chip8->registers.sound_timer - 1);
        printf("BING!\n");
    }

    // Execute the instruction
    unsigned int lpc = chip8->registers.PC;
    unsigned short opcode = chip8_read_short(chip8, chip8->registers.PC);

    // We must increment the program counter
    chip8_next_PC(&chip8->registers);

#ifdef DEBUG
    printf("Executing %x val=%x\n", lpc, opcode);
#endif

    chip8_exec(chip8, opcode);

    chip8->render();
    return true;
}

void chip8_load(struct chip8* chip8, char *program, size_t length)
{
    memcpy(&chip8->memory[CHIP8_PROGRAM_LOAD_ADDRESS], program, length);
}

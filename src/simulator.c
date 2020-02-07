#include "simulator.h"
#include "emulator.h"
#include "registers.h"
#include <memory.h>

void chip8_exec(struct chip8 *chip8, unsigned short opcode)
{

    switch (opcode)
    {
    // CLS, Clear the display
    case 0x00e0:
        memset(chip8->screen.pixels, 0, sizeof(chip8->screen.pixels));
        break;

    // Ret, return from subroutine
    case 0x00ee:
        chip8_set_PC(&chip8->registers, chip8->stack[chip8->registers.SP]);
        chip8_set_SP(&chip8->registers, chip8->registers.SP-1);
    break;
    }

    unsigned short opcode_1st = opcode & 0xf000;
    switch(opcode_1st)
    {
        // 1nnn JP Addr, sets the interpreters PC to final three bytes of opcode
        case 0x1000:
            chip8_set_PC(&chip8->registers, opcode & 0x0fff);
        break;

        // 2nnn CALL Addr, calls the subroutine specified by the address in nnn final three bytes of the opcode
        case 0x2000:
            chip8_set_PC(&chip8->registers, opcode & 0x0fff);
            chip8_set_SP(&chip8->registers, chip8->registers.SP+1);
            chip8->stack[chip8->registers.SP] = chip8->registers.PC;
        break;

        // 3xkk - SE Vx, byte, compares the register vX to kk and if they are equal skips next instruction. PC+=2
        case 0x3000:
            chip8_get_general_register(&chip8->registers, ((opcode >> 8) & 0x000f));
        break;
    }

}
#include "registers.h"
void chip8_set_general_register(struct chip8_registers* registers, int reg, char val)
{
    // Out of bounds!!!
    if (reg >= CHIP8_TOTAL_REGISTERS)
        return;
    registers->reg[reg] = val;
}


char chip8_get_general_register(struct chip8_registers* registers, int reg)
{
    // Out of bounds!
    if (reg >= CHIP8_TOTAL_REGISTERS)
        return 0;    
    
    return registers->reg[reg];
}

void chip8_set_delay_timer(struct chip8_registers* registers, char delay_timer)
{
    registers->delay_timer = delay_timer;
}
void chip8_set_sound_timer(struct chip8_registers* registers, char sound_timer)
{
    registers->sound_timer = sound_timer;
}

void chip8_set_I(struct chip8_registers* registers, unsigned short val)
{
    registers->I = val;
}

void chip8_set_PC(struct chip8_registers* registers, unsigned short val)
{
    registers->PC = val;
}

void chip8_set_SP(struct chip8_registers* registers, unsigned short val)
{
    registers->SP = val;
}
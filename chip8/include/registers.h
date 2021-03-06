#ifndef REGISTERS_H
#define REGISTERS_H
#include "config.h"
#include <stdbool.h>


struct chip8_registers
{
    unsigned char reg[CHIP8_TOTAL_REGISTERS];
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short I;
    unsigned short PC;
    unsigned char SP;
};

void chip8_set_general_register(struct chip8_registers* registers, int reg, char val);
unsigned char chip8_get_general_register(struct chip8_registers* registers, int reg);
void chip8_set_delay_timer(struct chip8_registers* registers, char delay_timer);
void chip8_set_sound_timer(struct chip8_registers* registers, char sound_timer);
void chip8_set_I(struct chip8_registers* registers, unsigned short val);
void chip8_set_PC(struct chip8_registers* registers, unsigned short val);
void chip8_set_SP(struct chip8_registers* registers, unsigned short val);

/**
 * Increments the program counter by two bytes
 */
void chip8_next_PC(struct chip8_registers* registers);
#endif
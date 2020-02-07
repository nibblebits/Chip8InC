#ifndef SIMULATOR_H
#define SIMULATOR_H
#include "config.h"

struct chip8;

void chip8_exec(struct chip8* chip8, unsigned short opcode);
#endif
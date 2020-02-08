#ifndef CMEMORY_H
#define CMEMORY_H

struct chip8;
unsigned short chip8_read_short(struct chip8* chip8, unsigned short addr);
unsigned char chip8_read_byte(struct chip8* chip8, unsigned short addr);

#endif
#include "simulator.h"
#include "emulator.h"
#include "registers.h"
#include <memory.h>
#include <time.h>
#include <stdlib.h>

static void chip8_exec_extended(struct chip8 *chip8, unsigned short opcode)
{
    unsigned char kk = 0;
    unsigned char x = 0;
    unsigned char y = 0;
    unsigned char n = 0;
    unsigned char reg_x_val = 0;
    unsigned char reg_y_val = 0;
    unsigned char vf = 0;
    unsigned char tmp = 0;

    unsigned short nnn = 0;
    unsigned short val = 0;
    unsigned short opcode_1st = opcode & 0xf000;
    unsigned short opcode_2nd = opcode & 0x000f;
    unsigned short prefix_opcode = opcode & 0x00ff;
    switch (opcode_1st)
    {
    // 1nnn JP Addr, sets the interpreters PC to final three bytes of opcode
    case 0x1000:
        chip8_set_PC(&chip8->registers, opcode & 0x0fff);
        break;

    // 2nnn CALL Addr, calls the subroutine specified by the address in nnn final three bytes of the opcode
    case 0x2000:
        chip8_set_PC(&chip8->registers, opcode & 0x0fff);
        chip8_set_SP(&chip8->registers, chip8->registers.SP + 1);
        chip8->stack[chip8->registers.SP] = chip8->registers.PC;
        break;

    // 3xkk - SE Vx, byte, compares the register vX to kk and if they are equal skips next instruction. PC+=2
    case 0x3000:
        reg_x_val = chip8_get_general_register(&chip8->registers, (opcode >> 8) & 0x000f);
        kk = opcode & 0x00ff;
        if (reg_x_val == kk)
        {
            chip8_next_PC(&chip8->registers);
        }

        break;

    // 4xkk - SNE Vx, byte compares the register vX to kk and if they are not equal skips next instruction. PC+=2
    case 0x4000:
        reg_x_val = chip8_get_general_register(&chip8->registers, (opcode >> 8) & 0x000f);
        kk = opcode & 0x00ff;
        if (reg_x_val != kk)
        {
            chip8_next_PC(&chip8->registers);
        }
        break;

    /* 5xy0 - SE Vx, Vy Skip next instruction if Vx = Vy.
    * While technically an ending zero is also required to represent this instruction, we will not check for it
    * as there is no conflicting instruction in the instruction set
    */
    case 0x5000:
        y = (opcode >> 4) & 0x000f;
        x = (opcode >> 8) & 0x000f;
        reg_x_val = chip8_get_general_register(&chip8->registers, x);
        reg_y_val = chip8_get_general_register(&chip8->registers, y);
        if (reg_x_val == reg_y_val)
        {
            chip8_next_PC(&chip8->registers);
        }
        break;

    //6xkk - LD Vx, byte, Set Vx = kk.
    case 0x6000:
        x = (opcode >> 8) & 0x000f;
        kk = opcode & 0x00ff;
        chip8_set_general_register(&chip8->registers, x, kk);
        break;

    // 7xkk - ADD Vx, byte, Set Vx = Vx + kk.
    case 0x7000:
        x = (opcode >> 8) & 0x000f;
        kk = opcode & 0x00ff;
        reg_x_val = chip8_get_general_register(&chip8->registers, x);
        chip8_set_general_register(&chip8->registers, x, reg_x_val + kk);
        break;

    // 8xyn Performs an operation on vX and vY depending on the "n"
    case 0x8000:
        y = (opcode >> 4) & 0x000f;
        x = (opcode >> 8) & 0x000f;
        reg_x_val = chip8_get_general_register(&chip8->registers, x);
        reg_y_val = chip8_get_general_register(&chip8->registers, y);
        switch (opcode_2nd)
        {
        // 8xy0 - LD Vx, Vy, load the value of vY into vX
        case 0x0000:
            chip8_set_general_register(&chip8->registers, x, reg_y_val);
            break;

        //8xy1 - OR Vx, Vy, Set Vx = Vx OR Vy.
        case 0x0001:
            chip8_set_general_register(&chip8->registers, x, reg_x_val | reg_y_val);
            break;

        //8xy2 - AND Vx, Vy, Set Vx = Vx AND Vy.
        case 0x0002:
            chip8_set_general_register(&chip8->registers, x, reg_x_val & reg_y_val);
            break;

        // 8xy3 - XOR Vx, Vy
        case 0x0003:
            chip8_set_general_register(&chip8->registers, x, reg_x_val ^ reg_y_val);
            break;
        // 8xy4 - ADD Vx, Vy, store result in Vx set VF to one if result greater than 8 bits 255 decimal, otherwise set to zero
        case 0x0004:
            val = reg_x_val + reg_y_val;
            vf = val > 0xff;
            chip8_set_general_register(&chip8->registers, x, val);
            chip8_set_general_register(&chip8->registers, 0x0f, vf);
            break;

        // 8xy5 - SUB Vx, Vy, Set Vx = Vx - Vy, set VF = NOT borrow.
        case 0x0005:
            val = reg_x_val - reg_y_val;
            vf = reg_x_val > reg_y_val;
            chip8_set_general_register(&chip8->registers, x, val);
            chip8_set_general_register(&chip8->registers, 0x0f, vf);
            break;

        // 8xy6 SHR Vx {, Vy} Set Vx = Vx SHR 1. If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
        case 0x0006:
            vf = reg_x_val & 0b00000001;
            val = reg_x_val / 2;
            chip8_set_general_register(&chip8->registers, 0x0f, val);
            break;

        //8xy7 - SUBN Vx, Vy, Set Vx = Vy - Vx, set VF = NOT borrow.
        case 0x0007:
            vf = reg_y_val > reg_x_val;
            val = reg_y_val - reg_x_val;
            chip8_set_general_register(&chip8->registers, x, val);
            break;

        // 8xyE - SHL Vx {, Vy} Set Vx = Vx SHL 1.,  If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
        case 0x000E:
            vf = reg_x_val & 0b10000000;
            val = reg_x_val * 2;
            chip8_set_general_register(&chip8->registers, x, val);
            break;
        }
        break;

    //9xy0 - SNE Vx, Vy, Skip next instruction if Vx != Vy., The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
    case 0x9000:
        y = (opcode >> 4) & 0x000f;
        x = (opcode >> 8) & 0x000f;
        reg_x_val = chip8_get_general_register(&chip8->registers, x);
        reg_y_val = chip8_get_general_register(&chip8->registers, y);
        if (reg_x_val != reg_y_val)
        {
            chip8_next_PC(&chip8->registers);
        }
        break;

    // Annn - LD I, addr, The value of register I is set to nnn.
    case 0xA000:
        chip8_set_I(&chip8->registers, opcode & 0x0fff);
        break;

    // Bnnn - JP V0, addr,  Jump to location nnn + V0.
    case 0xB000:
        val = chip8_get_general_register(&chip8->registers, 0);
        nnn = opcode & 0x0fff;
        chip8_set_PC(&chip8->registers, nnn + val);
        break;

    // Cxkk - RND Vx, byte, Set Vx = random byte AND kk.
    case 0xC000:
        kk = opcode & 0x00ff;
        x = (opcode >> 8) & 0x000f;
        srand(clock());
        val = (rand() % 255) & kk;
        chip8_set_general_register(&chip8->registers, x, val);
        break;

    /*
        * Dxyn - DRW Vx, Vy, nibble
        * Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
        * The interpreter reads n bytes from memory, starting at the address stored in I. 
        * These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.
        */
    case 0xD000:
    {
        y = (opcode >> 4) & 0x000f;
        x = (opcode >> 8) & 0x000f;
        n = opcode & 0x000f;
        reg_x_val = chip8_get_general_register(&chip8->registers, x);
        reg_y_val = chip8_get_general_register(&chip8->registers, y);
        bool sprite_hit =chip8_draw_sprite(chip8, reg_x_val, reg_y_val, chip8->registers.I, n);
        chip8_set_general_register(&chip8->registers, 0x0f, sprite_hit);
    }

    break;

    // Exnn
    case 0xE000:
    {
        x = (opcode >> 8) & 0x000f;
        reg_x_val = chip8_get_general_register(&chip8->registers, x);
        switch (prefix_opcode)
        {
        //Ex9E - SKP Vx, Skip next instruction if key with the value of Vx is pressed.
        case 0x009E:
            if (chip8_is_key_down(&chip8->keyboard, reg_x_val))
            {
                chip8_next_PC(&chip8->registers);
            }
            break;
        //ExA1 - SKNP Vx, Skip next instruction if key with the value of Vx is not pressed.
        case 0x00A1:
            if (!chip8_is_key_down(&chip8->keyboard, reg_x_val))
            {
                chip8_next_PC(&chip8->registers);
            }
            break;
        }
    }
    break;

    // Fxnn
    case 0xF000:
        x = (opcode >> 8) & 0x000f;
        reg_x_val = chip8_get_general_register(&chip8->registers, x);
        switch (prefix_opcode)
        {
        //Fx07 - LD Vx, DT, Set Vx = delay timer value.
        case 0x0007:
            chip8_set_general_register(&chip8->registers, x, chip8->registers.delay_timer);
            break;

        //Fx0A - LD Vx, K, Wait for a key press, store the value of the key in Vx.
        case 0x000A:
#warning Implement this later
            break;

        //Fx15 - LD DT, Vx, Set delay timer = Vx., DT is set equal to the value of Vx.
        case 0x0015:
            chip8_set_delay_timer(&chip8->registers, reg_x_val);
            break;

        // Fx18 - LD ST, Vx, Set sound timer = Vx.
        case 0x0018:
            chip8_set_sound_timer(&chip8->registers, reg_x_val);
            break;

        // Fx1E - ADD I, Vx, Set I = I + Vx.
        case 0x001E:
            chip8_set_I(&chip8->registers, chip8->registers.I + reg_x_val);
            break;

        //Fx29 - LD F, Vx, Set I = location of sprite for digit Vx.
        case 0x0029:
            chip8_set_I(&chip8->registers, reg_x_val * CHIP8_DEFAULT_CHARACTER_SET_BYTES_PER_CHARACTER);
            break;

        //Fx33 - LD B, Vx, Store BCD representation of Vx in memory locations I, I+1, and I+2., The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.
        case 0x0033:
        {
            unsigned char hundreds = reg_x_val / 100;
            unsigned char tens = reg_x_val / 10 % 10;
            unsigned char units = reg_x_val % 10;
            chip8->memory[chip8->registers.I] = hundreds;
            chip8->memory[chip8->registers.I + 1] = tens;
            chip8->memory[chip8->registers.I + 2] = units;
        }
        break;

        //Fx55 - LD [I], Vx, Store registers V0 through Vx in memory starting at location I., The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
        case 0x0055:
            for (int i = 0; i < reg_x_val; i++)
            {
#warning Change memory access to use a function to ensure we are in bounds! important!!
                tmp = chip8_get_general_register(&chip8->registers, i);
                chip8->memory[chip8->registers.I + i] = tmp;
            }

            break;

        //Fx65 - LD Vx, [I], Read registers V0 through Vx from memory starting at location I., The interpreter reads values from memory starting at location I into registers V0 through Vx.
        case 0x0065:
            for (int i = 0; i < reg_x_val; i++)
            {
                chip8_set_general_register(&chip8->registers, i, chip8->memory[chip8->registers.I + i]);
            }
            break;
        }

        break;
    }
}

void chip8_exec(struct chip8 *chip8, unsigned short opcode)
{

    switch (opcode)
    {
    // CLS, Clear the display
    case 0x00e0:
        chip8_clear_screen(&chip8->screen);
        break;

    // Ret, return from subroutine
    case 0x00ee:
        chip8_set_PC(&chip8->registers, chip8->stack[chip8->registers.SP]);
        chip8_set_SP(&chip8->registers, chip8->registers.SP - 1);
        break;

    default:
        chip8_exec_extended(chip8, opcode);
    };
}
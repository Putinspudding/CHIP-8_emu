#include <vector>
#include <stack>
#include <cstring>
#include <stdio.h>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <string>

typedef unsigned char BYTE;
typedef unsigned short int WORD;

BYTE memory[0xFFF];
BYTE registers[16];
WORD address_register_I;
WORD program_counter;
std::stack<WORD> stack;
BYTE screen_data[64][32];
BYTE input_data[16];

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

WORD keySelect()
{
}

void CPUReset()
{
    address_register_I = 0;
    program_counter = 0x200;
    memset(registers, 0, sizeof(registers)); // set registers to 0

    // load in the game
    FILE *in;
    in = fopen("c:/INVADERS", "rb");
    fread(&memory[0x200], 0xfff, 1, in);
    fclose(in);
}

WORD next_opcode()
{
    WORD res = 0;
    res = memory[program_counter];
    res <<= 8;
    res |= memory[program_counter + 1];
    program_counter += 2;
    return res;
}

void opcode_00EE(WORD opcode)
{
    program_counter = stack.top();
    stack.pop();
}

void opcode_1NNN(WORD opcode)
{
    program_counter = opcode & 0X0FFF;
}

void opcode_2NNN(WORD opcode)
{
    stack.push(program_counter);
    program_counter = opcode & 0x0FFF;
}

void opcode_3XNN(WORD opcode)
{
    if (registers[opcode & 0X0F00 >> 8] == opcode & 0X00FF)
        program_counter += 2;
}

void opcode_4XNN(WORD opcode)
{
    if (registers[opcode & 0X0F00 >> 8] != opcode & 0X00FF)
        program_counter += 2;
}

void opcode_5XY0(WORD opcode)
{
    if (registers[opcode & 0X0F00 >> 8] == registers[opcode & 0X00F0 >> 4])
        program_counter += 2;
}

void opcode_6XNN(WORD opcode)
{
    registers[opcode & 0X0F00 >> 8] = opcode & 0X00FF;
}

void opcode_7XNN(WORD opcode)
{
    registers[opcode & 0X0F00 >> 8] += opcode & 0X00FF;
}

void opcode_8XY0(WORD opcode)
{
    registers[opcode & 0X0F00 >> 8] = registers[opcode & 0X00F0 >> 4];
}

void opcode_8XY1(WORD opcode)
{
    registers[opcode & 0X0F00 >> 8] |= registers[opcode & 0X00F0 >> 4];
}

void opcode_8XY2(WORD opcode)
{
    registers[opcode & 0X0F00 >> 8] &= registers[opcode & 0X00F0 >> 4];
}

void opcode_8XY3(WORD opcode)
{
    registers[opcode & 0X0F00 >> 8] ^= registers[opcode & 0X00F0 >> 4];
}

void opcode_8XY4(WORD opcode)
{
    registers[opcode & 0X0F00 >> 8] += registers[opcode & 0X00F0 >> 4];
    registers[0xF] = registers[opcode & 0X0F00 >> 8] > registers[opcode & 0X00F0 >> 4];
}

void opcode_8XY5(WORD opcode)
{
    registers[0xF] = registers[opcode & 0X0F00 >> 8] >= registers[opcode & 0X00F0 >> 4];
    registers[opcode & 0X0F00 >> 8] -= registers[opcode & 0X00F0 >> 4];
}

void opcode_8XY6(WORD opcode)
{
    registers[0xF] = registers[opcode & 0X0F00 >> 8] & 0x1;
    registers[opcode & 0X0F00 >> 4] >>= 1;
}

void opcode_8XY7(WORD opcode)
{
    registers[0xF] = registers[opcode & 0X0F00 >> 8] <= registers[opcode & 0X00F0 >> 4];
    registers[opcode & 0X0F00 >> 8] = registers[opcode & 0X00F0 >> 4] - registers[opcode & 0X0F00 >> 8];
}

void opcode_8XYE(WORD opcode)
{
    registers[0xF] = registers[opcode & 0X0F00 >> 8] & 0x8000;
    registers[opcode & 0X0F00 >> 8] <<= 1;
}

void opcode_9XY0(WORD opcode)
{
    if (registers[opcode & 0X0F00 >> 8] != registers[opcode & 0X00F0 >> 4])
        program_counter += 2;
}

void opcode_ANNN(WORD opcode)
{
    address_register_I = opcode & 0X0FFF;
}

void opcode_BNNN(WORD opcode)
{
    program_counter = registers[0X0] + (opcode & 0X0FFF);
}

void opcode_CNNN(WORD opcode)
{
    registers[opcode & 0X0F00 >> 8] = rand() % 255 + (opcode & 0X00FF);
}

void opcode_DXYN(WORD opcode)
{
    WORD X = opcode & 0X0F00 >> 8;
    WORD Y = opcode & 0X00F0 >> 4;
    WORD height = opcode & 0X000F;
    registers[0XF] = 0;

    for (WORD h = 0; h < height; h++)
    {
        BYTE data = memory[address_register_I + h];
        int xpixelinv = 7;
        int xpixel = 0;
        for (xpixel = 0; xpixel < 8; xpixel++, xpixelinv--)
        {
            int mask = 1 << xpixelinv;
            if (data & mask)
            {
                int x = X + xpixel;
                int y = Y + h;
                if (screen_data[x][y] == 1)
                    registers[0xF] = 1;
                screen_data[x][y] ^= 1;
            }
        }
    }
}

void opcode_EX9E(WORD opcode)
{
    ;
}

void opcode_EXA1(WORD opcode)
{
    ;
}

void opcode_FX07(WORD opcode)
{
    ;
}

void opcode_FX0A(WORD opcode)
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        // User requests quit
        if (e.type == SDL_KEYDOWN)
        {
            break;
        }
    }
}

void opcode_FX15(WORD opcode)
{
    ;
}

void opcode_FX18(WORD opcode)
{
    ;
}

void opcode_FX1E(WORD opcode)
{
    address_register_I += registers[opcode & 0X0F00 >> 8];
}

void opcode_FX29(WORD opcode)
{
    ;
}

void opcode_FX33(WORD opcode)
{
    WORD X = opcode & 0X0F00 >> 8;

    WORD value = registers[X];

    WORD hundreds = value / 100;
    WORD tens = (value / 10) % 10;
    WORD units = value % 10;

    memory[address_register_I] = hundreds;
    memory[address_register_I + 1] = tens;
    memory[address_register_I + 2] = units;
}

void opcode_FX55(WORD opcode)
{
    WORD X = opcode & 0X0F00 >> 8;
    for (WORD i = 0; i <= X; i++)
    {
        memory[address_register_I + i] = registers[i];
    }
}

void opcode_FX65(WORD opcode)
{
    WORD X = opcode & 0X0F00 >> 8;
    for (WORD i = 0; i <= X; i++)
    {
        registers[i] = memory[address_register_I + i];
    }
}

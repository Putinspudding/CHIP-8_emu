#include "Chip8CPU.h"
#include <cstdlib>
#include <SDL2/SDL.h>
#include <iostream>
#include <string.h>

bool Chip8CPU::loadRom()
{
    FILE *in;
    char fileName[100];
    scanf("%s", fileName);
    in = fopen(fileName, "rb");
    // in = fopen("./pong2.c8", "rb");
    fread(&memory[0x200], 0xfff, 1, in);
    fclose(in);
    for (int i = 0; i < 32; ++i)
    {
        for (int j = 0; j < 64; ++j)
        {
            pixels[i][j] = 0x0;
        }
    }
    return true;
}

Chip8CPU::Chip8CPU()
{
    // pixels = new uint32_t[64 * 32];
}

bool Chip8CPU::reset()
{
    addressRegisterI = 0;
    programCounter = 0x200;
    memset(registers, 0, sizeof(registers)); // set registers to 0
    loadRom();
}

void Chip8CPU::decreaseTimers()
{
    if (delayTimer > 0)
    {
        delayTimer--;
    }
    if (delayTimer == 0)
    {
        delayTimer = 60;
    }

    if (soundTimer > 0)
        soundTimer--;

    if (soundTimer > 0)
        playBeep();
}

void Chip8CPU::playBeep()
{
}

bool Chip8CPU::executeNextOpcode()
{
    WORD opcode = nextOpcode();
    printf("0X%x\n----\n", opcode);
    // std::cout << opcode << "\n";
    switch (opcode & 0XF000)
    {
    case 0x0000:
        opcodeDecode0(opcode);
        break;
    case 0x1000:
        opcode_1NNN(opcode);
        break;
    case 0x2000:
        opcode_2NNN(opcode);
        break;
    case 0x3000:
        opcode_3XNN(opcode);
        break;
    case 0x4000:
        opcode_4XNN(opcode);
        break;
    case 0x5000:
        opcode_5XY0(opcode);
        break;
    case 0x6000:
        opcode_6XNN(opcode);
        break;
    case 0x7000:
        opcode_7XNN(opcode);
        break;
    case 0x8000:
        opcodeDecode8(opcode);
        break;
    case 0x9000:
        opcode_9XY0(opcode);
        break;
    case 0xA000:
        opcode_ANNN(opcode);
        break;
    case 0xB000:
        opcode_BNNN(opcode);
        break;
    case 0xC000:
        opcode_CXNN(opcode);
        break;
    case 0xD000:
        opcode_DXYN(opcode);
        break;
    case 0xE000:
        opcodeDecodeE(opcode);
        break;
    case 0xF000:
        opcodeDecodeF(opcode);
        break;

    default:
        break;
    }
}

WORD Chip8CPU::nextOpcode()
{
    WORD res = 0;
    res = memory[programCounter];
    res <<= 8;
    res |= memory[programCounter + 1];
    programCounter += 2;
    return res;
}

void Chip8CPU::opcodeDecode0(WORD opcode)
{
    switch (opcode)
    {
    case 0x00E0:
        opcode_00E0(opcode);
        break;
    case 0x00EE:
        opcode_00EE(opcode);
        break;
    default:
        break;
    }
}

void Chip8CPU::opcode_00E0(WORD opcode)
{
    for (int i = 0; i < 32; ++i)
        std::fill(pixels[i], pixels[i] + 64, 0);
};

void Chip8CPU::opcode_00EE(WORD opcode)
{
    programCounter = stackM.top();
    stackM.pop();
};

void Chip8CPU::opcode_1NNN(WORD opcode)
{
    WORD NNN = opcode & 0X0FFF;
    programCounter = NNN;
}

void Chip8CPU::opcode_2NNN(WORD opcode)
{
    WORD NNN = opcode & 0X0FFF;
    stackM.push(programCounter);
    programCounter = NNN;
}

void Chip8CPU::opcode_3XNN(WORD opcode)
{
    BYTE X = (opcode & 0X0F00) >> 8;
    WORD NN = opcode & 0X00FF;
    if (registers[X] == NN)
        programCounter += 2;
}

void Chip8CPU::opcode_4XNN(WORD opcode)
{
    BYTE X = (opcode & 0X0F00) >> 8;
    WORD NN = opcode & 0X00FF;
    if (registers[X] != NN)
        programCounter += 2;
}

void Chip8CPU::opcode_5XY0(WORD opcode)
{
    BYTE X = (opcode & 0X0F00) >> 8;
    BYTE Y = (opcode & 0X00F0) >> 4;
    if (registers[X] == registers[Y])
        programCounter += 2;
}

void Chip8CPU::opcode_6XNN(WORD opcode)
{
    BYTE X = (opcode & 0X0F00) >> 8;
    WORD NN = opcode & 0X00FF;
    registers[X] = NN;
}

void Chip8CPU::opcode_7XNN(WORD opcode)
{
    BYTE X = (opcode & 0X0F00) >> 8;
    WORD NN = opcode & 0X00FF;
    registers[X] += NN;
}

void Chip8CPU::opcodeDecode8(WORD opcode)
{
    switch (opcode & 0x000F)
    {
    case 0x0000:
        opcode_8XY0(opcode);
        break;
    case 0x0001:
        opcode_8XY1(opcode);
        break;
    case 0x0002:
        opcode_8XY2(opcode);
        break;
    case 0x0003:
        opcode_8XY3(opcode);
        break;
    case 0x0004:
        opcode_8XY4(opcode);
        break;
    case 0x0005:
        opcode_8XY5(opcode);
        break;
    case 0x0006:
        opcode_8XY6(opcode);
        break;
    case 0x0007:
        opcode_8XY7(opcode);
        break;
    case 0x000E:
        opcode_8XYE(opcode);
        break;
    }
}

void Chip8CPU::opcode_8XY0(WORD opcode)
{
    BYTE X = (opcode & 0X0F00) >> 8;
    BYTE Y = (opcode & 0X00F0) >> 4;
    registers[X] = registers[Y];
}

void Chip8CPU::opcode_8XY1(WORD opcode)
{
    BYTE X = (opcode & 0X0F00) >> 8;
    BYTE Y = (opcode & 0X00F0) >> 4;
    registers[X] |= registers[Y];
}

void Chip8CPU::opcode_8XY2(WORD opcode)
{
    BYTE X = (opcode & 0X0F00) >> 8;
    BYTE Y = (opcode & 0X00F0) >> 4;
    registers[X] &= registers[Y];
}

void Chip8CPU::opcode_8XY3(WORD opcode)
{
    BYTE X = (opcode & 0X0F00) >> 8;
    BYTE Y = (opcode & 0X00F0) >> 4;
    registers[X] ^= registers[Y];
}

void Chip8CPU::opcode_8XY4(WORD opcode)
{
    BYTE X = (opcode & 0X0F00) >> 8;
    BYTE Y = (opcode & 0X00F0) >> 4;
    unsigned int sum = registers[X] + registers[Y];
    if (sum > 0xFF)
    {
        registers[0xF] = 1;
    }
    else
    {
        registers[0xF] = 0;
    }
    registers[X] = sum & 0xFF;
}

void Chip8CPU::opcode_8XY5(WORD opcode)
{
    BYTE X = (opcode & 0X0F00) >> 8;
    BYTE Y = (opcode & 0X00F0) >> 4;
    registers[0xF] = registers[X] >= registers[Y];
    registers[X] -= registers[Y];
}

void Chip8CPU::opcode_8XY6(WORD opcode)
{
    BYTE X = (opcode & 0X0F00) >> 8;
    BYTE Y = (opcode & 0X00F0) >> 4;
    registers[0xF] = registers[X] & 0x1;
    registers[Y] >>= 1;
}

void Chip8CPU::opcode_8XY7(WORD opcode)
{
    BYTE X = (opcode & 0X0F00) >> 8;
    BYTE Y = (opcode & 0X00F0) >> 4;
    registers[0xF] = registers[X] <= registers[Y];
    registers[X] = registers[Y] - registers[X];
}

void Chip8CPU::opcode_8XYE(WORD opcode)
{
    BYTE X = (opcode & 0X0F00) >> 8;
    BYTE Y = (opcode & 0X00F0) >> 4;
    registers[0xF] = (registers[X] & 0x80) >> 7;
    registers[X] <<= 1;
}

void Chip8CPU::opcode_9XY0(WORD opcode)
{
    BYTE X = (opcode & 0X0F00) >> 8;
    BYTE Y = (opcode & 0X00F0) >> 4;
    if (registers[X] != registers[Y])
        programCounter += 2;
}

void Chip8CPU::opcode_ANNN(WORD opcode)
{
    WORD NNN = opcode & 0X0FFF;
    addressRegisterI = NNN;
}

void Chip8CPU::opcode_BNNN(WORD opcode)
{
    WORD NNN = opcode & 0X0FFF;
    programCounter = registers[0X0] + (NNN);
}

void Chip8CPU::opcode_CXNN(WORD opcode)
{
    BYTE X = (opcode & 0X0F00) >> 8;
    WORD NN = opcode & 0X00FF;
    registers[X] = rand() % 255 + (NN);
}

void Chip8CPU::opcode_DXYN(WORD opcode)
{
    BYTE X = (opcode & 0X0F00) >> 8;
    BYTE Y = (opcode & 0X00F0) >> 4;
    BYTE XCoord = registers[X];
    BYTE YCoord = registers[Y];
    BYTE height = opcode & 0X000F;
    registers[0XF] = 0;

    for (BYTE h = 0; h < height; h++)
    {
        BYTE data = memory[addressRegisterI + h];
        BYTE y = YCoord + h;
        for (BYTE x = 0; x < 8; x++)
        {

            BYTE xloc = x + XCoord;
            BYTE tempPixel = pixels[y][xloc] ? 1 : 0;
            BYTE tempBit = (data >> (7 - x)) & 0x1;
            if (tempPixel ^ tempBit == 1)
            {
                pixels[y][xloc] = 0xFFFFFF;
                // return;
            }
            else
            {
                registers[0xF] = 1;
            }
        }
    }
}

void Chip8CPU::opcodeDecodeE(WORD opcode)
{
    switch (opcode & 0x00FF)
    {
    case 0x009E:
        opcode_EX9E(opcode);
        break;
    case 0x00A1:
        opcode_EXA1(opcode);
        break;
    }
}

void Chip8CPU::opcode_EX9E(WORD opcode)
{
    BYTE key = getKeysLoop();
    if (key == registers[opcode & 0X0F00 >> 8])
    {
        programCounter += 2;
    }
}

void Chip8CPU::opcode_EXA1(WORD opcode)
{
    BYTE key = getKeysLoop();
    if (key != registers[opcode & 0X0F00 >> 8])
    {
        programCounter += 2;
    }
}

void Chip8CPU::opcodeDecodeF(WORD opcode)
{
    switch (opcode & 0x00FF)
    {
    case 0x0007:
        opcode_FX07(opcode);
        break;
    case 0x000A:
        opcode_FX0A(opcode);
        break;
    case 0x0015:
        opcode_FX15(opcode);
        break;
    case 0x0018:
        opcode_FX18(opcode);
        break;
    case 0x001E:
        opcode_FX1E(opcode);
        break;
    case 0x0029:
        opcode_FX29(opcode);
        break;
    case 0x0033:
        opcode_FX33(opcode);
        break;
    case 0x0055:
        opcode_FX55(opcode);
        break;
    case 0x0065:
        opcode_FX65(opcode);
        break;
    }
}

void Chip8CPU::opcode_FX07(WORD opcode)
{
    registers[opcode & 0X0F00 >> 8] = delayTimer;
}

void Chip8CPU::opcode_FX0A(WORD opcode)
{
    BYTE key = getKeysLoop();
    registers[opcode & 0X0F00 >> 8] = key;
}

void Chip8CPU::opcode_FX15(WORD opcode)
{
    delayTimer = registers[opcode & 0X0F00 >> 8];
}

void Chip8CPU::opcode_FX18(WORD opcode)
{
    soundTimer = registers[opcode & 0X0F00 >> 8];
}

void Chip8CPU::opcode_FX1E(WORD opcode)
{
    addressRegisterI += registers[opcode & 0X0F00 >> 8];
}

void Chip8CPU::opcode_FX29(WORD opcode)
{
    addressRegisterI = registers[opcode & 0x0F00 >> 8] * 5;
}

void Chip8CPU::opcode_FX33(WORD opcode)
{
    BYTE X = opcode & 0X0F00 >> 8;

    WORD value = registers[X];

    WORD hundreds = value / 100;
    WORD tens = (value / 10) % 10;
    WORD units = value % 10;

    memory[addressRegisterI] = hundreds;
    memory[addressRegisterI + 1] = tens;
    memory[addressRegisterI + 2] = units;
}

void Chip8CPU::opcode_FX55(WORD opcode)
{
    BYTE X = opcode & 0X0F00 >> 8;
    for (WORD i = 0; i <= X; i++)
    {
        memory[addressRegisterI + i] = registers[i];
    }
}

void Chip8CPU::opcode_FX65(WORD opcode)
{
    BYTE X = opcode & 0X0F00 >> 8;
    for (WORD i = 0; i <= X; i++)
    {
        registers[i] = memory[addressRegisterI + i];
    }
}

int getKey()
{
}

void Chip8CPU::test()
{
    printf("0X%x\n", programCounter);
}

KeyPress getKeysLoop()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_KEYDOWN)
        {
            // Select surfaces based on key press
            switch (e.key.keysym.sym)
            {
            case SDLK_0:
                return SDLK_PRESS_0;
                break;
            case SDLK_1:
                return SDLK_PRESS_1;
                break;
            case SDLK_2:
                return SDLK_PRESS_2;
                break;
            case SDLK_3:
                return SDLK_PRESS_3;
                break;
            case SDLK_4:
                return SDLK_PRESS_4;
                break;
            case SDLK_5:
                return SDLK_PRESS_5;
                break;
            case SDLK_6:
                return SDLK_PRESS_6;
                break;
            case SDLK_7:
                return SDLK_PRESS_7;
                break;
            case SDLK_8:
                return SDLK_PRESS_8;
                break;
            case SDLK_9:
                return SDLK_PRESS_9;
                break;
            case SDLK_a:
                return SDLK_PRESS_A;
                break;
            case SDLK_b:
                return SDLK_PRESS_B;
                break;
            case SDLK_c:
                return SDLK_PRESS_C;
                break;
            case SDLK_d:
                return SDLK_PRESS_D;
                break;
            case SDLK_e:
                return SDLK_PRESS_E;
                break;
            default:
                break;
            }
        }
    }
}
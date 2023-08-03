#pragma once
#include <stack>
#include <cstdint>
#include <unordered_map>
#include <SDL2/SDL.h>

typedef unsigned char BYTE;
typedef unsigned short int WORD;

const int ROMSIZE = 0xFFF;

const BYTE fontset[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

extern std::unordered_map<BYTE, SDL_Scancode> keysMapping;

class Chip8CPU
{
public:
    Chip8CPU();
    bool reset();
    bool loadRom();
    BYTE screenData[64][32];
    bool executeNextOpcode();
    void decreaseTimers();
    void test();
    uint32_t pixels[32][64];

private:
    WORD nextOpcode();
    void playBeep();
    void opcodeDecode0(WORD opcode);
    void opcode_00E0(WORD opcode);
    void opcode_00EE(WORD opcode);
    void opcode_1NNN(WORD opcode);
    void opcode_2NNN(WORD opcode);
    void opcode_3XNN(WORD opcode);
    void opcode_4XNN(WORD opcode);
    void opcode_5XY0(WORD opcode);
    void opcode_6XNN(WORD opcode);
    void opcode_7XNN(WORD opcode);
    void opcodeDecode8(WORD opcode);
    void opcode_8XY0(WORD opcode);
    void opcode_8XY1(WORD opcode);
    void opcode_8XY2(WORD opcode);
    void opcode_8XY3(WORD opcode);
    void opcode_8XY4(WORD opcode);
    void opcode_8XY5(WORD opcode);
    void opcode_8XY6(WORD opcode);
    void opcode_8XY7(WORD opcode);
    void opcode_8XYE(WORD opcode);
    void opcode_9XY0(WORD opcode);
    void opcode_ANNN(WORD opcode);
    void opcode_BNNN(WORD opcode);
    void opcode_CXNN(WORD opcode);
    void opcode_DXYN(WORD opcode);
    void opcodeDecodeE(WORD opcode);
    void opcode_EX9E(WORD opcode);
    void opcode_EXA1(WORD opcode);
    void opcodeDecodeF(WORD opcode);
    void opcode_FX07(WORD opcode);
    void opcode_FX0A(WORD opcode);
    void opcode_FX15(WORD opcode);
    void opcode_FX18(WORD opcode);
    void opcode_FX1E(WORD opcode);
    void opcode_FX29(WORD opcode);
    void opcode_FX33(WORD opcode);
    void opcode_FX55(WORD opcode);
    void opcode_FX65(WORD opcode);

    BYTE memory[0xFFF];
    BYTE registers[16];
    WORD addressRegisterI;
    WORD programCounter;
    std::stack<WORD> stackM;
    BYTE inputKey[16];
    BYTE delayTimer;
    BYTE soundTimer;
};

int getKey();
enum KeyPress
{
    SDLK_PRESS_0,
    SDLK_PRESS_1,
    SDLK_PRESS_2,
    SDLK_PRESS_3,
    SDLK_PRESS_4,
    SDLK_PRESS_5,
    SDLK_PRESS_6,
    SDLK_PRESS_7,
    SDLK_PRESS_8,
    SDLK_PRESS_9,
    SDLK_PRESS_A,
    SDLK_PRESS_B,
    SDLK_PRESS_C,
    SDLK_PRESS_D,
    SDLK_PRESS_E,
    SDLK_PRESS_F,
    SDLK_PRESS_NONE,

};
KeyPress getKeysLoop();

KeyPress getKeysNoLoop();
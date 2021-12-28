#ifndef REGISTERS_H
#define REGISTERS_H

//Screen dimension constants. These are for the original CHIP-8. Integer scaled.
#define SCREEN_WIDTH  64
#define SCREEN_HEIGHT 32

#define VX variableRegister[((opcode & 0x0F00) >> 8)]
#define VY variableRegister[((opcode & 0x00F0) >> 4)]
#define NNN (opcode & 0x0FFF)
#define NN (opcode & 0x00FF)

//Used for timing. Defined in hz
#define CLOCKSPEED 200

#include <stack.h>
#include <stdint.h>
#include <SDL.h>

class Chip8 {
  public:
    Chip8();
    virtual ~Chip8();

    uint16_t PC;
    uint16_t indexRegister;
    Stack    SP;
    uint8_t  delayTimer;
    uint8_t  soundTimer;
    uint8_t  variableRegister[16];



    /*  CHIP-8 includes a font for programs to utilize. This was declared in the interpreter's memory back in the day.
    */
    const unsigned char fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,		// 0
        0x20, 0x60, 0x20, 0x20, 0x70,		// 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0,		// 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0,		// 3
        0x90, 0x90, 0xF0, 0x10, 0x10,		// 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0,		// 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0,		// 6
        0xF0, 0x10, 0x20, 0x40, 0x40,		// 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0,		// 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0,		// 9
        0xF0, 0x90, 0xF0, 0x90, 0x90,		// A
        0xE0, 0x90, 0xE0, 0x90, 0xE0,		// B
        0xF0, 0x80, 0x80, 0x80, 0xF0,		// C
        0xE0, 0x90, 0x90, 0x90, 0xE0,		// D
        0xF0, 0x80, 0xF0, 0x80, 0xF0,		// E
        0xF0, 0x80, 0xF0, 0x80, 0x80		// F
    };

    uint8_t ROM[4096] {0};

    bool pixelsChanged;
    int  pixelBuffer[64][32] {{0}};

    bool isKeyPressed(uint8_t key, const uint8_t *state);
    void runCycle(const uint8_t *state, SDL_Event event);



  private:

};

#endif // REGISTERS_H

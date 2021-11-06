#include "chip8.h"

Chip8::Chip8() {

    PC = 0x200; //start location of program
    I  = 0;
    delayTimer = 0;
    soundTimer = 0;

    //hardcoded value, but I don't think the number of variable registers will change for this :)
    for(int i = 0; i < 16; i++) {
        variableRegister[i] = 0;
    }

    pixelsChanged = false;
}

Chip8::~Chip8() {
    //dtor
}

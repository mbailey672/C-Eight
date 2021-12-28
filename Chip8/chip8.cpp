#include "chip8.h"
#include <iostream>
#include <SDL.h>

Chip8::Chip8() {

    PC = 0x200; //start location of program
    indexRegister  = 0;
    delayTimer = 0;
    soundTimer = 0;

    //hardcoded value, but I don't think the number of variable registers will change for this :)
    for(int i = 0; i < 16; i++) {
        variableRegister[i] = 0;
    }

    //initializing the font set in memory. people seem to start this at 0x050, so that's what i'm doing as well
    for(int i = 0x050, j = 0; j < 80; i++, j++) {
        ROM[i] = fontset[j];
    }

    pixelsChanged = false;
}

Chip8::~Chip8() {
    //dtor
}
 
//Probably a better way to do this but alas
bool Chip8::isKeyPressed(uint8_t key, const uint8_t *state) {
    if(key == 0x1) {
        if(state[SDL_SCANCODE_1]) {
            return true;
        } else {
            return false;
        }
    } else if(key == 0x2) {
        if(state[SDL_SCANCODE_2]) {
            return true;
        } else {
            return false;
        }
    } else if(key == 0x3) {
        if(state[SDL_SCANCODE_3]) {
            return true;
        } else {
            return false;
        }
    } else if(key == 0xC) {
        if(state[SDL_SCANCODE_4]) {
            return true;
        } else {
            return false;
        }
    } else if(key == 0x4) {
        if(state[SDL_SCANCODE_Q]) {
            return true;
        } else {
            return false;
        }
    } else if(key == 0x5) {
        if(state[SDL_SCANCODE_W]) {
            return true;
        } else {
            return false;
        }
    } else if(key == 0x6) {
        if(state[SDL_SCANCODE_E]) {
            return true;
        } else {
            return false;
        }
    } else if(key == 0xD) {
        if(state[SDL_SCANCODE_R]) {
            return true;
        } else {
            return false;
        }
    } else if(key == 0x7) {
        if(state[SDL_SCANCODE_A]) {
            return true;
        } else {
            return false;
        }
    } else if(key == 0x8) {
        if(state[SDL_SCANCODE_S]) {
            return true;
        } else {
            return false;
        }
    } else if(key == 0x9) {
        if(state[SDL_SCANCODE_D]) {
            return true;
        } else {
            return false;
        }
    } else if(key == 0xE) {
        if(state[SDL_SCANCODE_F]) {
            return true;
        } else {
            return false;
        }
    } else if(key == 0xA) {
        if(state[SDL_SCANCODE_Z]) {
            return true;
        } else {
            return false;
        }
    } else if(key == 0x0) {
        if(state[SDL_SCANCODE_X]) {
            return true;
        } else {
            return false;
        }
    } else if(key == 0xB) {
        if(state[SDL_SCANCODE_C]) {
            return true;
        } else {
            return false;
        }
    } else if(key == 0xF) {
        if(state[SDL_SCANCODE_V]) {
            return true;
        } else {
            return false;
        }
    } 
    //physically impossible that this executes because I hardcoded a path for every value of key,
    //but i like the lack of compiler warnings
    throw;
}

void Chip8::runCycle(const uint8_t *state, SDL_Event event) {
    //used to address variableRegister by its proper name rather than a decimal representation
    //pretty sure this makes more sense.
    enum {V0, V1, V2, V3, V4, V5, V6, V7, V8, V9, VA, VB, VC, VD, VE, VF};


    int opcode = ROM[PC] << 8 | ROM[PC + 1];
    
    PC += 2; //IMPORTANT - program counter is being incremented after the FETCH phase
             //NOT THE DECODE PHASE!!

    switch(opcode & 0xF000) {
    case 0x0000: //If the first nibble == 0
        switch(opcode & 0x00FF) {
        case 0x00E0: //Opcode 00E0 - clear screen
            for(int i = 0; i < SCREEN_WIDTH; i++) {
                for(int j = 0; j < SCREEN_HEIGHT; j++) {
                    pixelBuffer[i][j] = 0;
                }
            }
            break;
        case 0x00EE: // Opcode 00EE - return from subroutine
            PC = SP.pop();
            break;
        default: //should only trigger for machine language routines.
            std::cout << "Unsupported opcode " << std::hex << opcode << "!";
            break;
        }
        break;
    case 0x1000: //opcode 1NNN - jump to address NNN
        PC = NNN;
        break;
    case 0x2000: //opcode 2NNN - start subroutine at NNN
        SP.push(PC);
        PC = NNN;
        break;
    case 0x3000: //opcode 3XNN - skip 1 instruction if VX == NN
        if(VX == NN) {
            PC += 2;
        } 
        break;
    case 0x4000: //opcode 4XNN - skip 1 instruction if VX != NN
        if(VX != NN) {
            PC += 2;
        }
        break;
    case 0x5000: //opcode 5XY0 - skips 1 instruction if VX == VY
        if(VX == VY) {
            PC += 2;
        }
        break;
    case 0x6000: //opcode 6XNN - Set register VX to NN
        VX = NN;
        break;
    case 0x7000: //opcode 7XNN - Add NN to VX
        VX += NN;
        break;
    case 0x8000: //if first nibble == 8
        switch(opcode & 0x000F) {
            case 0x0000: //opcode 8XY0 - set VX equal to VY
                VX = VY;
                break;
            case 0x0001: //opcode 8XY1 - set VX equal to logical OR of VX and VY
                VX = (VX | VY);
                break;
            case 0x0002: //opcode 8XY2 - set VX equal to logical AND of VX and VY
                VX = (VX & VY);
                break;
            case 0x0003: //opcode 8XY3 - set VX equal to logical XOR of VX and VY
                VX = (VX ^ VY);
                break;
            case 0x0004: { //opcode 8XY4 - set VX equal to the sum of VX and VY. Flip VF if result >255
                uint8_t temp = VX;
                VX = (VX + VY);
                if(temp > VX) {
                    variableRegister[VF] = 1;
                } else {
                    variableRegister[VF] = 0;
                }
                break;
            }
            case 0x0005: //opcode 8XY5 - set VX equal to VX - VY
                         //set VF if VX > VY
                if(VX > VY) {
                    variableRegister[VF] = 1;
                } else {
                    variableRegister[VF] = 0;
                }
                VX = (VX - VY);
                break;
            case 0x0006: {//opcode 8XY6 - set VX equal to VY, then bitshift 1 right
                          //set VF equal to the shifted bit
                //VX = VY;
                uint8_t lostBit = VX & 0b00000001;
                VX >>= 1;
                if(lostBit == 0b00000001) {
                    variableRegister[VF] = 1;
                } else {
                    variableRegister[VF] = 0;
                }
                break;
            }
            case 0x0007: //opcode 8XY7 - set VX equal to VY - VX
                         //set VF if VY > VX
                if(VY > VX) {
                    variableRegister[VF] = 1;
                } else {
                    variableRegister[VF] = 0;
                }
                VX = (VY - VX);
                break;
            case 0x000E: {//opcode 8XYE - set VX equal to VY, then bitshift 1 left
                          //set VF equal to the shifted bit
                //VX = VY;
                uint8_t lostBit = VX & 0b10000000;
                std::cout << +VX << " " << +lostBit <<  std::endl;
                if(lostBit == 0b10000000) {
                    variableRegister[VF] = 1;
                } else {
                    variableRegister[VF] = 0;
                }
                VX <<= 1;
                break;            
            }
        }
        break;
    case 0x9000: //opcode 9XYO - skips 1 instruction if VX != VY
        if(VX != VY) {
            PC += 2;
        }
        break;
    case 0xA000: //opcode ANNN - Set I to address NNN
        indexRegister = NNN;
        break;
    case 0xB000: //opcode BNNN - Jump to address NNN + V0
        PC = (NNN + variableRegister[V0]);
        break;
    case 0xC000: //opcode CXNN - generate a random number, logical AND with NN, and put result in VX
        VX = (rand() % 256) & NN;
        break;

    case 0xD000: { //opcode DXYN - Draw N tall sprite I at coordinates (VX, VY)
        variableRegister[VF] = 0;
        int height = (opcode & 0x000F);
        int xCoord = VX % 64;
        int yCoord = VY % 32;

        for(int i = 0; i < height && (yCoord + i) < 32; i++) {
            uint8_t currentRow = ROM[indexRegister + i];
            for(int j = 0; j < 8 && (xCoord + j) < 64; j++) {
                std::cout << (yCoord + i) << " " << (xCoord + j) << " " << (currentRow & (0x80 >> j)) << std::endl;
                if((currentRow & (0x80 >> j)) != 0) {
                    if(pixelBuffer[xCoord + j][yCoord + i] == 1) {
                        variableRegister[VF] = 1;
                    }
                    pixelBuffer[xCoord + j][yCoord + i] ^= 1;
                }
            }
        }

        pixelsChanged = true;
        break;
    } 
    case 0xE000:
        switch(opcode & 0x00FF) {
            case 0x009E: //opcode EX9E - skips instruction if the key in VX is held down
                if(isKeyPressed(VX, state)) {
                    PC += 2;
                }
                break;
            case 0x00A1: //opcode EXA1 - skips instruction if the key in VX is not held down
                if(!isKeyPressed(VX, state)) {
                    PC += 2;
                }
                break;
            default: //this should never happen.
                std::cout << "Unsupported opcode " << std::hex << "!";
                break;
        }
        break;
    case 0xF000:
        switch(opcode & 0x00FF) {
            case 0x0007: //opcode FX07 - set VX to value of delay timer
                VX = delayTimer;
                break;
            case 0x000A: {//opcode FX0A - wait for key input
                bool isDone = false;
                while(!isDone) {
                    if(SDL_WaitEvent(&event)) {
                        if(event.type == SDL_KEYDOWN) {
                            if(event.key.keysym.sym == SDLK_1) {
                                VX = 0x1;
                                break;
                            } else if(event.key.keysym.sym == SDLK_2) {
                                VX = 0x2;
                                break;
                            } else if(event.key.keysym.sym == SDLK_3) {
                                VX = 0x3;
                                break;
                            } else if(event.key.keysym.sym == SDLK_4) {
                                VX = 0xA;
                                break;
                            } else if(event.key.keysym.sym == SDLK_q) {
                                VX = 0x4;
                                break;
                            } else if(event.key.keysym.sym == SDLK_w) {
                                VX = 0x5;
                                break;
                            } else if(event.key.keysym.sym == SDLK_e) {
                                VX = 0x6;
                                break;
                            } else if(event.key.keysym.sym == SDLK_r) {
                                VX = 0xB;
                                break;
                            } else if(event.key.keysym.sym == SDLK_a) {
                                VX = 0x7;
                                break;
                            } else if(event.key.keysym.sym == SDLK_s) {
                                VX = 0x8;
                                break;
                            } else if(event.key.keysym.sym == SDLK_d) {
                                VX = 0x9;
                                break;
                            } else if(event.key.keysym.sym == SDLK_f) {
                                VX = 0xC;
                                break;
                            } else if(event.key.keysym.sym == SDLK_z) {
                                VX = 0xD;
                                break;
                            } else if(event.key.keysym.sym == SDLK_x) {
                                VX = 0x0;
                                break;
                            } else if(event.key.keysym.sym == SDLK_c) {
                                VX = 0xE;
                                break;
                            } else if(event.key.keysym.sym == SDLK_v) {
                                VX = 0xF;
                                break;
                            } 
                        } else if (event.type == SDL_QUIT) {
                            isDone = true;
                        }
                    }
                }
                break;
            }
            case 0x0015: //opcode FX15 - set delay timer to VX
                delayTimer = VX;
                break;
            case 0x0018: //opcode FX18 - set sound timer to VX
                soundTimer = VX;
                break;
            case 0x001E: //opcode FX1E - add VX to index register
                indexRegister += VX;
                if(indexRegister >= 0x1000) {
                    variableRegister[VF] = 1;
                }
                break;
            case 0x0029: //opcode FX29 - set index register to address of character in VX
                         //VX is 8 bit, so it holds two hex numbers. the character is the second character
                         //i.e. if VX=0xAF, the character pointed to would be F.
                indexRegister = 0x050 + (5 * (VX & 0x0F));
                break;
            case 0x0033: { //opcode FX33 - converts hex VX to decimal, and stores the decimal values at I, I+1, I+2
                uint8_t hundreds = VX / 100;        // if VX=0x90=144, 144/100==1 (integer division)
                uint8_t tens     = (VX % 100) / 10; // if VX=0x90=144, 144%100==44, and 44/10 = 4 (integer division)
                uint8_t ones     = VX % 10;         // if VX=0x90=144, 144%10==4
                
                ROM[indexRegister]     = hundreds;
                ROM[indexRegister + 1] = tens;
                ROM[indexRegister + 2] = ones;
                break;
            } 
            case 0x0055: {//opcode FX55 - store contents of variable registers in memory, starting from memory location indexRegister, from V0 to VX
                uint8_t X = ((opcode & 0x0F00) >> 8);
                for(int i = 0; i <= X; i++) {
                    ROM[indexRegister + i] = variableRegister[i];
                }
                break;
            }
            case 0x0065: {//opcode FX65 - load memory into variable registers, starting from memory location indexRegister, into V0 to VX
                uint8_t X = ((opcode & 0x0F00) >> 8);
                for(int i = 0; i <= X; i++) {
                    variableRegister[i] = ROM[indexRegister + i];
                }
                break;            
            }
            default: //this should never happen.
                std::cout << "Unsupported opcode " << std::hex << "!";
                break;
        }


        break;

    default: //this should never happen.
        std::cout << "Unsupported opcode " << std::hex << "!";
        break;
    }
}


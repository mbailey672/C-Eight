#include <SDL.h>
#include <iostream>
#include <stdint.h>
#include <fstream>
#include <iomanip>

#include <stack.h>
#include <chip8.h>

//Screen dimension constants. These are for the original CHIP-8. Integer scaled.
#define SCREEN_WIDTH  64
#define SCREEN_HEIGHT 32

#define VX chip8System.variableRegister[((opcode & 0x0F00) >> 8)]
#define VY chip8System.variableRegister[((opcode & 0x00F0) >> 4)]
#define NNN (opcode & 0x0FFF)
#define NN (opcode & 0x00FF)

//Allows for integer scaling of display
const int WIDTH_SCALE_FACTOR  = 16;
const int HEIGHT_SCALE_FACTOR = 16;


//Global because lazy
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

//Function headers
void playGame(Chip8 &chip8System);
void loadROM(Chip8 &chip8System, std::string romFilename);
void runCycle(Chip8 &chip8System);

int main(int argc, char* args[]) {

    //Initialize CHIP-8 system
    Chip8 chip8System;

    //Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL initialization failed - Error " << SDL_GetError() << std::endl;
    }
    //Create window
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH * WIDTH_SCALE_FACTOR, SCREEN_HEIGHT * HEIGHT_SCALE_FACTOR, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, WIDTH_SCALE_FACTOR, HEIGHT_SCALE_FACTOR);

    bool isDone = false; //main control flag
    SDL_Event event; //event stack

    loadROM(chip8System, "IBM Logo.ch8");
    std::cout << chip8System.ROM[chip8System.PC] << std::endl;

    playGame(chip8System);

    //Destroy window
    SDL_DestroyWindow(window);

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}

void playGame(Chip8 &chip8System) {
    int numTicks = SDL_GetTicks();
    int lastTickCheck = 0;
    while(1) {
        runCycle(chip8System);
        if(chip8System.pixelsChanged) {
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

            for (int i = 0; i < SCREEN_WIDTH; i++) {
                for (int j = 0; j < SCREEN_HEIGHT; j++) {
                    if (chip8System.pixelBuffer[i][j] == 1)
                        SDL_RenderDrawPoint(renderer, i, j);
                }
            }

            SDL_RenderPresent(renderer);
            chip8System.pixelsChanged = false;
        }
        //userInput here
        
    /*do {
        //main loop for event polling.
        while(SDL_PollEvent(&event) != 0) {
            SDL_RenderClear(renderer);
            switch(event.type) {
            //exits program in event of X press, cmd+Q, etc.
            case SDL_QUIT:
                isDone = true;
                break;
                //handles keypress events
            }

        }
    } while(!isDone);*/

    }

}

void runCycle(Chip8 &chip8System) {
    //used to address variableRegister by its proper name rather than a decimal representation
    //pretty sure this makes more sense.
    enum {V0, V1, V2, V3, V4, V5, V6, V7, V8, V9, VA, VB, VC, VD, VE, VF};


    int opcode = chip8System.ROM[chip8System.PC] << 8 | chip8System.ROM[chip8System.PC + 1];

    switch(opcode & 0xF000) {
    case 0x0000:
        switch(opcode & 0x00FF) {
        case 0x00E0: //clear screen
            for(int i = 0; i < SCREEN_WIDTH; i++) {
                for(int j = 0; j < SCREEN_HEIGHT; j++) {
                    chip8System.pixelBuffer[i][j] = 0;
                }
            }
            chip8System.PC += 2;
            break;
        case 0x00EE: //return from subroutine
            chip8System.PC = chip8System.SP.pop();
            break;
        default: //should only trigger for machine language routines.
            std::cout << "Unsupported opcode " << std::hex << "!";
            chip8System.PC += 2;
            break;
        }
        break;
    case 0x1000: //jump to address NNN
        chip8System.PC = NNN;
        break;
    case 0x6000: //Set register VX to NN
        VX = NN;
        chip8System.PC += 2;
        break;
    case 0x7000: //Add NN to VX
        VX += NN;
        chip8System.PC += 2;
        break;
    case 0xA000: //Set I to address NNN
        chip8System.I = NNN;
        chip8System.PC += 2;
        break;
    case 0xD000: { //Draw N tall sprite I at coordinates (VX, VY)
        chip8System.variableRegister[VF] = 0;
        int height = (opcode & 0x000F);
        int xCoord = VX % 64;
        int yCoord = VY % 32;

        for(int i = 0; i < height && (yCoord + i) < 32; i++) {
            uint8_t currentRow = chip8System.ROM[chip8System.I + i];
            for(int j = 0; j < 8 && (xCoord + j) < 64; j++) {
                std::cout << (yCoord + i) << " " << (xCoord + j) << " " << (currentRow & (0x80 >> j)) << std::endl;
                if((currentRow & (0x80 >> j)) != 0) {
                    if(chip8System.pixelBuffer[xCoord + j][yCoord + i] == 1) {
                        chip8System.variableRegister[VF] = 1;
                    }
                    chip8System.pixelBuffer[xCoord + j][yCoord + i] ^= 1;
                }
            }
        }

        chip8System.pixelsChanged = true;
        chip8System.PC += 2;
        break;
    } default: //this should never happen.
        std::cout << "Unsupported opcode " << std::hex << "!";
        break;
    }
}

void loadROM(Chip8 &chip8System, std::string romFilename) {
    std::ifstream in(romFilename, std::ios::binary);
    for(int i = 0x200; !in.eof(); i++) {
        chip8System.ROM[i] = in.get();
    }
}
/*
case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                case SDLK_RETURN: {
                    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                    SDL_RenderDrawPoint(renderer, 1, 1);
                    //SDL_RenderDrawRect(renderer, &pixel);
                    //SDL_RenderFillRect(renderer, &pixel);
                    //SDL_RenderPresent(renderer);
                    break;
                }
                case SDLK_SPACE: {
                    SDL_SetRenderDrawColor(renderer, 0xAF, 0xAF, 0xAF, 0xFF);
                    SDL_RenderDrawPoint(renderer, 1, 1);
                    SDL_RenderDrawRect(renderer, &pixel);
                    SDL_RenderFillRect(renderer, &pixel);
                    SDL_RenderPresent(renderer);
                    break;
                }

                }
                break;
*/

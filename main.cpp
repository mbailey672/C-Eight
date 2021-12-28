#include <SDL.h>
#include <iostream>
#include <stdint.h>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <chrono>
#include <thread>

#include <stack.h>
#include <chip8.h>

//sorry, no VFR or high refresh rate support :)
#define REFRESH_RATE 60

//Allows for integer scaling of display
const int WIDTH_SCALE_FACTOR  = 16;
const int HEIGHT_SCALE_FACTOR = 16;

//Global because lazy
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

//Function headers
void playGame(Chip8 &chip8System, SDL_Event event);
void loadROM(Chip8 &chip8System, std::string romFilename);

int main(int argc, char* args[]) {

    //Initialize CHIP-8 system
    Chip8 chip8System;

    //Seed random number generator
    srand(time(NULL));

    //Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL initialization failed - Error " << SDL_GetError() << std::endl;
    }
    //Create window
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH * WIDTH_SCALE_FACTOR, SCREEN_HEIGHT * HEIGHT_SCALE_FACTOR, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, WIDTH_SCALE_FACTOR, HEIGHT_SCALE_FACTOR);

    bool isDone = false; //main control flag
    SDL_Event event; //event stack

    loadROM(chip8System, "INVADERS.ch8");
    std::cout << chip8System.ROM[chip8System.PC] << std::endl;

    playGame(chip8System, event);

    //Destroy window
    SDL_DestroyWindow(window);

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}

void playGame(Chip8 &chip8System, SDL_Event event) {
    int numTicks = SDL_GetTicks();
    int lastTickCheck = 0;
    bool isDone = false;

    const uint8_t *keyStates = SDL_GetKeyboardState(NULL);

    while(!isDone) {
        std::chrono::high_resolution_clock::time_point beginTime = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < 8; i++) {
            chip8System.runCycle(keyStates, event);
        }
        //Decrement the delay timer
        if(chip8System.delayTimer != 0) {
            chip8System.delayTimer--;
        }
        //Decrement the sound timer
        if(chip8System.soundTimer != 0) {
            chip8System.soundTimer--;
        }

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

        //main loop for event polling.
        while(SDL_PollEvent(&event) != 0) {
            SDL_RenderClear(renderer);
            switch(event.type) {
            //handles keypress events

            //exits program in event of X press, cmd+Q, etc.
            case SDL_QUIT:
                isDone = true;
                break;
            }

        }
        std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> elapsedTime = endTime - beginTime;
        std::this_thread::sleep_for(std::chrono::microseconds(16666) - elapsedTime);

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

## C-Eight

A CHIP-8 interpreter written in C++, for learning about computer architecture, bitshift operations, and graphics. I'm also taking the opportunity to use writting this "emulator" as a learning experience for the other, more complex emulator I'm working on: [goboy!](https://github.com/mbailey672/goboy) So far, I've implemented enough instructions to boot and run the famous IBM logo program for the CHIP-8 (I believe this is public domain and as such I've included it in this repo)

## Some interesting features

- I've written a custom (albiet basic) stack implementation for the SP of the CHIP-8 system. This was far more fun then using `std::stack`!
- Graphics are currently handled using SDL 2 and it's support for hardware accelerated rendering. Not exactly necessary for a system as basic as this, but alas. In the future, I'm also intending to handle user input and audio using SDL 2 as well.
- I've written a few macros that are used throughout the program to simplify extracting important data from opcodes. This is pretty different to my other experience with emudev, but it's recommended to do this for the CHIP-8 and has worked fantastically for me.

## Remaining features to be implemented

# Note - this interpreter is very much still a work in progress. I pushed hard to have it be somewhat functional so I could publish this repository and call it a "project" in good faith, but there is still lots of work to be done!

- More opcodes need to be implemented
- Both sound and delay timers need to be implemented
- User input needs to be added to the main gameplay loop (currently this is commented out while I bugfix + research the best way to do this)
- Multi-threading support?
- Built in debugger?

## Images

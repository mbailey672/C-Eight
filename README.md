# C-Eight

A CHIP-8 interpreter written in C++, for learning about computer architecture, bitshift operations, and graphics. I'm also taking the opportunity to use writting this "emulator" as a learning experience for the other, more complex emulator I'm working on: [goboy!](https://github.com/mbailey672/goboy) So far, I've implemented enough instructions to boot and run the famous IBM logo program for the CHIP-8 (I believe this is public domain and as such I've included it in this repo)

# Some interesting features

- I've written a custom (albiet basic) stack implementation for the SP of the CHIP-8 system. This was far more fun then using `std::stack`!
- Graphics are currently handled using SDL 2 and it's support for hardware accelerated rendering. Not exactly necessary for a system as basic as this, but alas. In the future, I'm also intending to handle user input and audio using SDL 2 as well.
- I've written a few macros that are used throughout the program to simplify extracting important data from opcodes. This is pretty different to my other experience with emudev, but it's recommended to do this for the CHIP-8 and has worked fantastically for me.

# Remaining features to be implemented

- I've implemented all the necessary pieces for sound, except the code to actually generate sound. I'm not sure which ROMs I can test this with.
- Built in visual debugger?

# Images
The extremely impressive first program I ran. This is IBM Logo.ch8:
![The extremely impressive first program I ran](https://github.com/mbailey672/C-Eight/blob/main/images/IBM%20Logo.png)
The title screen for Space Invaders. This version is not quite as cool as the Midways cabinets are:
![Space Invaders!](https://github.com/mbailey672/C-Eight/blob/main/images/space%20invaders.png)
BC_test.ch8 is a test ROM designed by the community that tests each opcode. It spits out errors if there are problems, and says "BON!" if there aren't. Seeing this was very exciting:
![Passing BC Test, a ROM designed to ensure correct opcode functionality](https://github.com/mbailey672/C-Eight/blob/main/images/passing%20BC%20test.png)
Playing a game of Pong:
![Playing Pong](https://github.com/mbailey672/C-Eight/blob/main/images/pong.png)


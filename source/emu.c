#include <stdio.h>
#include <emu.h>
#include <cart.h>
#include <cpu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

//emulator core components
// CART , you have to load the CART
// read data and write data to CART

// CPU , main thing we are emulating
// emulate instructions and registers

// Address Bus reading and writing to address, they are mem mapped

// PPU pixel processing unit 
//responsible for writing pixels to the screen

// Timer process for the emulator and device in general

// creating static object for our emulator context. 
static emu_context con;

emu_context * emu_get_context()
{
    return &con;
}

//delay function 
// for creating a delay based on SDL function
void delay(u32 ms)
{
    SDL_Delay(ms);
}

int emu_run(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage: emu <rom_file>\n");
        return -1;
    }

    if(!cart_load(argv[1]))
    {
        printf("Failed to load ROM file: %s\n", argv[1]);
    }

    printf("cart loaded \n");

    SDL_Init(SDL_INIT_VIDEO);
    printf("SDL INITIATED \n");
    TTF_Init();
    printf("TTF INITIATED \n");

    cpu_init();

    con.running = true;
    con.paused = false;
    con.ticks = 0;

    while(con.running)
    {
        if(con.paused)
        {
            delay(10);
            continue;
        }

        if(!cpu_step())
        {
            printf("CPU stopped \n");
            return -3;
        }

        con.ticks++;
    }

    return 0;
}

void emu_cycles(int cpu_cycles)
{
    //todo   
}



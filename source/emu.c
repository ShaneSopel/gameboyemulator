#include <stdio.h>
#include <emu.h>
#include <timer.h>
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
//void delay(u32 ms)
//{
//    SDL_Delay(ms);
//}

void *cpu_run(void *p)
{
    timer_init();
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
            return 0;
        }

        //con.ticks++;
    }

    return 0;
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

    ui_init();

    pthread_t t1;
    if (pthread_create(&t1, NULL, cpu_run, NULL))
    {
        fprintf(stderr, "FAILED TO START MAIN CPU THREAD!\n");
        return -1;
    }

    while (!con.die)
    {
        usleep(1000);
        ui_handle_events();

        ui_update();
    }

    return 0;
}

void emu_cycles(int cpu_cycles)
{
    for (int i=0; i<cpu_cycles; i++)
    {
        for(int n; n<4; n++)
        {
            timer_tick();
        }

        dma_tick();
    }
}



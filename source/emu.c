#include <stdio.h>
#include <emu.h>

//TODO Add Windows Alternative...
#include <pthread.h>
#include <unistd.h>

/* 
  Emu components:

  |Cart|
  |CPU|
  |Address Bus|
  |PPU|
  |Timer|

*/

static emu_context con;

emu_context *emu_get_context() 
{
    return &con;
}

void *cpu_run(void *p) {
    apu_init();
    timer_init();
    cpu_init();
    ppu_init();

    con.running = true;
    con.paused = false;
    con.ticks = 0;

    while(con.running) {
        if (con.paused) {
            delay(10);
            continue;
        }

        if (!cpu_step()) {
            printf("CPU Stopped\n");
            return 0;
        }
    }

    return 0;
}

int emu_run(int argc, char **argv) 
{
    if (argc < 2) {
        printf("Usage: emu <rom_file>\n");
        return -1;
    }

    if (!cart_load(argv[1])) {
        printf("Failed to load ROM file: %s\n", argv[1]);
        return -2;
    }

    printf("Cart loaded..\n");

    ui_init();
    
    pthread_t t1;

    if (pthread_create(&t1, NULL, cpu_run, NULL)) 
    {
        fprintf(stderr, "FAILED TO START MAIN CPU THREAD!\n");
        return -1;
    }

    u32 prev_frame = 0;

    while(!con.die) 
    {
        usleep(1000);
        ui_handle_events();

        if (prev_frame != ppu_get_context()->current_frame) 
        {
            ui_update();
            apu_update();
        }

        prev_frame = ppu_get_context()->current_frame;
    }

    return 0;
}

void emu_cycles(int cpu_cycles) {
    
    for (int i=0; i<cpu_cycles; i++) 
    {
        for (int n=0; n<4; n++) {
            con.ticks++;
            timer_tick();
            ppu_tick();
    }

        dma_tick();
    }
}

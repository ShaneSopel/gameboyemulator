#ifndef EMU_H
#define EMU_H

#include <apu.h>
#include <common.h>
#include <cart.h>
#include <cpu.h>
#include <dma.h>
#include <ppu.h>
#include <ui.h>

#include <pthread.h>
#include <unistd.h>

typedef struct {
    bool paused;
    bool running;
    bool die;
    u64 ticks;
} emu_context;

int emu_run(int argc, char **argv);

emu_context *emu_get_context();

void emu_cycles(int cpu_cycles);

#endif

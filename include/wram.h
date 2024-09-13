#ifndef WRAM_H
#define WRAM_H

#include <common.h>

void wram_write(u16 address, u8 value);
u8 wram_read(u16 address);

u8 WRAMBank = 1;
u8 RAM[0x8000] = {0};

#endif
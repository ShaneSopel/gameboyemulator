#ifndef BUS_H
#define BUS_H

#include <cpu.h>
#include <common.h>
#include <cart.h>
#include <ram.h>
#include <io.h>

// 8 bit data bus
// 16 bit address bus
//  pass in a 16 bit address get an 8 bit data back.
u8 bus_read(u16 address);
void bus_write(u16 address, u8 value);

u16 bus_read16(u16 address);
void bus_write16(u16 address, u16 value);

#endif
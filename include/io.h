#ifndef IO_H
#define IO_H

#include <common.h>
#include <cpu.h>
#include <channel1.h>
#include <channel2.h>
#include <channel3.h>
#include <channel4.h>
#include <dma.h>
#include <gamepad.h>
#include <lcd.h>
#include <timer.h>

u8 io_read(u16 address);
void io_write(u16 address, u8 value);

#endif
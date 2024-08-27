#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <common.h>
#include <cpu.h>
#include <stack.h>

typedef enum
{
    IT_VBLANK = 1,
    IT_LCD_STAT = 2,
    IT_TIMER = 4,
    IT_SERIAL = 8,
    IT_JOYPAD = 16
} interrupt_type;

void int_handle(cpu_context *con, u16 address);

bool int_check(cpu_context *con, u16 address, interrupt_type t);

void cpu_request_interrupt(interrupt_type t);

void cpu_handle_interrupts(cpu_context *con);

#endif
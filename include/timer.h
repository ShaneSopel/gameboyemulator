#ifndef TIMER_H
#define TIMER_H

#include <common.h>

//FF04 DIV divider register
// this register is incremented at rate of 16384 Hz.
// writing any value to this register resets it to 0x00.
// additional this reg is reset when stop instruction , 
// and only begins ticking again once stop is ended

// FF05 TIMA timer counter
// this timer is incremeted at the clock freq specififed by 
// tac register FF07. when the value overfloweds (exceeds 0xFF)
// it resets to the value specied by TMA FF06. and an interrupt
// is requested.

// FF06 TMA timer modulo
// when TIMA Overflowed it is reset to the value in this reg
// and an interrupt is requested at the clock freq selected in TAC
// if TMA is set to 0xFE an interrupt is only requested every two increments
// which divides teh selected clock by two
// if TMA is set to 0xFD would divde the clock by three.

//FF07 TAC Timer controller
//TAC 7 6 5 4 3   2       1   0
//               Enable   Clock Select
// Enable : controls Whether TIMA is incremented
// note: DIV is always counting, regardless of this bit.
// Clock Select: Controls the freq at which TIMA is incremented.
// clock elect 00 is 4096 or 4194 Hz
// 01 is 262144 or 268400 Hz
// 10 is 65526 or 67110 Hz
// 11 is 16384 or 16780 Hz

typedef struct 
{
    u16 div;
    u8 tima;
    u8 tma;
    u8 tac;
}timer_context;

void timer_init();
void timer_tick();

void timer_write(u16 address, u8 value);
u8 timer_read(u16 address);

timer_context *timer_get_context();

#endif
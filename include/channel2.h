#ifndef CHANNEL2_H
#define CHANNEL2_H

#include <apu.h>
#include <common.h>

//FF16 - NR11: Channel 1 Length timer and duty cycle
// 76 wave duty
// 54321 Intial length Timer

//FF16 - NR12 Channel 1 volume and evelope
//7654 Initial volume 
// 3 Env dir
// 210 sweep pace

// FF18 - NR13 channel 1 peroid low (write only)

//FF19 - NR14 channel 1 peroid high control
// 7 Trigger (write only)
// 6 Length Enable
// 543 empty
//210 lower hald of ff13

void channel2_write(u16 address, u8 value);
u8 channel2_read(u16 address);

void set_pulse();
void pulse_calculate();

u8 get_duty_cycle();
void set_duty_cycle(u8 value);

void set_pulse_length_timer();

void set_pulse_trigger();

#endif


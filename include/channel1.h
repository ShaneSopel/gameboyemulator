#ifndef CHANNEL1_H
#define CHANNEL1_H

#include <apu.h>
#include <common.h>

//FF10 - NR10 Channel 1 sweep
// 654 PACE
// 3 Direction
// 210 Individual Step

//FF11 - NR11: Channel 1 Length timer and duty cycle
// 76 wave duty
// 54321 Intial length Timer

//FF12 - NR12 Channel 1 volume and evelope
//7654 Initial volume 
// 3 Env dir
// 210 sweep pace

// FF13 - NR13 channel 1 peroid low (write only)

//FF14 - NR14 channel 1 peroid high control
// 7 Trigger (write only)
// 6 Length Enable
// 543 empty
//210 lower hald of ff13

void channel1_write(u16 address, u8 value);
u8 channel1_read(u16 address);

void set_sweep();
void sweep_calculate();

u8 get_sweep_duty_cycle();
void set_sweep_duty_cycle(u8 value);

void set_sweep_length_timer();

void set_sweep_trigger();


#endif 
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


//const bool dutyTable[4][8];
/**= 
{
		{false, false, false, false, false, false, false, true},
		{true, false, false, false, false, false, false, true},
		{true, false, false, false, false, true, true, true},
		{false, true, true, true, true, true, true, false}
};*/

typedef struct
{
    bool envelope_running;
    int envelope_period;
    u8 volume;
    int sweep_count;
    bool sweep_enable;
    u16 sweep_shadow;
    int sweep_timer;
    bool enabled;
    bool dacEnabled;
    int timer;

    unsigned int outputVol;
	unsigned int sequencePointer;

} channel1;

void channel1_write(u16 address, u8 value);
u8 channel1_read(u16 address);

void channel1_init();

void set_sweep_env();
u8 get_sweep_volume();
void sweep_step();

void set_sweep_clk();
u16 sweep_calculate();

bool sweep_get_running();

void set_sweep_length_timer();

void set_sweep_trigger();


#endif 
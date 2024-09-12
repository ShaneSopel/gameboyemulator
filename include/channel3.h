#ifndef CHANNEL3_H
#define CHANNEL3_H

#include <apu.h>
#include <common.h>

// sound channel 3 wave output

// 0xFF1A - NR30 Channel 3 DAC Enable
// NR30 bit 7 DAC on and off 
// 6 5 4 3 2 1 0 has no data.

//0xFF1B Initial length timer 
// NR31 bits 7 - 0 (write only)

//0xFF1C Channel 3 output level 
// NR32 bit 6-5 output level
// all other bits are not used.

//(output level)
// 00 Mute 
// 01 100% volume 
// 10 50% volume
// 11 25% volume

//0xFF1D channel 3 peroid low (write only)
// same as channel 1 and 2 as far as 11 bits being stored and the rest are stored in
// high in FF1E

//0xFF1E trigger, length enable, period
// bit 7 - trigger
// bit 6 - length enable
// bit 2 1 0 - period

void channel3_write(u16 address, u8 value);
u8 channel3_read(u16 address);

void set_wave_length_timer();

void set_wave_trigger();

#endif
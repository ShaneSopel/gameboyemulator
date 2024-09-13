#ifndef CHANNEL4_H
#define CHANNEL4_H

#include <apu.h>
#include <common.h>

//Channel 4 noise
// FF20 - NR41 Channel 4 length timer 
// 0 1 2 3 4 5 initial length timer

// FF21 - NR42 Channel 4 volume & evelope
// same as NR12

// FF22 - NR43 channel 4 freq & randomness
// 7 6 5 4 clock shift
// 3 LSFR width
// 2 1 0 clock divider

// FF23 - NR44 channel 4 control
// 7 tigger
// 6 length enable

void channel4_write(u16 address, u8 value);
u8 channel4_read(u16 address);

#endif
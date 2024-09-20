#ifndef APU_H
#define APU_H

#include <common.h>
#include <SDL2/SDL.h>
#include <channel1.h>

#define samplesize 4096 

//NRx0 is some channel specific feature
//NRx1 controls the length of timer
//NRx2 controls the volume and evelope
//NRx3 controls the period (maybe only partially)
//NRx4 has the channel's trigger and length timer enable bits, as well as any leftover bits of period

//FF26 - NR52: Audio master control
//7                654                 3             2      1
//  audio on/off                       CH4 on          CH2 on CH1 on

//FF25 - NR51: sound panning
// 7               6        5       4       3        2         1   0
// CH4 left       CH3 left ....basically all the audio channels stero left and right 
// setting the bit to one enables the channel to go into the selected output

//FF24 - NR50: Master volume and VIN Panning
// 7 VIN left - set to zero if external sound hardware is not being used
//6 5 4 Left Volume
// 3 VIN Right -set to zero if external sound hardware is not being used
//2 1 0 Right Volume
typedef struct 
{

    //FF26
    bool audio_on;
    bool CH1_on;
    bool CH3_on;
    bool CH2_on;
    bool CH4_on;

    //FF25
    bool CH4_left;
    bool CH3_left;
    bool CH2_left;
    bool CH1_left;
    bool CH4_right;
    bool CH3_right;
    bool CH2_right;
    bool CH1_right;

    //FF24
    bool VIN_left;
    bool VIN_right;
    u8 left_volume;
    u8 right_volume;

    //channel 1 values
    //FF10
    bool sweep_direction;
    u8 sweep_pace;
    u8 sweep_individual_step;

    //FF11
    u8 sweep_duty_cycle;
    u8 sweep_length_timer;
    u8 sweep_length_counter;

    //FF12
    u8 sweep_initial_volume;
    u8 sweep_env_dir;
    u8 sweep_pace_env;

    //FF13
    u16 sweep_period;

    //FF14
    u8 sweep_trigger;
    u8 sweep_length_enable;


    //channel 2 values
    //FF16
    u8 sweep2_duty_cycle;
    u8 sweep2_length_timer;

    //FF17
    u8 sweep2_initial_volume;
    u8 sweep2_env_dir;
    u8 sweep2_pace_env;

    //FF18
    u16 sweep2_period;

    //FF19
    u8 sweep2_trigger;
    u8 sweep2_length_enable;

    //channel 3 values
    //FF1A
    u8 wave_dac_on;

    //FF1B
    u8 wave_length_timer;

    //FF1C
    u8 wave_output_level;

    //FF1D
    u16 wave_period; // add the high end from FF1E

    //FF1E
    u8 wave_trigger;
    u8 wave_length_enable;

    //channel 4 values
    //FF20
    u8 noise_length_timer;

    //FF21
    u8 noise_initial_volume;
    u8 noise_env_dir;
    u8 noise_pace_env;

    //FF22
    u8 noise_clock_shift;
    u8 noise_LFSR_width;
    u8 noise_clock_divider;

    //FF23
    u8 noise_trigger;
    u8 noise_length_enable;

    int frameSequenceCountDown;
    int downSampleCount;
	int bufferFillAmount;
    u8 frameSequencer;
    bool leftEnables[4];
	bool rightEnables[4];
	bool powerControl;
	float mainBuffer[samplesize];

} apu_context;

apu_context *apu_get_context();

void apu_init();
bool apu_step();

void apu_write(u16 address, u8 value);
u8 apu_read(u16 address);

#endif 
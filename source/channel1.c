#include <channel1.h>

#define LENGTH 64

static apu_context con;
static channel1 chan;

u8 channel1_read(u16 address)
{
    u8 val = 0x00;
    u8 val1 = 0x00;
    u8 val2 = 0x00;
    u8 val3 = 0x00;

    switch (address)
    {
        //NR10
        case 0xFF10:
             val1 = (con.sweep_pace & 0xFF) << 4; // sweep peroid load
             val2 = (con.sweep_direction & 0xFF) << 3; // sweep negate
             val3 = (con.sweep_individual_step & 0xFF); // sweep shift
             val = (val1 | val2) | val3;
             break;

        //NR11
        case 0xFF11:
            val1 = (con.sweep_duty_cycle) << 6; // duty
            //val2 = (con.sweep_length_timer && 0x3F) | val1; // lengthload
            val = val1;
            break;

        //NR12
        case 0xFF12:
            val1 = (con.sweep_initial_volume & 0xFF) << 4; // volumeload
            val2 = (con.sweep_env_dir & 0xFF) << 3; // evelopeaddmode
            val3 = (con.sweep_pace_env & 0x07) | val2; // envelopePeroid Load
            val = val1 | val3;
            break;

        //NR14
        case 0xFF14:
            val = (con.sweep_length_enable & 0xFF) << 6;         
            break;
    }

    return val;
}

void channel1_write(u16 address, u8 value)
{
      switch (address)
    {
        case 0xFF10:
            con.sweep_pace = (value >> 4) & 0x0F;
            con.sweep_direction = (value  >> 3) & 0x04;
            con.sweep_individual_step = (value & 0x07); 
            break;
        case 0xFF11:
            con.sweep_duty_cycle = (value >> 6) && 0x03;
            con.sweep_length_timer = (value & 0x3F);
            con.sweep_length_counter = LENGTH - (con.sweep_length_timer & 0x3F);

            break;
        case 0xFF12:
            con.sweep_initial_volume = (value >> 4) & 0x0F;
            con.sweep_env_dir = (value >> 3) & 0x01;
            con.sweep_pace_env = (value & 0x07);
            break;
        case 0xFF13:
            // /NR13 write only period low channel 1
            con.sweep_period = con.sweep_period & 0x700 | value; //timerload
            break;
        case 0xFF14:
            //NR14
            con.sweep_period = (con.sweep_period & 0xFF) | ((value & 0x07) << 8 ); 
            con.sweep_trigger = (value >> 7) & 0x01;
            if(con.sweep_trigger == 0x01)
            {
                set_sweep_trigger();
            } //trigger
            con.sweep_length_enable = (value >> 6) & 0x01; // lengthenable
            break;
    }
}
void channel1_init()
{
    chan.envelope_running = true;
    chan.envelope_period = 0;
    chan.volume = 0;
    chan.sweep_count = 0;
    chan.sweep_enable = false;
    chan.sweep_shadow = 0;
    chan.sweep_timer = 0;
    chan.enabled = false;
    chan.dacEnabled = true;
    chan.timer = 0;

    chan.outputVol = 0;
	chan.sequencePointer = 0;

    con.sweep_period = 0;
}

void set_sweep_env()
{
    if (--chan.envelope_period <=0)
    {
        chan.envelope_period = con.sweep_pace_env;
        if (chan.envelope_period == 0)
        {
            chan.envelope_period = 8;
        }

        if (chan.envelope_running && con.sweep_pace_env > 0)
        {
            if (con.sweep_env_dir && chan.volume < 15)
            {
                chan.volume++;
            }
            else if (!con.sweep_env_dir && chan.volume > 0)
            {
                chan.volume--;
            }
        }

        if (chan.volume == 0 || chan.volume == 15)
        {
            chan.envelope_running = false;
        }

    }

}

u8 get_sweep_volume()
{
    return chan.outputVol;
}

void sweep_step()
{
    if (--chan.timer <= 0)
    {
        chan.timer = (2048 -con.sweep_period) * 4;
        chan.sequencePointer = (chan.sequencePointer + 1) & 0x7;
    }

    if (chan.enabled && chan.dacEnabled)
    {
        chan.outputVol = chan.volume;
    }
    else 
    {
        chan.outputVol = 0;
    }

    //if (!dutyTable[con.sweep_duty_cycle][chan.sequencePointer])
    //{
    //    chan.outputVol = 0;
    //}
}


void set_sweep_clk()
{
    if (--chan.sweep_count <= 0)
    {
        chan.sweep_count = con.sweep_pace;

        if (chan.sweep_count == 0)
        {
            chan.sweep_count = 8;
        }
        if (chan.sweep_enable && con.sweep_pace)
        {
            u16 newFreq = sweep_calculate();
            if (newFreq <= 2047 && con.sweep_individual_step > 0)
            {
                chan.sweep_shadow = newFreq;
                con.sweep_period = newFreq;
                sweep_calculate();
            }
            sweep_calculate();
        }
    }

}

u16 sweep_calculate()
{
    u16 newFreq = 0;
    newFreq = chan.sweep_shadow >> con.sweep_individual_step;
    if (con.sweep_direction)
    {
        newFreq = chan.sweep_shadow - newFreq;
    }
    else 
    {
        newFreq = chan.sweep_shadow + newFreq;
    }


    return newFreq;
}

bool sweep_get_running()
{
    return con.sweep_length_counter > 0;
}

void set_sweep_length_timer()
{
    if (con.sweep_length_counter > 0 && con.sweep_length_enable)
    {
        con.sweep_length_counter--;
        if (con.sweep_length_counter == 0)
        {
            chan.enabled = false;
        }
    }

}

void set_sweep_trigger()
{
    chan.enabled = true;
    if (con.sweep_length_counter == 0)
    {
        con.sweep_length_counter = 64;
    }
    chan.sweep_timer = (2048 - con.sweep_period) * 4;
    chan.envelope_running = true;
    chan.envelope_period = con.sweep_pace_env;
    chan.volume = con.sweep_initial_volume;
    chan.sweep_shadow = con.sweep_period;
    chan.sweep_count = con.sweep_pace;
    if (chan.sweep_count == 0)
    {
        chan.sweep_count = 8;
    }

    chan.sweep_enable = (chan.sweep_count > 0) || (con.sweep_individual_step > 0);

    if (con.sweep_individual_step > 0)
    {
        sweep_calculate();
    }
}


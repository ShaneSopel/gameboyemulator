#include <channel1.h>

static apu_context con;

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
             val1 = (con.sweep_pace & 0xFF) << 4;
             val2 = (con.sweep_direction & 0xFF) << 3;
             val3 = (con.sweep_individual_step & 0xFF);
             val = (val1 | val2) | val3;
             break;

        //NR11
        case 0xFF11:
            val1 = (con.sweep_duty_cycle) << 6;
            //val2 = (con.sweep_length_timer && 0xFF) | val1;
            val = val1;
            break;

        //NR12
        case 0xFF12:
            val1 = (con.sweep_initial_volume & 0xFF) << 4;
            val2 = (con.sweep_env_dir & 0xFF) << 3;
            val3 = (con.sweep_pace_env & 0x07) | val2;
            val = val1 | val2;
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
            break;
        case 0xFF12:
            con.sweep_initial_volume = (value >> 4) & 0x0F;
            con.sweep_env_dir = (value >> 3) & 0x01;
            con.sweep_pace_env = (value & 0x07);
            break;
        case 0xFF13:
            // /NR13 write only period low channel 1
            con.sweep_period = con.sweep_period & 0x700 | value;
            break;
        case 0xFF14:
            //NR14
            con.sweep_period = (con.sweep_period & 0xFF) | ((value & 0x07) << 8 );
            con.sweep_trigger = (value >> 7) & 0x07;
            con.sweep_length_enable = (value >> 6) & 0x01;
            break;
    }

}


u8 get_sweep_duty_cycle()
{
    if (con.sweep_duty_cycle == 0x00)
    {

    }
    if (con.sweep_duty_cycle == 0x01)
    {

    }
    if (con.sweep_duty_cycle == 0x02)
    {

    }
    if (con.sweep_duty_cycle == 0x03)
    {

    }
}

void set_sweep_duty_cycle(u8 value)
{
    con.sweep_duty_cycle = value;
}


void set_sweep()
{

}

void sweep_calculate()
{

}


void set_sweep_length_timer()
{

}

void set_sweep_trigger()
{

}


#include <channel1.h>

static apu_context con;

u8 channel2_read(u16 address)
{
    u8 val = 0x00;
    u8 val1 = 0x00;
    u8 val2 = 0x00;
    u8 val3 = 0x00;

    switch (address)
    {

        //NR21
        case 0xFF16:
            val1 = (con.sweep2_duty_cycle) << 6;
            //val2 = (con.sweep2_length_timer && 0xFF) | val1;
            val = val1;
            break;

        //NR22
        case 0xFF17:
            val1 = (con.sweep2_initial_volume && 0xFF) << 4;
            val2 = (con.sweep2_env_dir && 0xFF) << 3;
            val3 = (con.sweep2_pace_env && 0x07) | val2;
            val = val1 | val2;
            break;

        //NR24
        case 0xFF19:
            val = (con.sweep2_length_enable & 0xFF) << 6;         
            break;
    }

    return val;
}

void channel2_write(u16 address, u8 value)
{
      switch (address)
    {
        case 0xFF16:
            con.sweep2_duty_cycle = (value >> 6) && 0x03;
            con.sweep2_length_timer = (value & 0x3F);
            break;
        case 0xFF17:
            con.sweep2_initial_volume = (value >> 4) & 0x0F;
            con.sweep2_env_dir = (value >> 3) & 0x01;
            con.sweep2_pace_env = (value & 0x07);
            break;
        case 0xFF18:
            // /NR23 write only period low channel 1
            con.sweep2_period = con.sweep2_period & 0x700 | value;
            break;
        case 0xFF19:
            //NR24
            con.sweep2_period = (con.sweep2_period & 0xFF) | ((value & 0x07) << 8 );
            con.sweep2_trigger = (value >> 7) & 0x07;
            con.sweep2_length_enable = (value >> 6) & 0x01;
            break;
    }

}


u8 get_pulse_duty_cycle()
{
    if (con.sweep2_duty_cycle == 0x00)
    {

    }
    if (con.sweep2_duty_cycle == 0x01)
    {

    }
    if (con.sweep2_duty_cycle == 0x02)
    {

    }
    if (con.sweep2_duty_cycle == 0x03)
    {

    }
}

void set_pulse_duty_cycle(u8 value)
{
    con.sweep2_duty_cycle = value;
}


void set_pulse()
{

}

void pulse_calculate()
{
    
}


void set_pulse_length_timer()
{

}


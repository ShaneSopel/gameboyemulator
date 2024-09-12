#include <channel3.h>

static apu_context con;

void channel3_write(u16 address, u8 value)
{
   
    switch (address)
    {
        case 0xFF1A:
            con.wave_dac_on = (value >> 7) & 0x01;
            break;
        case 0xFF1B:
            con.wave_length_timer = (value & 0xFF);
            break;
        case 0xFF1C:
            con.wave_output_level = (value >> 5) & 0x03;
            break;
        case 0xFF1D:
            // /NR13 write only period low channel 1
            con.wave_period = con.wave_period & 0x700 | value;
            break;
        case 0xFF1E:
            //NR14
            con.wave_period = (con.wave_period & 0xFF) | ((value & 0x07) << 8 );
            con.wave_trigger = (value >> 7) & 0x07;
            con.wave_length_enable = (value >> 6) & 0x01;
            break;
    }

}

u8 channel3_read(u16 address)
{
    u8 val = 0x00;
    u8 val1 = 0x00;
    u8 val2 = 0x00;
    u8 val3 = 0x00;

    switch (address)
    {
        //NR30
        case 0xFF1A:
             val = (con.wave_dac_on) << 7;
             break;

        //NR32
        case 0xFF1C:
            val = (con.wave_output_level && 0xFF) << 5;
            break;

        //NR14
        case 0xFF14:
            val = (con.wave_length_enable & 0xFF) << 6;         
            break;
    }

    return val;
}

void set_wave_length_timer()
{

}

void set_wave_trigger()
{

}
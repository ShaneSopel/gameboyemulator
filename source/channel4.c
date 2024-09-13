#include <channel4.h>

static apu_context con;

void channel4_write(u16 address, u8 value)
{
     switch (address)
    {
        case 0xFF20:
            con.noise_length_timer = (value & 0x3F);
            break;
        case 0xFF21:
            con.noise_initial_volume = (value >> 4) & 0x0F;
            con.noise_env_dir = (value >> 3) & 0x01;
            con.noise_pace_env = (value & 0x07);
            break;
        case 0xFF22:
            con.noise_clock_shift = (value >> 4);
            con.noise_LFSR_width = (value >> 3);
            con.noise_clock_divider = (value & 0x07);
            break;
        case 0x23:
            //NR14
            con.noise_trigger = (value >> 7) & 0x01;
            con.noise_length_enable = (value >> 6) & 0x01;
            break;
    }

}

u8 channel4_read(u16 address)
{
     u8 val = 0x00;
    u8 val1 = 0x00;
    u8 val2 = 0x00;
    u8 val3 = 0x00;

    switch (address)
    {
        //NR42
        case 0xFF21:
            val1 = (con.sweep_initial_volume && 0xFF) << 4;
            val2 = (con.sweep_env_dir & 0xFF) << 3;
            val3 = (con.sweep_pace_env & 0x07) | val2;
            val = val1 | val2;
            break;

        //NR43
        case 0xFF22:
            val1 = (con.noise_clock_shift & 0xFF) << 4;
            val2 = (con.noise_LFSR_width & 0xFF) << 3;
            val3 = (con.noise_clock_divider & 0x07);
            val = val1 | val2 | val3;
            break;

        //NR44
        case 0xFF23:
            val = (con.wave_length_enable & 0xFF) << 6;         
            break;
    }

    return val;
    
}
#include <apu.h>

static apu_context con;

apu_context *apu_get_context()
{
    return &con;
}

void apu_init()
{
    //FF26
    con.audio_on = false;

    con.CH1_on = false;
    con.CH2_on = false;
    con.CH3_on = false;
    con.CH4_on = false;

    //FF25
    con.CH4_left = false;
    con.CH3_left = false;
    con.CH2_left = false;
    con.CH1_left = false;
    con.CH4_right = false;
    con.CH3_right = false;
    con.CH2_right = false;
    con.CH1_right = false;

    //FF24
    con.VIN_left = false;
    con.VIN_right = false;
    con.left_volume = 0;
    con.right_volume = 0;

    SDL_AudioSpec audio;
    audio.freq = 44100;
    audio.format = AUDIO_F32SYS;
    audio.channels = 2;
    audio.samples = samplesize;
    audio.callback = NULL;
    audio.userdata = &con;

    SDL_AudioSpec obAudio;
    SDL_OpenAudio(&audio, &obAudio);
    SDL_PauseAudio(0);
}

void apu_update()
{

}

void apu_write(u16 address, u8 value)
{
    if (address == 0xFF25)
    {


    }
    if (address == 0xFF26)
    {


    }
}

u8 apu_read(u16 address)
{
    u8 val = 0x00;
    u8 val1 = 0x00;
    u8 val2 = 0x00;
    u8 val3 = 0x00;
    u8 val4 = 0x00;

    if (address == 0xFF25)
    {
        val1 = (con.audio_on << 7);
        val2 = (con.CH4_on << 3);
        val3 = (con.CH3_on << 2);
        val4 = (con.CH2_on << 1);
        val =   val1| val2 | val3 | val4 | con.CH1_on;
    }

    if (address == 0xFF26)
    {
        val = (con.CH4_left << 7) | (con.CH3_left << 6) | (con.CH2_left << 5) | (con.CH1_left << 4)
                (con.CH4_right << 3) | (con.CH3_right << 2) | (con.CH2_right << 1) || (con.CH1_right);
    }

    return val;
}
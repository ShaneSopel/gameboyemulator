#include <apu.h>
#include <emu.h>

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
    con.left_volume = 3;
    con.right_volume = 3;

    con.frameSequenceCountDown = 8192;
    con.downSampleCount = 95;
    con.bufferFillAmount = 0;
    con.frameSequencer = 0;
    bool leftEnables[4] = { false };
	bool rightEnables[4] = { false };
	bool powerControl = false;
    int bufferFillAmount = 0;
	float mainBuffer[samplesize] = { 0 };

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

    channel1_init();
}

void apu_write(u16 address, u8 value)
{
     
    if (address == 0xFF24)
    {
        con.VIN_left = (value >> 7 ) & 0x01;
        con.left_volume = (value >> 4) & 0xF;
        con.VIN_right = (value >> 3) & 0x01;
        con.right_volume = (value & 0x07);
    } 

    if (address == 0xFF25)
    {
        con.CH4_left = (value >> 7) & 0x01;
        con.CH3_left = (value >> 6) & 0x01;
        con.CH2_left = (value >> 5) & 0x01;
        con.CH1_left = (value >> 4) & 0x01; 
        con.CH4_right = (value >> 3) & 0x01;
        con.CH2_right = (value >> 2) & 0x01;
        con.CH3_right = (value >> 1) & 0x01;
        con.CH1_right = (value & 0x1); 
    }

    if (address == 0xFF26)
    {
        con.audio_on = (value >> 7) & 0x01;
        con.CH4_on = (value >> 3);
        con.CH3_on = (value >> 2);
        con.CH2_on = (value >> 1);
        con.CH1_on = value & 0x01;
    }
}

u8 apu_read(u16 address)
{
    u8 val = 0x00;
    u8 val1 = 0x00;
    u8 val2 = 0x00;
    u8 val3 = 0x00;
    u8 val4 = 0x00;

    if (address == 0xFF24)
    {
        val1 = (con.VIN_left << 7);
        val2 = (con.left_volume << 4);
        val3 = (con.VIN_right << 3);
        val = (con.right_volume & 0x07) | val1 | val2 | val3;
    }

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
        val = (con.CH4_left << 7) | (con.CH3_left << 6) | (con.CH2_left << 5) | 
            (con.CH1_left << 4) | (con.CH4_right << 3) | (con.CH3_right << 2) | 
            (con.CH2_right << 1) | (con.CH1_right);
    }

    return val;
}

bool apu_step()
{
    if (--con.frameSequenceCountDown <= 0)
    {
        con.frameSequenceCountDown = 8192;
        switch(con.frameSequencer)
        {
            case 0:
                set_sweep_length_timer();
                break;
            case 1:
                break;
            case 2:
                set_sweep_clk();
                set_sweep_length_timer();
            case 3:
                break;
            case 4:
                set_sweep_length_timer();
                break;
            case 5:
                break;
            case 6:
                set_sweep_clk();
                set_sweep_length_timer();
                break;
            case 7:
                set_sweep_env();
                break;
        }
        con.frameSequencer++;
        if (con.frameSequencer >= 8)
        {
            con.frameSequencer = 0;
        }

        sweep_step();

        if (--con.downSampleCount <= 0)
        {
            con.downSampleCount = 95;

            float bufferin0 = 0;
            float bufferin1 = 1;

            int volume = (128*con.left_volume)/7;
            if (con.leftEnables[0])
            {
                bufferin1 = ((float)get_sweep_volume()) / 100;
                SDL_MixAudioFormat((Uint8*)&bufferin0, (Uint8)&bufferin1, AUDIO_F32SYS, sizeof(float), volume);
            }

            con.mainBuffer[con.bufferFillAmount] = bufferin0;

            bufferin0 = 0;
            volume = (128 * con.right_volume) / 7;
            if (con.rightEnables[0])
            {
                bufferin1 = ((float)get_sweep_volume()) / 100;
                SDL_MixAudioFormat((Uint8*)&bufferin0, (Uint8)&bufferin1, AUDIO_F32SYS, sizeof(float), volume);
            }

            con.mainBuffer[con.bufferFillAmount + 1] = bufferin0;

            con.bufferFillAmount += 2;
        }

        if (con.bufferFillAmount >= samplesize)
        {
            con.bufferFillAmount = 0;
            while ((SDL_GetQueuedAudioSize(1)) > samplesize * sizeof(float))
            {
                SDL_Delay(1);
            }

            SDL_QueueAudio(1, con.mainBuffer, samplesize*sizeof(float));
        }
    }
   
}
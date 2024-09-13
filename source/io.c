#include <io.h>

static char serial_data[2];

u8 io_read(u16 address)
{
    // 0xFF00 is gamepad
    if (address == 0xFF00)
    {
        return gamepad_get_output();
    }

    // FF01 and FF02 are serial transfer
    if (address == 0xFF01)
    {
        return serial_data[0];
    }
    if (address == 0xFF02)
    {
        return serial_data[1];
    }
    //FF04 and FF07 are time and divder memory maps for IO
    if (BETWEEN(address, 0xFF04, 0xFF07))
    {
        return timer_read(address);
    }
    // interrupt flag
    if (address == 0xFF0F)
    {
        return cpu_get_int_flags();
    }

    if (BETWEEN(address, 0xFF10, 0xFF14))
    {
        return channel1_read(address);
    }

    if (BETWEEN(address, 0xFF16, 0xFF19))
    {
        return channel2_read(address);
    }

    if (BETWEEN(address, 0xFF1A, 0xFF1D))
    {
        return channel3_read(address);
    }

    if(BETWEEN(address, 0xFF20, 0xFF23))
    {
        return channel4_read(address);
    }
    if (BETWEEN(address, 0xFF24, 0xFF26))
    {
        return apu_read(address);
    }

    if (BETWEEN(address, 0xFF30, 0xFF3F))
    {
        return wram_read(address);
    }

    if (BETWEEN(address, 0xFF40, 0xFF4B))
    {
        return lcd_read(address);
    }

    printf("Unsupported io_read(%04X)\n", address);
    return 0;
}

void io_write(u16 address, u8 value)
{
    if (address == 0xFF00)
    {
        gamepad_set_sel(value);
        return;
    }
    if (address == 0xFF01)
    {
        serial_data[0] = value;
        return;
    }
    if (address == 0xFF02)
    {
        serial_data[1] = value;
        return;
    }
    if (BETWEEN(address, 0xFF04, 0xFF07))
    {
        timer_write(address, value);
        return;
    }
    // interrupt flag
    if (address == 0xFF0F)
    {
        cpu_set_int_flags(value);
        return;
    }

    if (BETWEEN(address, 0xFF10, 0xFF14))
    {
        channel1_write(address, value);
        return;
    }

    if (BETWEEN(address, 0xFF16, 0xFF19))
    {
        channel2_write(address, value);
        return;
    }

    if (BETWEEN(address, 0xFF1A, 0xFF1D))
    {
        channel3_write(address, value);
    }

    if(BETWEEN(address, 0xFF20, 0xFF23))
    {
        channel4_write(address, value);
        return;
    }

    if (BETWEEN(address, 0xFF30, 0xFF3F))
    {
        wram_write(address, value);
        return;
    }

    if (BETWEEN(address, 0xFF24, 0xFF26))
    {
        apu_write(address, value);
        return ;
    }

    if (BETWEEN(address, 0xFF40, 0xFF4B))
    {
        lcd_write(address, value);
        return;
    }

    printf("Unsupported io_write(%04X)\n", address);

}

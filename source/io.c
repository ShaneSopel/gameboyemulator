#include <io.h>

static char serial_data[2];

u8 ly = 0;

u8 io_read(u16 address)
{
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

    if(address == 0xFF44)
    {
        return ly++;
    }
    printf("Unsupported io_read(%04X)\n", address);
    return 0;
}
void io_write(u16 address, u8 value)
{
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

    if (address == 0xFF46)
    {
        dma_start(value);
        printf("DMA Starting");
    }
    printf("Unsupported io_write(%04X)\n", address);

}

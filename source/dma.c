#include <dma.h>

typedef struct
{
    bool active;
    u8 byte;
    u8 value;
    u8 start_delay;
} dma_context;

static dma_context con;

void dma_start(u8 start)
{
    con.active = true;
    con.byte = 0;
    con.start_delay = 2;
    con.value = start;
}

void dma_tick()
{
    if (!con.active)
    {
        return;
    }

    if (con.start_delay)
    {
        con.start_delay --;
        return;
    }

    ppu_oam_write(con.byte, bus_read((con.value* 0x100) + con.byte));

    con.byte++;

    con.active = con.byte < 0xA0;

    if (!con.active)
    {
        printf("DMA DONE! \n");
        sleep(2);
    }

}

bool dma_transferring()
{
    return con.active;
}
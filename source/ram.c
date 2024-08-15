#include <ram.h>

typedef struct
{
    u8 wram[0x2000];
    u8 hram[0x80];
    
}ram_context;

static ram_context con;

u8 wram_read(u16 address)
{
    address -= 0xC000;

    if(address >= 0x2000)
    {
        printf("INVALID WRAM ADDR %08X\n", address + 0xC000);
        exit(-1);
    }

    return con.wram[address];
}

void wram_write(u16 address, u8 value)
{
    

}

u8 hram_read(u16 address)
{

}

void hram_write(u16 address, u8 value)
{

}
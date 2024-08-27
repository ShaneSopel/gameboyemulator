#include <ppu.h>
#include <lcd.h>
#include <string.h>
#include <ppu_sm.h>

static ppu_context con;

static const int bufferSize = 32;

ppu_context *ppu_get_context()
{
    return &con;
}

void ppu_init()
{
    con.current_frame = 0;
    con.line_ticks    = 0;
    con.video_buffer  =  malloc(YRES * XRES * sizeof(32));

    lcd_init();
    LCDS_MODE_SET(MODE_OAM);

    memset(con.oam_ram, 0, sizeof(con.oam_ram));
    memset(con.video_buffer, 0, YRES * XRES * sizeof(u32));

}

void ppu_tick()
{
    con.line_ticks++;

    switch(LCDS_MODE)
    {
    case MODE_OAM:
        ppu_mode_oam();
        break;
    case MODE_XFER:
        ppu_mode_xfer();
        break;
    case MODE_VBLANK:
        ppu_mode_vblank();
        break;
    case MODE_HBLANK:
        ppu_mode_hblank();
        break;
    }
}

void ppu_oam_write(u16 address, u8 value)
{
    if (address >= 0xFE00)
    {
        address -= 0xFE00;
    }

    u8 *p = (u8 *)con.oam_ram;
    p[address] = value;
}

u8 ppu_oam_read(u16 address)
{
    if (address >= 0xFE00)
    {
        address -= 0xFE00;
    }

    u8 *p = (u8 *)con.oam_ram;
    return p[address];
}

void ppu_vram_write(u16 address, u8 value)
{
    con.vram[address - 0x8000] = value;
}

u8 ppu_vram_read(u16 address)
{
    return con.vram[address - 0x8000];
}

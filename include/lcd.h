#ifndef LCD_H
#define LCD_H

#include <common.h>
#include <dma.h>
#include <ppu.h>


//FF40 LCDC (LCD Control)
    // bit  7 - LCD and PPU Enable
    // bit 6  - Window tile 
    // bit 5  - window enable
    // bit 4  - BG and Window tile data area 0 - 8800 - 97FF, 1 - 8000 -8FFF
    // bit 3  - BG tile map area 0 - 8800 - 97FF, 1 - 8000 -8FFF
    // bit 2  - OBJ size
    // bit 1  - OBJ Enable
    // bit 0  - BG and window enable / priority
#define LCDC_BGW_ENABLE (BIT(lcd_get_context()->lcdc, 0))
#define LCDC_OBJ_ENABLE (BIT(lcd_get_context()->lcdc, 1))
#define LCDC_OBJ_HEIGHT (BIT(lcd_get_context()->lcdc, 2) ? 16 : 8)
#define LCDC_BG_MAP_AREA (BIT(lcd_get_context()->lcdc, 3) ? 0x9C00 : 0x9800)
#define LCDC_BGW_DATA_AREA (BIT(lcd_get_context()->lcdc, 4) ? 0x8000 : 0x8800)
#define LCDC_WIN_ENABLE (BIT(lcd_get_context()->lcdc, 5))
#define LCDC_WIN_MAP_AREA (BIT(lcd_get_context()->lcdc, 6) ? 0x9C00 : 0x9800)
#define LCDC_LCD_ENABLE (BIT(lcd_get_context()->lcdc, 7))

//FF41 STAT LCD Status
    //  bit 6 LYC INT Select
    //  bit 5 MODE 2 int select
    //  bit 4 MODE 1 int select
    //  bit 3 MODE 0 int select
    //  bit 2 LYC == LY
    //  bit 1-0 PPU Mode 

#define LCDS_MODE ((lcd_mode)(lcd_get_context()->lcds & 0b11))
#define LCDS_MODE_SET(mode) { lcd_get_context()->lcds &= ~0b11; lcd_get_context()->lcds |= mode; }




//FF42 SCY (Scroll Y) (R/w) FF43 SCX (Scroll X)
//FF44 LY LCD Y coordinate (current line we are on)
//FF45 LYC LY  compare (compare register)
//FF47 BGP (BG Palette Data) (R/W) -Non CGB Mode Only

#define LCDS_LYC (BIT(lcd_get_context()->lcds, 2))

#define LCDS_LYC_SET(b) (BIT_SET(lcd_get_context()->lcds, 2, b))

#define LCDS_STAT_INT(src) (lcd_get_context()->lcds & src)

typedef struct 
{
    u8 lcdc; //FF40
    u8 lcds; //FF41
    u8 scroll_y; //FF42
    u8 scroll_x; //FF43
    u8 ly; //FF44
    u8 ly_compare; //FF45
    u8 dma; //FF46
    u8 bg_palette; //FF47
    u8 obj_palette[2];
    u8 win_y;
    u8 win_x;

    u32 bg_colors[4];
    u32 sp1_colors[4];
    u32 sp2_colors[4];

} lcd_context;

typedef enum
{
    MODE_HBLANK,
    MODE_VBLANK,
    MODE_OAM,
    MODE_XFER
} lcd_mode;

typedef enum 
{
    SS_HBLANK  = (1 << 3),
    SS_VBLANK  = (1 << 4),
    SS_OAM     = (1 << 5),
    SS_LYC     = (1 << 6),
} stat_src;

lcd_context *lcd_get_context();

void lcd_init();

u8 lcd_read(u16 address);
void lcd_write(u16 address, u8 value);

#endif

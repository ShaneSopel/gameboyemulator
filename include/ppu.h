#ifndef PPU_H
#define PPU_H

#include <common.h>

//objet attribute memory entry

// BG Map Attributes
// bit 7 BG to DAM Priority
// bit 6 vertical flip
// bit 5 horizontal flip
// bit 4  not used
// bit 3 tile VRAM bank number
// bit 2-0 background palette number 

// byte 3 attribute flags
// bit 7 BG and Window 0 = no 1 = BG and window colors 1-3 over the OBJ 
// bit 6 y flip 0 = normal 1 = vertically mirroed
// bit 5 x flip 0 = normal 1 = horizontally mirroed
// bit 4 palette number 
// bit 3 tile VRAM bank
// bit 2-0 pallette number 

// the tile data is stored in the memory area 0x8000 - 0x97FF
// with each tile taking 16 bytes.
// meaning you will have 384 tiles. 
// each tile has 8x8 pixels and has color depth of 4 colors/ grey shades.

// block   vram address     OBJS     BG/WIN if LDCD4=1   BG/WIN if LDCD4=0
//   0       8000 - 87FF    0-127      0 - 127
//   1       8800 - 87FF    128-255    128-255                  128 - 255
//   2       9000 - 97FF    cant use     b                        0 - 127


typedef struct 
{
    u8 y;
    u8 x;
    u8 tile;
    //u8 flags;

    //create a bit field instead of a u8 byte
    unsigned f_cgb_pn : 3;
    unsigned f_cgb_vram_bank : 1;
    unsigned f_pn : 1;
    unsigned f_x_flip : 1;
    unsigned f_y_flip : 1;
    unsigned f_bgp : 1 ;
} oam_entry;

typedef struct ppu
{
    oam_entry oam_ram[40];
    u8 vram[0x2000];
}ppu_context;


void ppu_init();
void ppu_tick();

void ppu_oam_write(u16 address, u8 value);
u8 ppu_oam_read(u16 address);

void ppu_vram_write(u16 address, u8 value);
u8 ppu_vram_read(u16 address);

#endif 
#include <bus.h>

//0x0000 - 0x3FFF	16 KiB ROM bank 00	From cartridge, usually a fixed bank
//0x4000 - 0x7FFF	16 KiB ROM Bank 01–NN	From cartridge, switchable bank via mapper (if any)
//0x8000 - 0x9FFF	8 KiB Video RAM (VRAM)	In CGB mode, switchable bank 0/1 // (0x8000 - 0x97FF BG Map 1) (0x9C00 - 0x9FFF BM Map 2)
//0xA000 - 0xBFFF	8 KiB External RAM	From cartridge, switchable bank if any
//0xC000 - 0xCFFF	4 KiB Work RAM (WRAM)	RAM BANK 0
//0xD000 - 0xDFFF	4 KiB Work RAM (WRAM)	In CGB mode, switchable bank 1–7
//0xE000 - 0xFDFF	Echo RAM (mirror of C000–DDFF)	Nintendo says use of this area is prohibited.
//0xFE00 - 0xFE9F	Object attribute memory (OAM)	
//0xFEA0 - 0xFEFF	Not Usable	Nintendo says use of this area is prohibited.
//0xFF00 - 0xFF7F	I/O Registers	
//0xFF80 - 0xFFFE	High RAM (HRAM) (zero page)	
//FFFF	FFFF	Interrupt Enable register (IE)	

u8 bus_read(u16 address)
{
    if (address < 0x8000)
    {
        return cart_read(address);
    } 
    else if (address < 0xA000)
    {
        //char/map data
        return ppu_vram_read(address);
    } 
    else if (address < 0xC000) 
    {
        //Cartridge RAM
        return cart_read(address);
    }
     else if (address < 0xE000){
        //WRAM (Working RAM)
        return wram_read(address);
    } 
    else if (address < 0xFE00) 
    {
        //reserved echo ram...
        return 0;
    } 
    else if (address < 0xFEA0) 
    {
        //OAM
        if(dma_transferring())
        {
            return 0xFF;
        }
        return ppu_oam_read(address);
    } 
    else if (address < 0xFF00) 
    {
        //reserved unusable...
        return 0;
    } 
    else if (address < 0xFF80) 
    {
        //IO Registers...
        return io_read(address);
    } 
    else if (address == 0xFFFF) 
    {
        //CPU Enable Register..
        return cpu_get_ie_register();
    }

    //NO_IMPL
    return hram_read(address);
}

void bus_write(u16 address, u8 value)
{
    if (address < 0x8000)
    {
        //ROM Data
        cart_write(address, value);
    } 
    else if (address < 0xA000) 
    {
        //Char/Map Data
        ppu_vram_write(address, value);
    } 
    else if (address < 0xC000) 
    {
      //EXT-RAM
      cart_write(address, value);  
    } 
    else if (address < 0xE000) 
    {
        wram_write(address, value);
    }
    else if (address < 0xFE00)
    {
        //reserved echo ram
    }
    else if (address < 0xFEA0)
    {
        //OAM 
        if(dma_transferring())
	{
	   return;
	}
        ppu_oam_write(address, value);
    } 
    else if (address < 0xFF00) 
    {
        //unusable reserved
    } 
    else if (address < 0xFF80) 
    {
        //IO Register...
        io_write(address, value);
    }
    else if (address == 0xFFFF)
    {
        // CPU SET Enable Register
        cpu_set_ie_register(value);
    }
    else
    {
        hram_write(address, value);
    }
}

u16 bus_read16(u16 address) {
    u16 lo = bus_read(address);
    u16 hi = bus_read(address + 1);

    return lo | (hi << 8);
}

void bus_write16(u16 address, u16 value) {
    bus_write(address + 1, (value >> 8) & 0xFF);
    bus_write(address, value & 0xFF);
}

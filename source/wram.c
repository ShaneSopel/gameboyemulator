#include <wram.h>

void wram_write(u16 address, u8 value)
{
    if (address >= 0xC000 && address <= 0xFDFF) 
    {
		uint16_t wramAddress = (address & 0x1FFF);
		if (wramAddress >= 0x1000) 
        {
			wramAddress &= 0xFFF;
			wramAddress |= WRAMBank << 12;
		}
		RAM[wramAddress] = value;	// Mask covers echo area too
	}
	else if (address == 0xFF70) 
    {
		WRAMBank = value & 0x7;
		if (WRAMBank == 0) 
        {
			WRAMBank = 1;
		}
	}

}

u8 wram_read(u16 address)
{
    uint8_t returnVal = 0xFF;
	if (address >= 0xC000 && address <= 0xFDFF) 
    {
		uint16_t wramAddress = (address & 0x1FFF);
		if (wramAddress >= 0x1000) 
        {
			wramAddress &= 0xFFF;
			wramAddress |= WRAMBank << 12;
		}
		returnVal = RAM[wramAddress];	// Mask covers echo area too
	}
	else if (address == 0xFF70) 
    {
		returnVal = WRAMBank;
	}
	return returnVal;

}
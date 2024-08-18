#ifndef CPU_H
#define CPU_H

#include <common.h>
#include <instructions.h>

// the registers of the game boy CPU
// Registers
//16-bit	Hi	Lo	Name/Function
//AF	A	-	Accumulator & Flags
//BC	B	C	BC
//DE	D	E	DE
//HL	H	L	HL
//SP	-	-	Stack Pointer
//PC	-	-	Program Counter/Pointer
typedef struct 
{
    u8 a;
    u8 f;
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 h;
    u8 l;
    u16 pc;
    u16 sp;
} cpu_registers;

typedef struct cpu
{
    // the registers that cpu has that we created above. 
    cpu_registers regs;

    // current fetch
    u16 fetch_data;

    u16 mem_dest;

    bool dest_is_mem;

    u8 cur_opcode;

    instruction *cur_inst;

    bool halted;
    bool stepping;

    bool int_master_enabled;
    bool enabling_ime;
    
    u8 ie_register;
} cpu_context;

void cpu_init();
bool cpu_step();

u8 cpu_get_ie_register();
void cpu_set_ie_register(u8 value);

#endif
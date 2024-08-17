#include <bus.h>
#include <cpu.h>
#include <cpu_fetch.h>
#include <cpu_proc.h>
#include <emu.h>

cpu_context con = {0};

void cpu_init()
{
    con.regs.pc = 0x100;
    con.regs.a = 0x01;
}

static void fetch_instruction()
{
    con.cur_opcode = bus_read(con.regs.pc++);
    con.cur_inst = instruction_by_opcode(con.cur_opcode);
}


static void execute()
{
    IN_PROC proc = inst_get_processor(con.cur_inst->type);

    if(!proc)
    {
        NO_IMPL
    }

    proc(&con);
}

bool cpu_step()
{
    if (!con.halted)
    {
        u16 pc = con.regs.pc;
        // fetch the instruction
        fetch_instruction();

        //fetch the data
        emu_cycles(1);
        fetch_data();

        printf("%04X:  %-7s (%02X %02X %02X) A: %02X, BC: %02X%02X, DE: %02X%02X HL: %02X%02X \n", 
        pc, inst_name(con.cur_inst->type), con.cur_opcode,
        bus_read(pc + 1), bus_read(pc + 2), con.regs.a, con.regs.b, con.regs.c,
        con.regs.d, con.regs.e, con.regs.h, con.regs.l);

        if (con.cur_inst == NULL)
        {
            printf("unknown instruction! %02X\n", con.cur_opcode);
            exit(-7);
        }

        //execute
        execute();
    }
    
    return true;
}


u8 cpu_get_ie_register()
{
    return con.ie_register;
}

void cpu_set_ie_register(u8 value)
{
    con.ie_register = value;
}

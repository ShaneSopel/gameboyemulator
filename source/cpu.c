#include <cpu.h>
#include <common.h>
#include <cpu_fetch.h>
#include <cpu_proc.h>
#include <emu.h>
#include <dbg.h>
#include <interrupts.h>

cpu_context con = {0};

void cpu_init()
{
    // default values of these registers.
    con.regs.pc = 0x100;
    con.regs.sp = 0xFFFE;
    *((short *)&con.regs.a) = 0xB001;
    *((short *)&con.regs.a) = 0x1300;
    *((short *)&con.regs.a) = 0xD800;
    *((short *)&con.regs.a) = 0x4D81;
    con.ie_register = 0;
    con.int_flags = 0;
    con.int_master_enabled = false;
    con.enabling_ime = false;

    timer_get_context()->div = 0xABCC;
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

        char flags[16];
        sprintf(flags, "%c%c%c%c",
            con.regs.f & (1 <<7) ? 'Z' : '-',
            con.regs.f & (1 <<6) ? 'N' : '-',
            con.regs.f & (1 <<5) ? 'H' : '-',
            con.regs.f & (1 <<4) ? 'C' : '-'
        );

        char inst[16];
        inst_to_str(&con, inst);

        printf("%08lX - %04X:  %-12s (%02X %02X %02X) A: %02X, F: %s BC: %02X%02X, DE: %02X%02X HL: %02X%02X \n", 
        emu_get_context()->ticks,
        pc, inst, con.cur_opcode,
        bus_read(pc + 1), bus_read(pc + 2), con.regs.a, flags, con.regs.b, con.regs.c,
        con.regs.d, con.regs.e, con.regs.h, con.regs.l);

        if (con.cur_inst == NULL)
        {
            printf("unknown instruction! %02X\n", con.cur_opcode);
            exit(-7);
        }

        dbg_update();
        dbg_print();

        //execute
        execute();
    } else {
        //is halted...
        emu_cycles(1);

        if (con.int_flags) 
        {
            con.halted = false;
        }
    }

    if (con.int_master_enabled) 
    {
        cpu_handle_interrupts(&con);
        con.enabling_ime = false;
    }

    if (con.enabling_ime) 
    {
        con.int_master_enabled = true;
    }

    return true;
}

u8 cpu_get_ie_register()
{
    return con.ie_register;
}

u8 cpu_get_int_flags() {
    return con.int_flags;
}

void cpu_set_int_flags(u8 value) {
    con.int_flags = value;
}

void cpu_set_ie_register(u8 value)
{
    con.ie_register = value;
}

void cpu_request_interrupt(interrupt_type t)
{
    con.int_flags |= t;
}

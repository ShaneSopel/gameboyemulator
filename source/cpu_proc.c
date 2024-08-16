#include <bus.h>
#include <cpu.h>
#include <cpu_proc.h>
#include <emu.h>

// processing CPU Instructions...

static void proc_none(cpu_context *con)
{
    printf("INVALID INSTRUCTION\n");
    exit(-7);
}

static void proc_di(cpu_context *con)
{
    con->int_master_enabled = false;
}

static void proc_ldh(cpu_context *con)
{
    if(con->cur_inst->reg_1 == RT_A)
    {
        cpu_set_reg(con->cur_inst->reg_1, bus_read(0xFF00 | con->fetch_data));
    }
    else
    {
        bus_write(0xFF00 | con->fetch_data, con->regs.a);
    }

    emu_cycles(1);
}

static void proc_nop(cpu_context *con)
{
    printf("no op \n");
}

static void proc_ld(cpu_context *con)
{
    if (con->dest_is_mem)
    {
        if(con->cur_inst->reg_2 >= RT_AF)
        {
            //if 16 bit int bus write
            emu_cycles(1);
            bus_write16(con->mem_dest, con->fetch_data);
        }
        else
        {
            bus_write(con->mem_dest, con->fetch_data);
        }

        return;
    }

    if (con->cur_inst->mode == AM_HL_SPR)
    {
        u8 hflag = (cpu_read_reg(con->cur_inst->reg_2) & 0xF) + (con->fetch_data & 0xF ) 
        >= 0x10;

        u8 cflag = (cpu_read_reg(con->cur_inst->reg_2) & 0xFF) + (con->fetch_data & 0xFF ) 
        >= 0x100;

        cpu_set_flags(con, 0, 0, hflag, cflag);
        cpu_set_reg(con->cur_inst->reg_1, cpu_read_reg(con->cur_inst->reg_2)
        + (char)con->fetch_data);

        return;

    }

    cpu_set_reg(con->cur_inst->reg_1, con->fetch_data);
}

void cpu_set_flags(cpu_context *con, char z, char n, char h, char c)
{
    if(z != -1)
    {
        BIT_SET(con->regs.f, 7, z);
    }

    if(n != -1)
    {
        BIT_SET(con->regs.f, 6, n);
    }

    if(h != -1)
    {
        BIT_SET(con->regs.f, 5, h);
    }

    if(c != -1)
    {
        BIT_SET(con->regs.f, 4, c);
    }

}

static void proc_xor(cpu_context *con)
{
    con->regs.a ^= con->fetch_data & 0xFF;
    cpu_set_flags(con, con->regs.a == 0, 0, 0, 0);
}

static bool check_cond(cpu_context *con)
{
    bool z = CPU_FLAG_Z;
    bool c = CPU_FLAG_C;

    switch(con->cur_inst->cond)
    {
        case CT_NONE: return true;
        case CT_C: return c;
        case CT_NC: return !c;
        case CT_Z: return z;
        case CT_NZ: return !z;
    }

    return false;
}

static void proc_jp(cpu_context *con)
{
    if(check_cond(con))
    {
        con->regs.pc = con->fetch_data;
        emu_cycles(1);
    }
}

IN_PROC processors[] =
{
    [IN_NONE] = proc_none, //  
    [IN_NOP] = proc_nop,
    [IN_LD] = proc_ld,
    [IN_JP] = proc_jp, 
    [IN_DI] = proc_di,
    [IN_LDH] = proc_ldh,
    [IN_XOR] = proc_xor
};

IN_PROC inst_get_processor(in_type type)
{
    return processors[type];
}


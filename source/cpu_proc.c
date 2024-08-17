#include <bus.h>
#include <cpu.h>
#include <cpu_proc.h>
#include <cpu_util.h>
#include <emu.h>
#include <stack.h>

// processing CPU Instructions...

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

static void proc_none(cpu_context *con)
{
    printf("INVALID INSTRUCTION\n");
    exit(-7);
}

static void proc_nop(cpu_context *con)
{
    //printf("no op \n");
}

static void proc_di(cpu_context *con)
{
    con->int_master_enabled = false;
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

        emu_cycles(1);

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

static void proc_ldh(cpu_context *con)
{
    if(con->cur_inst->reg_1 == RT_A)
    {
        cpu_set_reg(con->cur_inst->reg_1, bus_read(0xFF00 | con->fetch_data));
    }
    else
    {
        bus_write(con->mem_dest, con->regs.a);
    }

    emu_cycles(1);
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

static void goto_addr(cpu_context *con, u16 addr, bool pushpc)
{
    if (check_cond(con))
    {
        if (pushpc)
        {
            emu_cycles(2);
            stack_push16(con->regs.pc);
        }

        con->regs.pc = addr;
        emu_cycles(1);
    }
}

static void proc_jp(cpu_context *con)
{
    goto_addr(con, con->fetch_data, false);
}

static void proc_jr(cpu_context *con)
{
    char rel = (char)(con->fetch_data & 0xFF);
    u16 addr = con->regs.pc + rel;
    goto_addr(con, addr, false);
}


static void proc_call(cpu_context *con)
{
    goto_addr(con, con->fetch_data, true);
}

static void proc_ret(cpu_context *con)
{
    if(con->cur_inst->cond != CT_NONE)
    {
        emu_cycles(1);
    }

    if (check_cond(con))
    {
        u16 lo = stack_pop();
        emu_cycles(1);
        u16 hi = stack_pop();
        emu_cycles(1);

        u16 n = (hi << 8) | lo;
        con->regs.pc = n;

        emu_cycles(1);

    }
}

static void proc_reti(cpu_context *con)
{
   con->int_master_enabled = true;
   proc_ret(con);
}


static void proc_pop(cpu_context *con)
{
    u16 lo = stack_pop();
    emu_cycles(1);
    u16 hi = stack_pop();
    emu_cycles(1);

    u16 n = (hi << 8) | lo;

    cpu_set_reg(con->cur_inst->reg_1, n);

    if (con->cur_inst->reg_1 == RT_AF)
    {
        cpu_set_reg(con->cur_inst->reg_1, n & 0xFFF0);
    }
}

static void proc_push(cpu_context *con)
{
    u16 hi = (cpu_read_reg(con->cur_inst->reg_1) >> 8 ) & 0xFF;
    emu_cycles(1);
    stack_push(hi);

    u16 lo = cpu_read_reg(con->cur_inst->reg_2) & 0xFF;
    emu_cycles(1);
    stack_push(lo);

    emu_cycles(1);
}




IN_PROC processors[] =
{
    [IN_CALL] = proc_call,
    [IN_DI] = proc_di,
    [IN_JP] = proc_jp, 
    [IN_JR] = proc_jr,
    [IN_LD] = proc_ld,
    [IN_LDH] = proc_ldh,
    [IN_NONE] = proc_none, //  
    [IN_NOP] = proc_nop,
    [IN_POP] =  proc_pop,
    [IN_PUSH] = proc_push,
    [IN_RET] = proc_ret,
    [IN_RETI] = proc_reti,
    [IN_XOR] = proc_xor
};

IN_PROC inst_get_processor(in_type type)
{
    return processors[type];
}


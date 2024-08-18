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

reg_type rt_lookup[] =
{
    RT_B,
    RT_C,
    RT_D,
    RT_E,
    RT_H,
    RT_L,
    RT_HL,
    RT_A,  
};

reg_type decode_reg(u8 reg)
{
    if (reg > 0b111)
    {
        return RT_NONE;
    }

    return rt_lookup[reg];
}

static void proc_cb(cpu_context *con)
{
    u8 op = con->fetch_data;
    reg_type reg = decode_reg(op & 0b111);
    u8 bit = (op >> 3) & 0b111;
    u8 bit_op = (op >> 6) & 0b11;
    u8 reg_val = cpu_read_reg8(reg);

    emu_cycles(1);

    if (!reg == RT_HL)
    {
        emu_cycles(2);
    }

    switch(bit_op)
    {
        case 1:
            //BIT
            cpu_set_flags(con, !(reg_val & (1 << bit)), 0, 1, -1);
            return;
        case 2:
            //RST
            reg_val &= ~(1 << bit);
            cpu_set_reg8(reg, reg_val);
            return;
        case 3:
            //set
            reg_val |= (1 << bit);
            cpu_set_reg8(reg, reg_val);
            return;
    }

    bool flagC = CPU_FLAG_C;

    switch(bit)
    {
        case 0:
        {
            //RLC
            bool setC = false;
            u8 result = (reg_val << 1) & 0xFF;

            if ((reg_val & (1 << 7)) != 0)
            {
                result |= 1;
                setC = true;
            }
            cpu_set_reg8(reg, result);
            cpu_set_flags(con, result == 0, false, false, setC);
            return;
        }
        case 1:
        {
             //RRC
             u8 old = reg_val;
             reg_val >>= 1;
             reg_val |= (old << 7);

             cpu_set_reg8(reg, reg_val);
             cpu_set_flags(con, !reg_val, false, false, old & 1);
            return;
        }
        case 2:
        {
            //RL
            u8 old = reg_val;
            reg_val <<= 1;
            reg_val |= flagC;

            cpu_set_reg8(reg, reg_val);
            cpu_set_flags(con, !reg_val, false, false, !!(old & 0x80));
            return;
        }
        case 3:
        {
            //RR
            u8 old = reg_val;
            reg_val >> 1;

            reg_val |= (flagC << 7);
            cpu_set_reg8(reg, reg_val);
            cpu_set_flags(con, !reg_val, false, false, old & 1);
            return;
        }
        case 4:
        {
            //SLA
            u8 old = reg_val;
            reg_val <<= 1;

            cpu_set_reg8(reg, reg_val);
            cpu_set_flags(con, !reg_val, false, false, !!(old & 0x80));
            return;
        }
        case 5:
        {
            //SRA
            u8 new = (int8_t) reg_val >> 1;
            cpu_set_reg8(reg, new);
            cpu_set_flags(con, !new, 0, 0, reg_val & 1);
            return;
        }
        case 6:
        {
            //SWAP
            reg_val = ((reg_val & 0xF0) >> 4) | ((reg_val & 0xF) << 4);
            cpu_set_reg8(reg, reg_val);
            cpu_set_flags(con, reg_val == 0, false, false ,false);
            return;
        }
        case 7:
        {
            //SRL
            u8 new = reg_val >> 1;
            cpu_set_reg8(reg, new);
            cpu_set_flags(con, !new, 0, 0, reg_val & 1);
            return;
        }
    }

    fprintf(stderr, "Error: Invalid CB: %02X", op);
    NO_IMPL
}

static void proc_rlca(cpu_context *con)
{
    u8 u = con->regs.a;
    bool c = (u >> 7) & 1;
    u = (u << 1) | c;
    con->regs.a = u;

    cpu_set_flags(con, 0, 0, 0, c);
}

static void proc_rrca(cpu_context *con)
{
    u8 b = con->regs.a & 1;
    con->regs.a >>= 1;
    con->regs.a |= (b << 7);

    cpu_set_flags(con, 0, 0, 0, b);
}

static void proc_rla(cpu_context *con)
{
    u8 u = con->regs.a;
    u8 cf = CPU_FLAG_C;
    u8 c = (u >> 7) & 1;

    con->regs.a = u;
    cpu_set_flags(con, 0, 0, 0, c);
}

static void proc_rra(cpu_context *con)
{
    u8 carry = CPU_FLAG_C;
    u8 new_c = con->regs.a & 1;

    con->regs.a >>=1;
    con->regs.a |= (carry << 7);

    cpu_set_flags(con, 0, 0, 0, new_c);
    
}
static void proc_stop(cpu_context *con)
{
    fprintf(stderr, "Stopping \n");
    NO_IMPL
}

static void proc_daa(cpu_context *con)
{
    u8 u = 0;
    int fc = 0;

    if (CPU_FLAG_H || (!CPU_FLAG_N && (con->regs.a & 0xF) > 9))
    {
        u = 6;

    }

    if (CPU_FLAG_C || (!CPU_FLAG_N && con->regs.a > 0x99))
    {
        u |= 0x60;
        fc = 1;
    }

    con->regs.a += CPU_FLAG_N ? -u : u;

    cpu_set_flags(con, con->regs.a == 0, -1, 0, fc);

}

static void proc_cpl(cpu_context *con)
{
    con->regs.a = ~con->regs.a;
    cpu_set_flags(con, -1, 1, 1, -1);
}

static void proc_scf(cpu_context *con)
{
    cpu_set_flags(con, -1, 0, 0, 1);
}

static void proc_ccf(cpu_context *con)
{
    cpu_set_flags(con, -1, 0, 0, CPU_FLAG_C ^ 1);
}

static void proc_and(cpu_context *con)
{
    con->regs.a &= con->fetch_data;
    cpu_set_flags(con, con->regs.a == 0, 0, 1, 0);
}

static void proc_or(cpu_context *con)
{
    con->regs.a |= con->fetch_data;
    cpu_set_flags(con, con->regs.a == 0, 0, 0, 0);
}

static void proc_xor(cpu_context *con)
{
    con->regs.a ^= con->fetch_data & 0xFF;
    cpu_set_flags(con, con->regs.a == 0, 0, 0, 0);
}

static void proc_cp(cpu_context *con)
{
    int n = (int)con->regs.a - (int)(con->fetch_data);

    cpu_set_flags(con, n == 0, 1, 
    ((int)con->regs.a & 0x0F) - ((int)con->fetch_data & 0x0F) < 0, n < 0);
}


static void proc_di(cpu_context *con)
{
    con->int_master_enabled = false;
}

static bool is_16_bit(reg_type rt)
{
    return rt >= RT_AF;
}

static void proc_ld(cpu_context *con)
{
    if (con->dest_is_mem)
    {
        if(is_16_bit(con->cur_inst->reg_2))
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
        u8 hflag = (cpu_read_reg(con->cur_inst->reg_2) & 0xF) 
        + (con->fetch_data & 0xF ) >= 0x10;

        u8 cflag = (cpu_read_reg(con->cur_inst->reg_2) & 0xFF) 
        + (con->fetch_data & 0xFF ) >= 0x100;

        cpu_set_flags(con, 0, 0, hflag, cflag);
        cpu_set_reg(con->cur_inst->reg_1, 
        cpu_read_reg(con->cur_inst->reg_2) + (char)con->fetch_data);

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


static void proc_rst(cpu_context *con)
{
    goto_addr(con, con->cur_inst->param, true);
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

    u16 lo = cpu_read_reg(con->cur_inst->reg_1) & 0xFF;
    emu_cycles(1);
    stack_push(lo);

    emu_cycles(1);
}

static void proc_inc(cpu_context *con)
{
    u16 val = cpu_read_reg(con->cur_inst->reg_1) + 1;


    if (is_16_bit(con->cur_inst->reg_1))
    {
        emu_cycles(1);
    }

    if(con->cur_inst->reg_1 == RT_HL && con->cur_inst->mode == AM_MR)
    {
        val = bus_read(cpu_read_reg(RT_HL)) + 1;
        val &= 0xFF;
        bus_write(cpu_read_reg(RT_HL), val);
    }
    else
    {
        cpu_set_reg(con->cur_inst->reg_1, val);
        val = cpu_read_reg(con->cur_inst->reg_1);
    }

    if ((con->cur_opcode & 0x03) == 0x03)
    {
        return;
    }

    cpu_set_flags(con, val == 0, 0, (val & 0x0F) == 0, -1);
}

static void proc_dec(cpu_context *con)
{
    u16 val = cpu_read_reg(con->cur_inst->reg_1) - 1;


    if (is_16_bit(con->cur_inst->reg_1))
    {
        emu_cycles(1);
    }

    if(con->cur_inst->reg_1 == RT_HL && con->cur_inst->mode == AM_MR)
    {
        val = bus_read(cpu_read_reg(RT_HL)) - 1;
        //val &= 0xFF;
        bus_write(cpu_read_reg(RT_HL), val);
    }
    else
    {
        cpu_set_reg(con->cur_inst->reg_1, val);
        val = cpu_read_reg(con->cur_inst->reg_1);
    }

    if ((con->cur_opcode & 0x0B) == 0x0B)
    {
        return;
    }

    cpu_set_flags(con, val == 0, 1, (val & 0x0F) == 0x0F, -1);
}

static void proc_sub(cpu_context *con)
{
    u16 val = cpu_read_reg(con->cur_inst->reg_1) - con->fetch_data;

    int z = val == 0;
    int h = ((int)cpu_read_reg(con->cur_inst->reg_1) & 0xF) - ((int)con->fetch_data & 0xF) < 0;
    int c = ((int)cpu_read_reg(con->cur_inst->reg_1)) - ((int)con->fetch_data) < 0;

    cpu_set_reg(con->cur_inst->reg_1, val);
    cpu_set_flags(con, z, 1, h, c);
}

static void proc_sbc(cpu_context *con)
{
    u8 val =  con->fetch_data + CPU_FLAG_C;

    int z = cpu_read_reg(con->cur_inst->reg_1 - val) == 0;
    int h = ((int)cpu_read_reg(con->cur_inst->reg_1) & 0xF) 
    - ((int)con->fetch_data & 0xF) 
    - ((int) CPU_FLAG_C) < 0;
    int c = ((int)cpu_read_reg(con->cur_inst->reg_1)) 
    - ((int)con->fetch_data) 
    - ((int)CPU_FLAG_C) < 0;

    cpu_set_reg(con->cur_inst->reg_1, cpu_read_reg(con->cur_inst->reg_1) - val);
    cpu_set_flags(con, z, 1, h, c);
}

static void proc_adc(cpu_context *con)
{
    u16 u = con->fetch_data;
    u16 a = con->regs.a;
    u16 c = CPU_FLAG_C;

    con->regs.a = (a + u + c) & 0xFF;

    cpu_set_flags(con, con->regs.a == 0, 0,
        (a & 0xF) + (u & 0xF) + c > 0xF,
        a + u + c > 0xFF);

}

static void proc_add(cpu_context *con)
{
    u32 val = cpu_read_reg(con->cur_inst->reg_1) +
        con->fetch_data;
    
    bool is_16bit = is_16_bit(con->cur_inst->reg_1);
    if (is_16bit)
    {
        emu_cycles(1);
    }

    if(con->cur_inst->reg_1 == RT_SP)
    {
        val = cpu_read_reg(con->cur_inst->reg_1) + (char)(con->fetch_data);
    }

    int z = (val & 0xFF) == 0;
    int h = (cpu_read_reg(con->cur_inst->reg_1) & 0xF) + 
    (con->fetch_data & 0xF) >= 0x10;
    int c = (int)(cpu_read_reg(con->cur_inst->reg_1) & 0xFF) +
    (int)(con->fetch_data & 0xFF) >= 0x100;

    if (is_16bit)
    {
        z = -1;
        h = (cpu_read_reg(con->cur_inst->reg_1) & 0xFFF) +
        (con->fetch_data & 0xFFF) >= 0x1000;
        u32 n = ((u32)cpu_read_reg(con->cur_inst->reg_1)) + ((u32)con->fetch_data);
        c = n >= 0x10000;
    }

    if (con->cur_inst->reg_1 == RT_SP)
    {
        z = 0;
         int h = (cpu_read_reg(con->cur_inst->reg_1) & 0xF) + 
        (con->fetch_data & 0xF) >= 0x10;
        int c = (int)(cpu_read_reg(con->cur_inst->reg_1) & 0xFF) +
        (int)(con->fetch_data & 0xFF) >= 0x100;

    }

    cpu_set_reg(con->cur_inst->reg_1, val & 0xFFFF);
    cpu_set_flags(con, z, 0, h, c);
}

IN_PROC processors[] =
{
    [IN_ADD] = proc_add,
    [IN_ADC] = proc_adc,
    [IN_AND] = proc_and,
    [IN_CALL] = proc_call,
    [IN_CB] = proc_cb,
    [IN_CP] = proc_cp,
    [IN_DI] = proc_di,
    [IN_DEC] = proc_dec,
    [IN_INC] = proc_inc,
    [IN_JP] = proc_jp, 
    [IN_JR] = proc_jr,
    [IN_LD] = proc_ld,
    [IN_LDH] = proc_ldh,
    [IN_NONE] = proc_none, //  
    [IN_NOP] = proc_nop,
    [IN_OR] = proc_or,
    [IN_POP] =  proc_pop,
    [IN_PUSH] = proc_push,
    [IN_RET] = proc_ret,
    [IN_RETI] = proc_reti,
    [IN_RLA] = proc_rla,
    [IN_RLCA] = proc_rlca,
    [IN_RRA] = proc_rra,
    [IN_RRCA] = proc_rrca,
    [IN_RST] = proc_rst,
    [IN_SBC] = proc_sbc,
    [IN_SUB] = proc_sub, 
    [IN_STOP] = proc_stop,
    [IN_XOR] = proc_xor
};

IN_PROC inst_get_processor(in_type type)
{
    return processors[type];
}


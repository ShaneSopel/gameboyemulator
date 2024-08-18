#ifndef CPU_PROC_H
#define CPU_PROC_H

#define CPU_FLAG_Z BIT(con->regs.f, 7)
#define CPU_FLAG_N BIT(con->regs.f, 6)
#define CPU_FLAG_H BIT(con->regs.f, 5)
#define CPU_FLAG_C BIT(con->regs.f, 4)

#include<common.h>
#include<instructions.h>

static void proc_none(cpu_context *con);

static void proc_di(cpu_context *con);

static bool is_16_bit(reg_type rt);

static void proc_nop(cpu_context *con);

static void proc_ld(cpu_context *con);

static void proc_ldh(cpu_context *con);

reg_type decode_reg(u8 reg);

static void proc_rlca(cpu_context *con);

static void proc_stop(cpu_context *con);

static void proc_daa(cpu_context *con);

static void proc_cpl(cpu_context *con);

static void proc_scf(cpu_context *con);

static void proc_ccf(cpu_context *con);

static void proc_halt(cpu_context *con);

static void proc_rra(cpu_context *con);

static void proc_rla(cpu_context *con);

static void proc_rrca(cpu_context *con);

static void proc_and(cpu_context *con);

static void proc_xor(cpu_context *con);

static void proc_or(cpu_context *con);

static void proc_cp(cpu_context *con);

static void proc_cb(cpu_context *con);

static void goto_addr(cpu_context *con, u16 addr, bool pushpc);

void cpu_set_flags(cpu_context *con, char z, char n, char h, char c);

static void proc_xor(cpu_context *con);

static bool check_cond(cpu_context *con);

static void proc_jp(cpu_context *con);

static void proc_jr(cpu_context *con);

static void proc_call(cpu_context *con);

static void proc_rst(cpu_context *con);

static void proc_ret(cpu_context *con);

static void proc_reti(cpu_context *con);

static void proc_pop(cpu_context *con);

static void proc_push(cpu_context *con);

static void proc_dec(cpu_context *con);

static void proc_inc(cpu_context *con);

static void proc_sub(cpu_context *con);

static void proc_sbc(cpu_context *con);

static void proc_add(cpu_context *con);

static void proc_adc(cpu_context *con);

typedef void (*IN_PROC)(cpu_context *);

IN_PROC inst_get_processor(in_type type);

#endif
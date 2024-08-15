#ifndef CPU_PROC_H
#define CPU_PROC_H

#include<common.h>
#include<instructions.h>

static void proc_none(cpu_context *con);

static void proc_di(cpu_context *con);

static void proc_nop(cpu_context *con);

static void proc_ld(cpu_context *con);

void cpu_set_flags(cpu_context *con, char z, char n, char h, char c);

static void proc_xor(cpu_context *con);

static bool check_cond(cpu_context *con);

static void proc_jp(cpu_context *con);

typedef void (*IN_PROC)(cpu_context *);

IN_PROC inst_get_processor(in_type type);


#endif
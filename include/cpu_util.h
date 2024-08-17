#ifndef CPU_UTIL_H
#define CPU_UTIL_H

#include <common.h>
#include <cpu.h>

u16 cpu_read_reg(reg_type rt);

void cpu_set_reg(reg_type rt, u16 val);

cpu_registers *cpu_get_regs();

#endif
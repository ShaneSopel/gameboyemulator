#ifndef CPU_UTIL_H
#define CPU_UTIL_H

#include <bus.h>
#include <common.h>
#include <instructions.h>

u16 cpu_read_reg(reg_type rt);

void cpu_set_reg(reg_type rt, u16 val);

cpu_registers *cpu_get_regs();

u8 cpu_read_reg8(reg_type rt);
void cpu_set_reg8(reg_type rt, u8 value);

#endif
#include <cpu.h>
#include <cpu_util.h>

extern cpu_context con;

u16 reverse(u16 n) {
    return ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8);
}

// will read reg type. 
u16 cpu_read_reg(reg_type rt) 
{
    switch(rt) {

        //will return the register based on the 
        // enum 
        case RT_A: return con.regs.a;
        case RT_F: return con.regs.f;
        case RT_B: return con.regs.b;
        case RT_C: return con.regs.c;
        case RT_D: return con.regs.d;
        case RT_E: return con.regs.e;
        case RT_H: return con.regs.h;
        case RT_L: return con.regs.l;

        // will return the 16 bit register based on the 
        // enum.
        case RT_AF: return reverse(*((u16 *)&con.regs.a));
        case RT_BC: return reverse(*((u16 *)&con.regs.b));
        case RT_DE: return reverse(*((u16 *)&con.regs.d));
        case RT_HL: return reverse(*((u16 *)&con.regs.h));

        // pc and sp register return.
        case RT_PC: return con.regs.pc;
        case RT_SP: return con.regs.sp;
        default: return 0;
    }
}

void cpu_set_reg(reg_type rt, u16 val) 
{
    switch(rt) {
        // will set the reg based on the enum and
        // will AND the value for 16 bit functionality.
        case RT_A: con.regs.a = val & 0xFF; break;
        case RT_F: con.regs.f = val & 0xFF; break;
        case RT_B: con.regs.b = val & 0xFF; break;
        case RT_C: {
             con.regs.c = val & 0xFF;
        } break;
        case RT_D: con.regs.d = val & 0xFF; break;
        case RT_E: con.regs.e = val & 0xFF; break;
        case RT_H: con.regs.h = val & 0xFF; break;
        case RT_L: con.regs.l = val & 0xFF; break;

        case RT_AF: *((u16 *)&con.regs.a) = reverse(val); break;
        case RT_BC: *((u16 *)&con.regs.b) = reverse(val); break;
        case RT_DE: *((u16 *)&con.regs.d) = reverse(val); break;
        case RT_HL: {
         *((u16 *)&con.regs.h) = reverse(val); 
         break;
        }

        case RT_PC: con.regs.pc = val; break;
        case RT_SP: con.regs.sp = val; break;
        case RT_NONE: break;
    }
}

u8 cpu_read_reg8(reg_type rt) {
    switch(rt) {
        case RT_A: return con.regs.a;
        case RT_F: return con.regs.f;
        case RT_B: return con.regs.b;
        case RT_C: return con.regs.c;
        case RT_D: return con.regs.d;
        case RT_E: return con.regs.e;
        case RT_H: return con.regs.h;
        case RT_L: return con.regs.l;
        case RT_HL: {
            return bus_read(cpu_read_reg(RT_HL));
        }
        default:
            printf("**ERR INVALID REG8: %d\n", rt);
            NO_IMPL
    }
}

void cpu_set_reg8(reg_type rt, u8 val) {
    switch(rt) {
        case RT_A: con.regs.a = val & 0xFF; break;
        case RT_F: con.regs.f = val & 0xFF; break;
        case RT_B: con.regs.b = val & 0xFF; break;
        case RT_C: con.regs.c = val & 0xFF; break;
        case RT_D: con.regs.d = val & 0xFF; break;
        case RT_E: con.regs.e = val & 0xFF; break;
        case RT_H: con.regs.h = val & 0xFF; break;
        case RT_L: con.regs.l = val & 0xFF; break;
        case RT_HL: bus_write(cpu_read_reg(RT_HL), val); break;
        default:
            printf("**ERR INVALID REG8: %d\n", rt);
            NO_IMPL
    }
}

cpu_registers *cpu_get_regs()
{
    return &con.regs;
}

#include <bus.h>
#include <cpu.h>
#include <cpu_fetch.h>

extern cpu_context con;

void fetch_data()
{
    con.mem_dest=0;
    con.dest_is_mem = false;

    if(con.cur_inst == NULL)
    {
        printf("current instruction is NULL");
        return;
    }

    // a switch for the instruction address mode.
    // The address mode is who is talking to who.
    // we have reg to reg
    // reg to A8/D8 mem address
    // reg to memory reg.
    // reg to A16 / D16 , where we have to do bitwise functions
    // to shift the higher 8 bits >> to the right. 
    // adding PC program counter and emulation cycles, 
    // when functionality is present. 
    // you have your HL Reg increment / decrement.
    switch(con.cur_inst->mode)
    {
	case AM_IMP: return;

        // fetch data from a register
        case AM_R:
        {
            con.fetch_data = cpu_read_reg(con.cur_inst->reg_1);
            return;
        }

        // fetch data from a register and place it into reg 2.
        case AM_R_R:
        {
            con.fetch_data = cpu_read_reg(con.cur_inst->reg_2);
            return;
        }

        // load data from a 8 bit address to a reg.
        case AM_R_D8:
        {
            con.fetch_data = bus_read(con.regs.pc);
            emu_cycles(1);
            con.regs.pc++;
            return;
        }
        
        // load from address 16 bit to a reg
        case AM_R_D16:
        // load from 16 bit address.
        case AM_D16:
        {
            //printf("current instruction is AM_R_D16 or AM_D16");
            u16 lo = bus_read(con.regs.pc);
            emu_cycles(1);

            u16 hi = bus_read(con.regs.pc + 1);
            emu_cycles(1);

            con.fetch_data = lo | (hi << 8);

            con.regs.pc += 2;

            return;
        }

        // load from a reg to a register memory region.
        case AM_MR_R:
        {
            con.fetch_data = cpu_read_reg(con.cur_inst->reg_2);
            con.mem_dest = cpu_read_reg(con.cur_inst->reg_1);
            con.dest_is_mem = true;

            if(con.cur_inst->reg_1 == RT_C)
            {
                con.mem_dest |= 0xFF00;
            }

            return;
        }

        // register memory address register to a register
        case AM_R_MR:
        {
            u16 addr = cpu_read_reg(con.cur_inst->reg_2);

            if(con.cur_inst->reg_2 == RT_C)
            {
                addr |= 0xFF00;
            }

            con.fetch_data = bus_read(addr);
            emu_cycles(1);

            return;
        }
        
        // load to a 8 bit register from a 16 bit HL register (increment)
        case AM_R_HLI:
        {
            con.fetch_data = bus_read(cpu_read_reg(con.cur_inst->reg_2));
            emu_cycles(1);
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
            return;
        }

        // load to a 8 bit register from a 16 bit HL register (decrement)
        case AM_R_HLD:
        {
            con.fetch_data = bus_read(cpu_read_reg(con.cur_inst->reg_2));
            emu_cycles(1);
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
            return;
        }
        
        // load to an absolute addressed specficed HL from a 8 bit register (increment)
        case AM_HLI_R:
        {
            con.fetch_data = cpu_read_reg(con.cur_inst->reg_2);
            con.mem_dest = cpu_read_reg(con.cur_inst->reg_1);
            con.dest_is_mem = true;
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
            return;
        }

        // load to an absolute addressed specficed HL from a 8 bit register (decrement)
        case AM_HLD_R:
        {
            con.fetch_data = cpu_read_reg(con.cur_inst->reg_2);
            con.mem_dest = cpu_read_reg(con.cur_inst->reg_1);
            con.dest_is_mem = true;
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
            return;
        }

        case AM_R_A8:
        {
            con.fetch_data = bus_read(con.regs.pc);
            emu_cycles(1);
            con.regs.pc++;
            return;
        }

        case AM_A8_R:
        {
            con.mem_dest = bus_read(con.regs.pc) | 0xFF00;
            con.dest_is_mem = true;
            emu_cycles(1);
            con.regs.pc++;
            return;
        }

        case AM_HL_SPR:
        {
            con.fetch_data = bus_read(con.regs.pc);
            emu_cycles(1);
            con.regs.pc++;
            return;
        }

        // load from a 8 bit address
        case AM_D8:
        {
            con.fetch_data = bus_read(con.regs.pc);
            emu_cycles(1);
            con.regs.pc++;
            return;
        }

         //load from register into address A16
        case AM_A16_R:
        case AM_D16_R:
        {
            u16 lo = bus_read(con.regs.pc);
            emu_cycles(1);

            u16 hi = bus_read(con.regs.pc + 1);
            emu_cycles(1);

            con.mem_dest = lo | (hi << 8);
            con.dest_is_mem = true;

            con.regs.pc += 2;
            con.fetch_data = cpu_read_reg(con.cur_inst->reg_2);

            return;
        }

        case AM_MR_D8:
        {
            con.fetch_data = bus_read(con.regs.pc);
            emu_cycles(1);
            con.regs.pc++;
            con.mem_dest = cpu_read_reg(con.cur_inst->reg_1);
            con.dest_is_mem = true;
            return;
        }

        case AM_MR:
        {
            con.mem_dest = cpu_read_reg(con.cur_inst->reg_1);
            con.dest_is_mem = true;
            con.fetch_data = bus_read(cpu_read_reg(con.cur_inst->reg_1));
            emu_cycles(1);
            return;
        }

        //load from address A16 to register
        case AM_R_A16:
        {
             u16 lo = bus_read(con.regs.pc);
            emu_cycles(1);

            u16 hi = bus_read(con.regs.pc + 1);
            emu_cycles(1);

            u16 addr = lo | (hi << 8);

            con.regs.pc += 2;
            con.fetch_data = bus_read(addr);
            emu_cycles(1);

            return;
        }
            
        default:
            printf("Unknown Addressing Mode: %d (%02X)\n", con.cur_inst->mode, con.cur_opcode);
            exit(-7);
            return;
    }
   
}

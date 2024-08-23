#include <interrupts.h>


void int_handle(cpu_context *con, u16 address)
{
    stack_push16(con->regs.pc);
    con->regs.pc = address;
}

bool int_check(cpu_context *con, u16 address, interrupt_type t)
{
    if(con->int_flags & t && con->ie_register & t)
    {
        int_handle(con, address);
        con->int_flags &= ~t;
        con->halted = false;
        con->int_master_enabled = false;

        return true;
    }
    return false;
}


void cpu_handle_interrupts(cpu_context *con)
{
    if(int_check(con, 0x40, IT_VBLANK))
    {

    }
    else if (int_check(con, 0x48, IT_LCD_STAT))
    {
        /* code */
    }
    else if (int_check(con, 0x50, IT_TIMER))
    {
        /* code */
    }
    else if (int_check(con, 0x58, IT_SERIAL))
    {
        /* code */
    }
    else if (int_check(con, 0x60, IT_JOYPAD))
    {
        /* code */
    }
}

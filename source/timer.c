#include <timer.h>
#include <interrupts.h>



static timer_context con = {0};

timer_context *timer_get_context()
{
    return &con;
}

void timer_init()
{
    con.div = 0xAC00;
}

// what happens on a timer tick

void timer_tick()
{
    u16 prev_div = con.div;

    con.div++;

    bool timer_update = false;

    //where we update the tac's clock select
    // tac & with 0b11
    switch(con.tac & (0b11))
    {
        // if clock select case is equal to 0b00
        case 0b00:
            // the previous div grabbing the 9th bit, and if the 9th bit is not set on the new value.
            // that means there is a new timer value.
            // essentially how we divided 4194304 by 1024.
            timer_update = (prev_div & (1 << 9)) && (!(con.div & (1 << 9)));
            break;
            // the previous div grabbing the 3rd bit, and if the 3rd bit is not set on the new value.
            // that means there is a new timer value.
            // essentially we divded 4194304 by 16 
         case 0b01:
            timer_update = (prev_div & (1 << 3)) && (!(con.div & (1 << 3)));
            break;
         case 0b10:
            // the previous div grabbing the 5th bit, and if the 5th bit is not set on the new value.
            // that means there is a new timer value.
            // esentially we divided 4194304 by 64
            timer_update = (prev_div & (1 << 5)) && (!(con.div & (1 << 5)));
            break;
         case 0b11:
            // the previous div grabbing the 5th bit, and if the 5th bit is not set on the new value.
            // that means there is a new timer value.
            // esentially we divided 4194304 by 256 
            timer_update = (prev_div & (1 << 7)) && (!(con.div & (1 << 7)));
            break;
    }

   // if the timer needs to be updated and the timer is enabled. 
    if (timer_update && con.tac & (1 << 2))
    {
        // increment the TIMA value
        con.tima++;

        // if the TIMA value is 0xFF
        if (con.tima == 0xFF)
        {
            // then we need to reset the TIMA value to the 
            // TMA value 
            con.tima = con.tma;

            // and we need to request an interrupt. 
            cpu_request_interrupt(IT_TIMER);

            // this all follows the logit in the header file.
            // it follows the registers instructions
            // of TIMA and TMA.
        }

    }
}

void timer_write(u16 address, u8 value)
{
    switch (address)
    {
        case 0xFF04:
            //DIV
            con.div = 0;
            break;
        
        case 0xFF05:
            //TIMA
            con.tima = value;
            break;

        case 0xFF06:
            //TMA
            con.tma = value;
            break;
        
        case 0xFF07:
            //TAC
            con.tac = value;
            break;
    }
}

u8 timer_read(u16 address)
{
    switch (address)
    {
        case 0xFF04:
            return con.div >> 8;
        case 0xFF05:
            return con.tima;
        case 0xFF06:
            return con.tma;
        case 0xFF07:
            return con.tac;            
    }
}


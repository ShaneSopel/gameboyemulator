#include <stack.h>

/*
    STACK

    // the stack pointer is set to the bottom of the working RAM
    // this is done via a load call. 
    SP=0xDFFF

    MEMORY:
    0xDFF7: 00
    0xDFF8: 00
    0xDFF9: 00
    0xDFFA: 00
    0xDFFB: 00
    0xDFFC: 00
    0xDFFD: 00
    0xDFFE: 00
    0xDFFF: 00 <- SP

    // if we want to push to 0x55
    PUSH 0x55
    

    // the first thing we wwould do is stack pointer minus minus
    SP-- = 0xDFFE
    this sets it done to DFFE
    and set it to value 0x55
    MEMORY[0xDFFE] = 0x55

    MEMORY:
    0xDFF7: 00
    0xDFF8: 00
    0xDFF9: 00
    0xDFFA: 00
    0xDFFB: 00
    0xDFFC: 00
    0xDFFD: 00
    0xDFFE: 55 <- SP
    0xDFFF: 00

    // if we want to do this again for a value of 0x77
    
    PUSH 0x77
    // we decrement the value to 0xDFFD
    SP-- = 0xDFFD
    // then we store 0x77 at 0xDFFD
    MEMORY[0xDFFD] = 0x77

    MEMORY:
    0xDFF7: 00
    0xDFF8: 00
    0xDFF9: 00
    0xDFFA: 00
    0xDFFB: 00
    0xDFFC: 00
    0xDFFD: 77 <- SP
    0xDFFE: 55
    0xDFFF: 00

    // if we want to POP the value
   
    val = POP
    // we would have to do get the value of the memory location
    // then that would get us the value stored there 
    val = MEMORY[0xDFFD] = 0x77
    // increment the stack pointer. 
    SP++ = 0xDFFE

    MEMORY:
    0xDFF7: 00
    0xDFF8: 00
    0xDFF9: 00
    0xDFFA: 00
    0xDFFB: 00
    0xDFFC: 00
    0xDFFD: 77 
    0xDFFE: 55 <- SP
    0xDFFF: 00

    // since we just popped the value 
    // when we push 0x88 into the memory
    // it will use the decrement value that 0x77
    // previously used. 
    PUSH 0x88

    SP-- = 0xDFFD
    MEMORY[0xDFFD] = 0x88

    MEMORY:
    0xDFF7: 00
    0xDFF8: 00
    0xDFF9: 00
    0xDFFA: 00
    0xDFFB: 00
    0xDFFC: 00
    0xDFFD: 88 <- SP
    0xDFFE: 55 
    0xDFFF: 00
*/

void stack_push(u8 data)
{
    cpu_get_regs()->sp--;
    bus_write(cpu_get_regs()->sp, data);
}
void stack_push16(u16 data)
{
    stack_push((data >> 8)& 0xFF);
    stack_push(data & 0xFF);
}

u8 stack_pop()
{
    return bus_read(cpu_get_regs()->sp++);
}

u16 stack_pop16()
{
    u16 lo = stack_pop();
    u16 hi = stack_pop();

    return (hi << 8) | lo;
}
# Lab 06 Report
<p>Name: <b>Jace Kline</b></p>
<p>KUID: 2881618</p>
<p>Lab Section: Monday, 2:00-3:50 PM</p>

## Title: Interrupts

## Quiz Question
*Q: How long (in years) will it take to overflow the 64bit timer? Assume the counter begins
from zero at reset, and is incremented at the speed of 32678Hz.*
- **Answer: 17900190.26 years**

## Procedure
1. Download Source archive and extract the tar file a folder
2. Open VSCode
3. Open the extracted folder as a PlatformioIDE project
4. Plug in the HiFive1 Microcontroller via the USB port to the development PC
5. Follow the lab instructions to connect the servo to the HiFive board
6. Read the source code for the library and main execution code
7. Implement the 'timer_handler()' function
   * This is where we set the mtimecmp to essentially define how long we want until the next interrupt occurs (100ms)
   * mtimecmp = mtime + (clock ticks required equivalent to 100ms)
   * Uses the 'set_cycles()' and 'get_cycles()' functions to directly update the mtimecmp register
   * When the value in the mtime register becomes greater than or equal to the value in the mtimecmp register, an interrupt automatically occurs
```c
void timer_handler()
{
    intr_count++;

    /* 
    Set the mtimecmp register to the value of the 
    mtime register + the cycles required to make the
    delay time to be 100ms before the next interrupt.
    */
    set_cycles(get_cycles() + (100*(32768/1000)));
}
```
8. Implement the 'enable_interrupt()' function
   * This function is what enables the 3rd bit inside the 'mstatus' register to signal that machine interrupts are enabled
   * We do this by using a logical OR with the 3rd bit in the mstatus register
```c
void enable_interrupt()
{
    /*
    Turns on the required bit (bit 3) in the mstatus register to
    enable machine interrupts.
    */
    write_csr(mstatus, read_csr(mstatus) | (1 << MSTATUS_MIE_BIT));
}
```
9. Implement the 'disable_interrupt()' function
   * This function disables the bit in the mstatus register that is responsible for signaling on the machine interrupts
   * Check whether the bit is already a 1
   * If the bit is 1, then disable the bit using a logical AND between the original value with the original value - 8.
```c
void disable_interrupt()
{
    /*
    Checks to see if the 3rd bit in the mstatus register is on, 
    then disables the bit. Otherwise, it does nothing.
    */
    if((read_csr(mstatus) & (1 << MSTATUS_MIE_BIT) >> MSTATUS_MIE_BIT)) {
        write_csr(mstatus, read_csr(mstatus) & (read_csr(mstatus) - (1 << MSTATUS_MIE_BIT)));
    }
}
```
10. Upload the code to the HiFive board and run
11. Desired behavior: The LED blinks at 100ms intervals and switches between blue and green every 5 blinks.


## Conclusion

In this lab, I learned about the topic of interrupts. This lab helped me to better understand their structure and significance in computing. I will summarize the following topics:
1. What interrupts are
2. Types of interrupts
3. Benefits and Problems of interrupts

Interrupts are a concept of an asynchronus and unpredictable "interrupt" in the current CPU process, which must be immediately handled. Each interrupt defined in any layer of the hardware or software must have a corresponding *handler* procedure, stored in memory, that is loaded and executed when the interrupt occurs. On each interrupt that is handled by the CPU, the entire working memory and state of the current program must be copied to another memory location. After this occurs, the interrupt handling procedure, often called an interrupt service routine (ISR), is loaded and executed by the CPU. Following this, the main program's last state is then re-loaded back into the CPU's working memory where the CPU resumes its execution of the program. Interrupts are often used to model signals, events, or queries that are asynchronous and seperate from the sequence of instructions defined for the main program.

There are three major types of interrupts. The first type is called an external interrupt. This type usually pertains to an interrupt triggered by an external I/O device when a specific signal needs to be transmitted to the host CPU. The second type of interrupt is called a hardware interrupt. This type of interrupt often occurs when the machine is in an undesireable state (like dividing by zero, etc.), but it occurs at a level below the software. The last major type of interrupt is called a software interrupt. These interrupts, their triggers, and their handlers are defined within software itself.

The concept of the interrupt is very controversial. In some cases it can be beneficial, but in other cases it can cause a variety of problems and edge cases. Interrupts are sometimes useful for periodically receiving external asynchronous signals and other similar tasks that are hard to model in sequential code. However, introducing interrupts in the program allows for non-determinism in timing and behavior, which drastically increases the complexity and number of edge cases in the program. Along with this, it adds the issues of concurrency and data races. Lastly, interrupts require the time overhead of copying the current program state to another memory location and then re-loading the state on every single interrupt occurrence. If these interrupts occur often, this can be very inefficient.

# Lab 03 Report
<p>Name: Jace Kline</p>
<p>KUID: 2881618</p>
<p>Lab Section: Monday, 2:00-3:50 PM</p>

## Title: UART Communication
## Procedure
1. Download Source archive and extract the tar file a folder
2. Open VSCode
3. Open the extracted folder as a PlatformioIDE project
4. Plug in the HiFive1 Microcontroller via the USB port to the development PC
5. Read over the library source code in the eecs388_lib.h and eecs388_lib.c to understand the ser_setup(), ser_write(), and ser_printline() functions that will help us implement the ser_read() function.
6. Read over the manuals and memory maps that specify the memory locations we need and the mapping of that memory
7. Implement the ser_read() function
   * Create the main loop that repeats until a bit is read from the queue rxdata queue
   * We know that readable data is available in the queue when the 31st bit of the rxdata register has a value of 0.
   * On the contrary, if the 31st bit of the rxdata register is a 1, then that signals the queue is empty.
   * Therefore, we loop until the value of the 31st bit in the register is a 0.
   * When the loop exits, the data was is in our variable. This data came from the derefereneced pointer address of the rxdata register.
   * Finally, we return the captured value from the register.
```c
char ser_read()
{ 
  // A variable to store the dereferenced data value
  uint32_t receive_val;
  
  // the looping condition is true when the 31st bit in our received data is true
  // this indicates that the queue is empty and therefore we repeat receiving the data
  do {
    // the value is extracted from the dereferenced pointer of the rxdata register
    receive_val = *(volatile uint32_t *)(UART0_CTRL_ADDR + UART_RXDATA);
  } while(receive_val & 0x80000000); 

  //return the stored value when the queue is no longer empty
  return(receive_val); 
}
```
8. Build and Upload the instructions to the board
9. Open the serial terminal inside Platformio and test each of the inputs 'r', 'g', and 'b'
   * If the LED corresponding to the character lights up based on the entered input for all inputs, then the ser_read() function was implemented correctly.

## Conclusion

<p>In this lab, I learned about the following main concepts:</p>
1. Serial Connections
   * Reading (rxdata)
   * Transmitting / Writing (txdata)
2. Registers
3. Memory Maps
I will expand on each of these concepts.

A serial connection is a connection between two devices where the data is communicated over a single channel one bit at a time. This differs from parallel connections, where multiple parallel channels are set up between the two devices.

Registers


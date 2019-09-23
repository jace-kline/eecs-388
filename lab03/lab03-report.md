# Lab 03 Report
<p>Name: <b>Jace Kline</b></p>
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
   * We know that readable data is available in the queue when the 31st bit of the rxdata block has a value of 0.
   * On the contrary, if the 31st bit of the rxdata memroy block is a 1, then that signals the queue is empty.
   * Therefore, we loop until the value of the 31st bit in the register is a 0.
   * When the loop exits, the data was is in our variable. This data came from the derefereneced pointer address of the rxdata block in memory.
   * Finally, we return the captured value from that dereferenced memory.
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
2. Volatile vs Non-volatile memory
3. Memory Maps

I will expand on each of these concepts.

A serial connection is a connection between two devices where the data is communicated over a single channel (one way) one bit at a time. In the particular use case for lab 3 with the UART0, there are two channels of communication. The first channel is for writing data from our computer to the HiFive board. This corresponds to the ser_write() function. The other channel is for reading data provided by the HiFive board to our computer. This corresponds to the ser_read() function. Together, these two channels allow the two devices transmit data both ways between our PC and the HiFive board.

In this lab and in lecture, I have also learned the meaning of the 'volatile' keyword in C, as well as the meaning of volatile vs non-volatile memory. From a memory standpoint, volatile memory is memory that is not stored when the computational device loses power. On the contrary, non-volatile memory is persistent when the power is shut off. 

In C, we have to use the 'volatile' keyword when defining the pointers to the memory addresses used by the UART0 data queues because the values in these memory locations can unpredictably change, due to these memory locations being used for serial transmission between different devices. Hence, we use the 'volatile' keyword upon declaration of a pointer assignment to signal to the compiler that there is unpredictability at the target memory address and to avoid optimization.

At a high level, a memory map is a structure that is defined to map specific sections of memory to different purposes like registers, peripheral devices, etc. In lab 3, we had to read the provided manual to determine the memory addresses used for our UART0 device. Further, we had to determine which parts of the UART0 memory chunk was used for what purpose/signal. By reading the memory map (manual), we can figure out how to write C code to interact with our desired device. Digging deeper into memory maps, the concept of memory maps can actually be extended to multiple abstraction layers within the hardware and software stack, manifesting in different forms at each layer. 


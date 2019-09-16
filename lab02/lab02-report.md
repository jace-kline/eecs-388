# Lab 02 Report

<p>Name: Jace Kline</p>
<p>KUID: 2881618</p>
<p>Lab Section: Monday, 2:00-3:50 PM</p>

## Section 1: Quiz Questions
1. Convert the following numbers in to decimal:
   * 101<sub>2</sub> = 5<sub>10</sub>
   * 1111<sub>2</sub> = 15<sub>10</sub>
   * 11<sub>16</sub> = 17<sub>10</sub>
   * 1f<sub>16</sub> = 31<sub>10</sub>
2. What will be the output of the following code. Why?
```c
uint8_t var = 256;
printf("%d\n", var);
```
The output will be **0** because the integer type is 8-bit, fixed, and unsigned. 256 is the next number after the maximum that the uint8_t type can hold (255). Because it is unsigned, it will wrap the value 256 to 0.

3. Suppose val = 0 and gpio = 3. What is the value of 'val' after the operation `val |= (1<<gpio);`? (Binary and Hexadecimal)
   * Answer: **val = 1000<sub>2</sub> = 8<sub>16</sub>**
4. Suppose val = 8 and gpio = 3. What is the value of 'val' after the operation `val &= (~(1<<gpio));`? (Binary and Hexadecimal)
   * Answer: **val = 0<sub>2</sub> = 0<sub>16</sub>**

## Section 2: Report
### Title: RGB Blinky LED Lab
### Procedure:
1. Download Source archive and extract the tar file a folder
2. Open VSCode
3. Open the extracted folder as a PlatformioIDE project
4. Plug in the HiFive1 Microcontroller via the USB port to the development PC
5. Read over the library source code in the eecs388_lib.h and eecs388_lib.c to understand the 'gpio' functions that will be used in our code
6. Implement the desired program functionality in the eecs388_blink.c file (Part 1)
    * Initial Functionality: The green LED blinks by being on for 1000 ms, then off for 300 ms. (from lab 01)
    * Desired Functionality: The red, green, and blue LED lights alternate blinking in a loop. The time per blink is 500 ms on, followed by 300 ms off.
      * Create an update LED color function based on the current color given as an argument.
        * red -> green
        * green -> blue
        * otherwise -> red
      * Change the delay periods from the defaults in lab01, per the instructions
        * 1000 ms ON changed to 500 ms ON
      * Update the gpio color encoding at the end of each loop using the updateGPIO function
      * Update the gpio mode using the changed color from above
```c
//returns the next color's integer code based on the previous color's code
int updateGPIO(int lastGPIO) {
  switch (lastGPIO) {
    case RED_LED: return(GREEN_LED);
    case GREEN_LED: return(BLUE_LED);
    default: return(RED_LED);
  }
  return(GREEN_LED);
}

int main()
{
  int gpio = GREEN_LED;

  gpio_mode(gpio, OUTPUT);
  while(1) {
       gpio_write(gpio, ON);
       
       //changed from 1000 ms (lab 01) to 500 ms 
       delay(500);                  
       gpio_write(gpio, OFF);
       delay(300);
       
       //reassigns the gpio int to be the next color based on current color
       gpio = updateGPIO(gpio);
       
       //updates the gpio_mode to use the newly-assigned color
       gpio_mode(gpio, OUTPUT);     
  }
}
```
7. Add the feature of a white blink every fourth blink to the above code
    * Determine that a white blink corresponds to red, green, and blue LEDs blinking simultaneously
    * Refactor the previous code in to a sub-loop that runs for each color (3 iterations)
    * Add the functionality of the white light in the outer 'while' loop
      * To turn on all 3 lights, use 'gpio_write' function with and ON argument for all 3 colors
      * Leave all LEDs on for 500 ms
      * Turn all 3 lights off using the 'gpio_write' function with an OFF second argument
      * Leave off for 300 ms
```c
int updateGPIO(int lastGPIO) {
   switch (lastGPIO) {
      case RED_LED: return(GREEN_LED);
      case GREEN_LED: return(BLUE_LED);
      default: return(RED_LED);
   }
   return(GREEN_LED);
}

int main()
{
    int gpio = GREEN_LED;

    gpio_mode(gpio, OUTPUT);
    
    while(1) {
    
      // Loop where each color blinks once
      for(int i = 0; i < 3; i++)
      {
         gpio_write(gpio, ON);
         delay(500);
         gpio_write(gpio, OFF);
         delay(300);

         gpio = updateGPIO(gpio);
         gpio_mode(gpio, OUTPUT);
         
      }
      
      // All LEDs ON for 500 ms
      gpio_write(GREEN_LED, ON);
      gpio_write(BLUE_LED, ON);
      gpio_write(RED_LED, ON);
      delay(500);
      
      // All LEDs OFF for 300 ms
      gpio_write(GREEN_LED, OFF);
      gpio_write(BLUE_LED, OFF);
      gpio_write(RED_LED, OFF);
      delay(300);
    }
}
```

### Conclusion

In this lab, I became more familiar with various aspects of C Programming, PlatformioIDE, and foundational concepts useful in low-level programming. In higher-level languages, the notion of memory management is hidden (for the most part). However, in C, the programmer is responsible for the management of raw signals, primitive data, and memory which brings on more complications, but also more control.

In particular, I learned about the following C Programming topics during the lab:
* Number Systems
* Data Types
* Endianness
* Operators
I will now discuss each of these in further detail.

Number systems are a way to represent numbers with respect to a given number base. For example, binary (base 2), octal (base 8), decimal (base 10), and hexadecimal (base 16) are all number systems that can represent the quantitity of any number. With respect to C programming, we often use hexadecimal number representations rather than decimal representations because the time and memory cost of the conversion between hexadecimal and binary representations is much lower. This is because 2^4 = 16, and therefore the hexadecimal digits can be translated digit-by-digit in sequential order and concattenated in to a binary number. In C, binary numbers are represented by a '0b' at the beginning and hexadecimal numbers are represented by '0x' at the beginning. Examples...
```c
int bin = 0b1000;
int hex = 0xf;
int dec = 15;
```

Data types are the way information is stored and used in a C program. There are 4 basic data types: char, int, float, and double. The number data types can either be signed (the positive/negative is encoded by the first bit) or unsigned (the number is assumed positive and all bits are used for digit storage). Depending on the architecture of the computing machine, the 'int' data type will have 32 bits or 64 bits of storage. For writing code that may be used across different architectures, there are additional fixed-width integer data types that may be imported via stdint.h.

Endianness refers to which 'end' of the data type is stored at which 'end' of a sequential block of memory addresses. There is Little Endian, where the least significant bit (LSB) is stored in the far left memory address. Little Endian basically stores the data "backwards" with respect to how we would read the digits. Big Endian stores the numbers with the most significant bit (MSB) in the first memory address. Our HiFive board and most PCs use Little Endian format.

Operators are the primitive infix functions used to manipulate data in C. There are the following "classes" of operators...
* Arithmetic - use two numbers to produce another number
* Conditional - use values to produce a boolean (0 or 1)
* Logical - use two boolean values to produce another boolean value
* Bitwise - Logical operators applied to each individual bit of the binary representation of a piece of data
* Shift - operators that shift the binary representation of piece of data right or left by a specified amount
* Assignment - Shorthand notations for applying one of the above operations to a piece of data and then overwriting that data

In conclusion, I have gained a solid foundation in the principles discussed in this section. I have also gained more experience programming in C. These combined will help me in future labs.





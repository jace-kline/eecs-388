/**
 * This program read the distance data from TFmini lidar sensor and 
 * print the read data and controls LED depending on the measured distance.
 */
#include <stdint.h>
#include <stdio.h>

#include "eecs388_lib.h"

int main()
{
    gpio_mode(RED_LED, OUTPUT);
    gpio_mode(GREEN_LED, OUTPUT);
    ser_setup();

    ser_printline("Setup completed.\n");

    int dist = 0; // read distance value. 

    while (1) {
        /* 
            Task 1: 
            - read a data frame from the TFmini sensor
            - print the read distance data. e.g., "dist: 45 cm"
              (you can use either printf or sprintf & ser_printline function)
        */
        if ('Y' == ser_read() && 'Y' == ser_read()) {
            // Read the first byte of number data
            int low_data = ser_read();

            // Read the second byte of number data
            int high_data = ser_read();

            /* Shift the bits of the high data bits to the left to allow room for the
               lower 8 bits of data. Then use the bitwise OR operation to combine the 
               two numbers together. The result is stored as 'dist'.
            */
            dist = ((high_data << 8) | low_data);

            //Print the stream of the dist data. One value on each line.
            printf("dist: %d cm\n", dist);
        }

        /* 
            Task 2: 
            - turn on the red light if the distance is less than 50cm. 
            - otherwise turn on the green light 
        */

       // For each condition, turn off the other condition's LED, then
       // turn on the correct condition's LED
        if (dist < 50) {
            gpio_write(GREEN_LED, OFF);
            gpio_write(RED_LED, ON);
        } else {
            gpio_write(RED_LED, OFF);
            gpio_write(GREEN_LED, ON);
        }
    }
}
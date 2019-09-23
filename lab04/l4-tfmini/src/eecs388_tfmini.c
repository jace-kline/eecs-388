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

    int dist; // read distance value. 

    while (1) {
        /* 
            Task 1: 
            - read a data frame from the TFmini sensor
            - print the read distance data. e.g., "dist: 45 cm"
              (you can use either printf or sprintf & ser_printline function)
        */
        if ('Y' == ser_read() && 'Y' == ser_read()) {
            // YOUR CODE HERE
        }

        /* 
            Task 2: 
            - turn on the red light if the distance is less than 50cm. 
            - otherwise turn on the green light 
        */
        if (dist < 50) {
            // YOUR CODE HERE
        } else {
            // YOUR CODE HERE
        }
    }
}
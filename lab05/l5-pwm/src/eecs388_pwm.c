#include <stdio.h>
#include <stdint.h>

#include "eecs388_lib.h"

#define SERVO_PULSE_MAX 2400    /* 2400 us */
#define SERVO_PULSE_MIN 544     /* 544 us */
#define SERVO_PERIOD    20000   /* 20000 us (20ms) */

/**
 * Generate a pwm signal
 * 
 * Input:
 *  @gpio   gpio number
 *  @pos    degree [0,180]
 */
void servo(int gpio, int pos)
{
   // Calculate the amount of time that we must delay with the ON signal
   int on_delay_per_cycle = ((((SERVO_PULSE_MAX - SERVO_PULSE_MIN) / 180) * pos) + SERVO_PULSE_MIN);
   // Write the ON signal
   gpio_write(gpio, ON);
   // Delay for the calculated time (in microseconds (us))
   delay_usec(on_delay_per_cycle);
   
   // Calculate the amount of time to delay with an OFF signal
   int off_delay_per_cycle = SERVO_PERIOD - on_delay_per_cycle;
   // Write the OFF signal
   gpio_write(gpio, OFF);
   // Delay for the calculated time (in microseconds (us))
   delay_usec(off_delay_per_cycle);
}

int main()
{
    int gpio = PIN_19;
    gpio_mode(gpio, OUTPUT);

    while (1) {
        for (int pos = 0; pos <= 180; pos += 30) {
            printf("pos: %d (degree)\n", pos);
            /* control the servor for 1 sec duration */
            for (int i = 0; i < 50; i++)
                servo(gpio, pos);
        }
    }
}
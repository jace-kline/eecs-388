#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "eecs388_lib.h"

#define SERVO_PULSE_MAX 2400    /* 2400 us */
#define SERVO_PULSE_MIN 544     /* 544 us */
#define SERVO_PERIOD    20000   /* 20000 us (20ms) */

void led_red() {
    gpio_mode(GREEN_LED, OUTPUT);
    gpio_write(GREEN_LED, OFF);
    gpio_mode(BLUE_LED, OUTPUT);
    gpio_write(BLUE_LED, OFF);
    gpio_mode(RED_LED, OUTPUT);
    gpio_write(RED_LED, ON);
}

void led_blue() {
    gpio_mode(GREEN_LED, OUTPUT);
    gpio_write(GREEN_LED, OFF);
    gpio_mode(BLUE_LED, OUTPUT);
    gpio_write(BLUE_LED, ON);
    gpio_mode(RED_LED, OUTPUT);
    gpio_write(RED_LED, OFF);
}

void led_green() {
    gpio_mode(GREEN_LED, OUTPUT);
    gpio_write(GREEN_LED, ON);
    gpio_mode(BLUE_LED, OUTPUT);
    gpio_write(BLUE_LED, OFF);
    gpio_mode(RED_LED, OUTPUT);
    gpio_write(RED_LED, OFF);
}

void led_white() {
    gpio_mode(GREEN_LED, OUTPUT);
    gpio_write(GREEN_LED, ON);
    gpio_mode(BLUE_LED, OUTPUT);
    gpio_write(BLUE_LED, ON);
    gpio_mode(RED_LED, OUTPUT);
    gpio_write(RED_LED, ON);
}

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
    // initialize UART channels
    ser_setup(0); // uart0 (debug)
    ser_setup(1); // uart1 (raspberry pi)

    printf("Setup completed.\n");
    printf("Begin the main loop.\n");

    int dist = 0;
    int prev_dist = 0;
    int prev_angle = 0;
    int short_dist = 0;
    char data;
    
    while (1) {
        if ('Y' == ser_read(0) && 'Y' == ser_read(0)) {
            // Read the first byte of number data
            int low_data = ser_read(0);

            // Read the second byte of number data
            int high_data = ser_read(0);

            /* Shift the bits of the high data bits to the left to allow room for the
               lower 8 bits of data. Then use the bitwise OR operation to combine the 
               two numbers together. The result is stored as 'dist'.
            */
            dist = ((high_data << 8) | low_data);

            printf("%d", dist);

            if (dist <= 50) {
                short_dist = 1;
            } 
            else {
                short_dist = 0;
            }
        }

        if(short_dist == 1) {
            led_red();
        }
        else if(ser_isready(1)) {
            data = ser_read(1);
            ser_write(0, data);
            if(data < -30) {
                led_blue();
            } else if(data > 30) {
                led_green();
            } else {
                led_white();
            }
            gpio_mode(PIN_19, OUTPUT);
            servo(PIN_19, data); 
        }
    }
}

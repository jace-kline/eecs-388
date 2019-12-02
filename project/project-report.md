# Final Project Report
<p>Name: <b>Jace Kline</b></p>
<p>KUID: 2881618</p>
<p>Lab Section: Monday, 2:00-3:50 PM</p>

## Title: Project - Self-Driving Car

## Procedure
### Part 1 - HiFive Board
1. Ensure all wire connections from PC to HiFive, Pi 4 to HiFive, and the HiFive to motor are connected correctly
2. Create a VSCode PlatformioIDE project with the library files from Lab10
3. Implement the following constraints for the received steering angle
   * Be able to read the steering angle from the UART1 serial port
   * If the steering angle is less than -30 degrees, turn on blue LED
   * If the steering angle is greater than +30 degrees, turn on the green LED
   * If the steering angle is between -30 degrees and +30 degrees, turn on white LED
4. Implement the following constraints for the received LIDAR distance (in cm)
   * Be able to read the LIDAR distance from the UART0 serial port
   * If distance is greater than 50 cm, continue normal operation
   * If distance is less than 50 cm, turn on red LED and stop the servo motor (regardless of steering angle)
```c
// Constants
#define SERVO_PULSE_MAX 2400    /* 2400 us */
#define SERVO_PULSE_MIN 544     /* 544 us */
#define SERVO_PERIOD    20000   /* 20000 us (20ms) */

// Function to turn LED red
void led_red() {
    gpio_mode(GREEN_LED, OUTPUT);
    gpio_write(GREEN_LED, OFF);
    gpio_mode(BLUE_LED, OUTPUT);
    gpio_write(BLUE_LED, OFF);
    gpio_mode(RED_LED, OUTPUT);
    gpio_write(RED_LED, ON);
}

// Function to turn LED blue
void led_blue() {
    gpio_mode(GREEN_LED, OUTPUT);
    gpio_write(GREEN_LED, OFF);
    gpio_mode(BLUE_LED, OUTPUT);
    gpio_write(BLUE_LED, ON);
    gpio_mode(RED_LED, OUTPUT);
    gpio_write(RED_LED, OFF);
}

// Function to turn LED green
void led_green() {
    gpio_mode(GREEN_LED, OUTPUT);
    gpio_write(GREEN_LED, ON);
    gpio_mode(BLUE_LED, OUTPUT);
    gpio_write(BLUE_LED, OFF);
    gpio_mode(RED_LED, OUTPUT);
    gpio_write(RED_LED, OFF);
}

// Function to turn LED white
void led_white() {
    gpio_mode(GREEN_LED, OUTPUT);
    gpio_write(GREEN_LED, ON);
    gpio_mode(BLUE_LED, OUTPUT);
    gpio_write(BLUE_LED, ON);
    gpio_mode(RED_LED, OUTPUT);
    gpio_write(RED_LED, ON);
}

// Function to supply PWM signal to servo
// Takes arguments of the GPIO pin and the angle to turn
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

// Main program
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
                // Set the signal that RED LED should be on, motor should stop
                short_dist = 1;
            } 
            else {
                // Resume signal
                short_dist = 0;
            }
        }
        
        /* 
        Based on the signal provided, either turns LED red and ignores the motor signal, or checks for the motor signal,
        changes the LED, and signals the motor
        */
        // If LIDAR distance is less than 50 cm
        if(short_dist == 1) {
            led_red();
        }
        // If LIDAR distance is greater than 50 cm, proceed with distance reading and servo control
        else if(ser_isready(1)) {
            data = ser_read(1);
            ser_write(0, data);
            // If steering angle is less than -30, turn on blue LED
            if(data < -30) {
                led_blue();
            // If steering angle greater than 30, turn on green LED
            } else if(data > 30) {
                led_green();
            // Steering angle must be between -30 and 30 -> turn on white LED
            } else {
                led_white();
            }
            // Send signal to the servo based on steering angle data
            gpio_mode(PIN_19, OUTPUT);
            servo(PIN_19, data); 
        }
    }
}
```


## Conclusion


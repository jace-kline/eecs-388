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
### Part 2 - Raspberry Pi 4
1. Change to the Raspberry Pi board and log in
2. Modify the dnn.py from lab 10 to accomplish the following...
   * Show each video frame using a GUI library (reading from video file)
   * On each video frame, display the inference angle computed by the neural network and send the signal to the HiFive board
```python
#!/usr/bin/env python
from __future__ import division

# Imports
import tensorflow as tf
model = __import__("model")
import cv2
import sys
import os
import time
import math
import numpy as np
import serial
import struct

font = cv2.FONT_HERSHEY_SIMPLEX

ser1 = serial.Serial(port='/dev/ttyAMA1', baudrate=115200)
ser2 = serial.Serial(port='/dev/ttyAMA2', baudrate=115200)

# Radian <-> Degree conversion functions
def deg2rad(deg):
        return deg * math.pi / 180.0
def rad2deg(rad):
        return 180.0 * rad / math.pi

#Get and set the number of cores to be used by TensorFlow
if(len(sys.argv) > 1):
        NCPU = int(sys.argv[1])
else:
        NCPU = 1
config = tf.ConfigProto(intra_op_parallelism_threads=NCPU, \
                        inter_op_parallelism_threads=NCPU, \
                        allow_soft_placement=True, \
                        device_count = {'CPU': 1})

#The max number of frames to be processed,
#    and the number of frames already processed
NFRAMES = 1000
curFrame = 0

#Periodic task options
period = 50
is_periodic = True

#Load the model
sess = tf.InteractiveSession(config=config)
saver = tf.train.Saver()
model_load_path = "model/model.ckpt"
saver.restore(sess, model_load_path)

#Create lists for tracking operation timings
cap_time_list = []
prep_time_list = []
pred_time_list = []
tot_time_list = []

print('---------- Processing video for epoch 1 ----------')

#Open the video file
vid_path = '../lab09/l9-dnn/epoch-1.avi'
assert os.path.isfile(vid_path)
cap = cv2.VideoCapture(vid_path)

#Process the video while recording the operation execution times
print('performing inference...')
first_frame = True
while(1):
        if curFrame < NFRAMES:
                cam_start = time.time()

                #Get the next video frame
                ret, img = cap.read()
                if not ret:
                        break
                
                prep_start = time.time()

                #Preprocess the input frame
                img = cv2.resize(img, (200, 66))
                img = img / 255.

                pred_start = time.time()

                #Feed the frame to the model and get the control output
                rad = model.y.eval(feed_dict={model.x: [img]})[0][0]
                deg = rad2deg(rad)

                pred_end   = time.time()

                #Calculate the timings for each step
                cam_time  = (prep_start - cam_start)*1000
                prep_time = (pred_start - prep_start)*1000
                pred_time = (pred_end - pred_start)*1000
                tot_time  = (pred_end - cam_start)*1000

                #Write the angle to the serial 1 port
                ser1.write(struct.pack("lf", deg, deg))
                print(deg)

                text = 'Angle: {}'.format(float(deg))

                cv2.putText(img, text, (5,50), font, 1.0, (255,255,255))
                cv2.imshow('frame', img)
                cv2.waitKey(0)

                #Don't include the timings for the first frame due to cache warmup
                if first_frame:
                        first_frame = False
                else:
                        tot_time_list.append(tot_time)
                        curFrame += 1

                #Wait for next period
                wait_time = (period - tot_time) / 1000
                if is_periodic and wait_time > 0:
                        time.sleep(wait_time)
        else:
                break

cap.release()

ser1.close()
ser2.close()

print("Finished")
```


## Conclusion

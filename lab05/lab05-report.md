# Lab 05 Report
<p>Name: <b>Jace Kline</b></p>
<p>KUID: 2881618</p>
<p>Lab Section: Monday, 2:00-3:50 PM</p>

## Title: Servo Motor Control
## Procedure
1. Download Source archive and extract the tar file a folder
2. Open VSCode
3. Open the extracted folder as a PlatformioIDE project
4. Plug in the HiFive1 Microcontroller via the USB port to the development PC
5. Follow the lab instructions to connect the servo to the HiFive board
6. Read the source code for the library and main execution code
7. Read the lab specifications and instructions to understand the concept of PWM
   * PWM allows us to mimic an analog signal with a pulse of digital signals
   * The proportion between the time spent in "high" signal and "low" signal forms an average voltage that represents an intermediary value
7. Determine the mapping to convert the desired rotation angle (in degrees) to a pulse signal duration
   * The relationship is linear
   * 0 degrees corresponds to 544 microseconds (SERVO_PULSE_MIN) of an ON signal
   * 180 degrees corresponds to 2400 microseconds (SERVO_PULSE_MAX) of an ON signal
   * Therefore, `int on_delay_per_cycle = ((((SERVO_PULSE_MAX - SERVO_PULSE_MIN) / 180) * pos) + SERVO_PULSE_MIN);` by using the definition of a linear relationship
8. Determine the time that the signal should be "off" per cycle
   * This must be the difference between the total cycle time duration (in microseconds) minus the time that the signal will be ON. The global variable 'SERVO_PERIOD' represents the total cycle time.
   * Hence, `int off_delay_per_cycle = SERVO_PERIOD - on_delay_per_cycle;`
9. Use our calculated values to complete the implementation of the 'servo' function
   * For the ON period, we must "write" and ON signal to the GPIO memory address shared with the servo, then delay for the calculated ON time duration (see above).
   * For the OFF period, we must "write" an OFF signal to the GPIO memory address shared with the servo, then delay for the calculated OFF time duration (see above).

```c
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
```
8. Build and Upload the instructions to the board
9. Observe the behavior of the servo. It should rotate in 30 degree increments, then return back to start
9. Open the serial terminal inside Platformio and test the stream of 'printf' outputs
   * Each printf output will print the current angle measure (in degrees) that the servo is turned to

## Conclusion

<p>In this lab, I learned about the following main concepts:</p>

1. Digital-to-Analog conversion
2. Actuator devices
3. Pulse-Width Modulation (PWM)
4. Servos

Following from lab 03 when we learned about sensors and how they map "real world" analog data in to digital (binary) data, lab 04's purpose was to look at how we can represent digital (binary) data in a non-binary, analog format that can encode a large range values besides on or off. A common use case for needing to encode digital data in an analog format is when we want to send a large spectrum of possible values to an output device (actuator) that will perform a real-world action based on the values computed by the computer. Since actuators often act in a non-binary way, we have to transmit high/low voltage values in a way that "looks like" intermediary values on a scale when the voltage signals are averaged over a short time period. Ultimately, the process of digital-to-analog conversion involves a mapping from an average voltage value over a specified time period into a scalar quantity that represents the "intensity" of whatever output the output device is performing.

Actutator devices are the opposite of sensor devices. Actuators take computer data (binary) and convert values in to a real-world, analog context where some action is performed.

Pulse-width modulation (PWM) is a method by which to achieve digital-to-analog conversion by rapidly changing the ON/OFF signals at a specific time ratio over a specific PWM period. Each instance of an ON signal represents a "pulse". The ratio of the pulse time over the length of the single duty cycle corresponds to an average voltage intensity proportional to the length of the pulse signal in the duty cycle. By repeating these duty cycles over an even longer period of time - the PWM cycle period - we can simulate a fairly smooth "average" voltage signal. This average voltage signal is then used by an actuator device to produce an output based on the intensity of the average signal over the actuator device's specified PWM period.

A servo is a type of actuator device with a contol board, potentiometer, and a motor. Simply put, the servo continuously takes voltage readings via the potentiometer (measures electric potential ~ voltage) and uses its control board to convert those readings into motor output. For a servo, the motor output results in rotation at varying degrees. In this particular lab, our program (compiled to the HiFive board) was what was providing the voltage signals via GPIO to the servo. These voltage signals were then mapped into varying degrees of rotation based on the voltage signal.

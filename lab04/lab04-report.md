# Lab 04 Report
<p>Name: <b>Jace Kline</b></p>
<p>KUID: 2881618</p>
<p>Lab Section: Monday, 2:00-3:50 PM</p>

## Title: LIDAR Sensor
## Procedure
1. Download Source archive and extract the tar file a folder
2. Open VSCode
3. Open the extracted folder as a PlatformioIDE project
4. Plug in the HiFive1 Microcontroller via the USB port to the development PC
5. Follow the lab instructions to connect the LIDAR sensor to the HiFive board
6. Read the layout of the data packet that the LIDAR sends to encode its sensory data
   * The first two data bytes are metadata that should correspond to ASCII 'Y' to encode the signal that the following will be the sensory data bits
   * The third data bit represents the lower half (least significant bits) of the data digits
   * The fourth data bit represents the upper half (most significant bits) of the data digits
   * Ultimately, we must construct a 16 bit (2 byte) integer from two separate 8 bit integers
7. Implement the functionality of the data-reading conditional code block
   * The first conditional code block checks for the two consecutive encoding bytes 'Y' and 'Y' coming from ser_read(), which indicates the following two bytes will be information
   * We then read in the lower_data and upper_data in order from the stream via ser_read()
   * To combine these two 8-bit integers in to a 16-bit integer, we must allocate a 16-bit integer (called 'data')
   * 'data' will store the integer constructed by shifting the most significant bits (upper_data) left by 8 bits, then adding the bits of the least significant bits via either the logical OR operator or the addition operator
   * To provide feedback about the data values, we insert a print statement to print each data value as it is received
```c
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
```
8. Implement the logic to light the red and green LEDs depending on the distance data read by the LIDAR sensor
   * If the distance is less than 50cm, we light the red LED
   * Otherwise, we light the green LED
```c
if (dist < 50) {
    gpio_write(GREEN_LED, OFF);
    gpio_write(RED_LED, ON);
} else {
    gpio_write(RED_LED, OFF);
    gpio_write(GREEN_LED, ON);
}
```
8. Build and Upload the instructions to the board
9. Open the serial terminal inside Platformio and test the stream of 'printf' outputs
   * If the lowest bound of the sensor data is 30cm and the sensor's behavior seems to be acting correctly, then you can infer that the board is reading the sensor data as it should be
   * Furthermore, if the red LED triggers within 50cm on the sensor, and the green LED is on otherwise, then we know the conditional LED logic was implemented correctly

## Conclusion

<p>In this lab, I learned about the following main concepts:</p>

1. Sensors and sensory data
2. Metadata

Simply put, a sensor is a device that takes information about the "real" world and encodes the data in a cyber-readable format that computers can use to act on that data. The sensor will read data in via a fluid, wave-like analog signal that converts the data to a stream of discrete high and low voltages which are read by computers as bits. Each sensor will have a format by which the data is encoded when data is sent to a computer. It is important to read and understand the format of the data packet that the sensor will send. This is how a programmer can set up a method by which to extract useful and usable data. This data can then be used in the program to propogate the information in to an output via an actuator.

Metadata ties in to the idea of sensors and data in general as it is a way to encode data about the data. In this particular lab, it was useful to understand the use of the two 'Y' ASCII characters at the front of the data packet that acted as a signal to initiate the reading of the following data bits. Metadata is important in nearly all computational contexts from low-level to high-level programming.


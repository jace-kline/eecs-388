# Lab 10 Report
<p>Name: <b>Jace Kline</b></p>
<p>KUID: 2881618</p>
<p>Lab Section: Monday, 2:00-3:50 PM</p>

## Title: Networking Between Two Boards

## Procedure
1. Ensure all wire connections between PC and HiFive, as well as between the Pi 4 and the HiFive, are connected correctly
2. On the main PC, download the source code tar.gz archive and unzip into a directory
3. Open VSCode and edit the 'comm.c' file in the 'src' directory of the project
   * Within the main loop, must check whether new data has been received over UART
   * When this condition is met, read the data from the input serial line (UART1) and then write the same data to the other serial line (UART0) as output
```c
while (1) {
        if(ser_isready(1)) {
            char data = ser_read(1);
            ser_write(0, data); 
        }
    }
```
4. Upload the code to the board
5. Switch to the Raspberry PI 4
6. Start two 'screen' processes (terminal windows) as follows
   * The first command corresponds to connection to the output serial port /dev/ttyAMA1
   * The second command corresponds to connection to the input serial port /dev/ttyAMA2
   * The 115200 argument is the baud rate for the serial connections
```bash
$ screen /dev/ttyAMA1 115200
$ screen /dev/ttyAMA2 115200
```
7. Place cursor in the first screen and type some characters. Observe that these characters should be outputted in the second screen (terminal window)
8. Modify the code in 'ddn.py' from lab09 to repeatedly serial write the steering modification angle to serial port 1 after every deep neural network inference
9. At the top of the dnn.py file, import the 'serial' and 'struct' libraries
```python
import serial
import struct
```
10. Create the two Serial objects (from serial library) to represent the input and output serial connections
```python
ser1 = serial.Serial(port='/dev/ttyAMA1', baudrate=115200)
ser2 = serial.Serial(port='/dev/ttyAMA2', baudrate=115200)
```
11. Within the main loop, write the floating point number stored in 'deg' as a 1-byte data type to the serial 1 port. Additionally, print the degree to ensure correct data when running the program.
```python
ser1.write(struct.pack("lf", deg, deg))
print(deg)
```
12. At the end of the file, close the serial connections
```python
ser1.close()
ser2.close()
```
13. Run the program with the command `python dnn.py`


## Conclusion

In this lab, I learned how to connect the two microcontrollers, HiFive1 and Pi 4, together over serial ports and have them communicate at a shared baud rate. On the Pi 4 board, the pins 27 and 29 correspond to the input and output serial connection ports to the HiFive1 board, respectively. On the HiFive1 Board, UART1 receives data bytes from the Pi 4 board and it writes data over UART0. In this "network", we have two serial connections that allow for two-way communication between the boards.

Since we got exposure to it in this lab, I also want to discuss the 'screen' command in Linux. This command is used to spawn a single "session" with multiple "windows", where each window can correspond to a shell or a program running. Programs can continue to run when not visible or detached. Also, the programs from the different screen windows are completely independent of eachother. You can split, detach, reattach, close, and spawn new windows all within the single screen session. In this lab, we used the 'screen' command to open and observe two windows, with each observing a port (file) at /dev/ttyAMA1 and /dev/ttyAMA2.

In conclusion, I learned more about the serial interfaces provided by both the HiFive1 and the Pi 4 boards, gained some basic knowledge of how serial ports work in Linux, and I learned about the very powerful 'screen' command.

# Lab 07 Report
<p>Name: <b>Jace Kline</b></p>
<p>KUID: 2881618</p>
<p>Lab Section: Monday, 2:00-3:50 PM</p>

## Title: Introduction to Raspberry Pi 4

## Procedure
1. Plug in the Raspberry Pi to the power and the HDMI 
2. Switch the display to the Raspberry Pi display on the monitor
3. Log in to the Raspberry Pi
4. Change the password of the lab2 user (corresponds to my lab) using the `$passwd` command
5. Create a Hello World program in C
6. Compile and run the C program with `$gcc hello.c -o HelloWorldInC`, then `./HelloWorldInC`
7. Start a Python 2 REPL session with the command `$python` and input some test expressions
8. Open a new terminal window and run the `$htop` command. This displays real-time data about the CPU cores and memory usage. In addition, it displays information about current programs running, their users, and other information.
9. To test the htop program, create a program in C that ran an infinite loop. After compiling and running this program, observe (via htop) that one of the CPU cores runs at 100% until terminating the infinite loop program.
10. Run the `$pinout` command, unique to Raspberry Pi, that displays the mapping of all interface ports available on the board and their usage.

## Conclusion

In this lab, I got familiar with the Raspberry Pi 4 Board, namely its operating system and useful tools that come in its specialized Linux flavor. I learned that the Raspberry Pi, although a microcontroller, can be used for many common PC tasks, including development. In addition, the Raspberry Pi can be used in conjunction with other microcontrollers (connected) as part of a larger Cyber-Physical system.

Some useful programs that come as part of the Raspberry Pi's Linux Distribution include htop and pinout. These tools will become very important when we start to dive deeper in to embedded systems development on the Raspberry Pi.

Seperate from the Raspberry Pi, I also learned a little bit about some common Linux programs including 'gcc', 'python', 'emacs', and 'vi'. After testing out both text editors Emacs and Vi, I personally prefer Emacs. With regards to GCC, I learned the syntax for compiling a C file (.c) into an executable file. The 'python' program can be used, without arguments, to start a real-time interpreter. If run with the argument of a .py file, the python command will interpret and execute the python commands in the file.

In conclusion, this lab helped me to become familiar with Raspberry Pi, its Linux Distribution, and some useful programs that will help in future labs.

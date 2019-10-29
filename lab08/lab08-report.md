# Lab 08 Report
<p>Name: <b>Jace Kline</b></p>
<p>KUID: 2881618</p>
<p>Lab Section: Monday, 2:00-3:50 PM</p>

## Title: Playing with the Linux Scheduler

## Procedure
1. Plug in the Raspberry Pi to the power and the HDMI 
2. Switch the display to the Raspberry Pi display on the monitor
3. Log in to the Raspberry Pi
4. Navigate to the location of the 'cpuhog' program compiled in the last lab
5. Run three instances of the cpuhog program on CPU #0 using the following command 3 times: `$ taskset -c 0 ./cpuhog`
6. Open a new terminal window and run the 'htop' program to view the status of the multiple CPU cores and processes
7. Notice that the 3 instances of the 'cpuhog' program have NICE priority level of 0, which is the default
8. One at a time, change the NICE priority of the PID of each 'cpuhog' instance using the following command: `$ renice 5 <PID>`
9. After each time of the previous step, notice how the percentage of the CPU utilization will change for the instances of the 'cpuhog' program. After all 3 have had their priority updated, the percentage of each 'cpuhog' process will be roughly %33.3 of CPU core 0's utilization
10. Modify the 'cpuhog' source code (cpuhog.c) to include the responsibilities of:
    * forcing itself to be scheduled on CPU Core #0 using the **sched_setaffinity** function
    * Changing its NICE priority value to 5 using the **setpriority** function
```c
#define _GNU_SOURCE

#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>

int main() {
  cpu_set_t s;
  CPU_ZERO(&s);
  CPU_SET(0, &s);
  sched_setaffinity(0, sizeof(cpu_set_t), &s);

  setpriority(PRIO_PROCESS, 0, 5);

  while(1);
}
```
11. Compile the new source code `$ gcc cpuhog.c -o cpuhog` and run an instance of the program `$ ./cpuhog`
12. Open another 'htop' program window and ensure that the 'cpuhog' process has a NICE priority value of 5 and that it is running on the CPU Core #0


## Conclusion

In this lab, I got acclimated with the Linux scheduler on the Raspberry PI. In particular, we looked at how to specify a CPU Core for a program to be executed on. Additionally, we learned how to change the NICE priorty value for a program. In both of these activities, we learned how to perform these actions within BASH (external to the program) and also as system calls within our C program itself. I will now discuss the concepts of the NICE priority value and system calls (from C programs).

The NICE priority value of a process, group, or user in Linux represents the relative priority of a task or set of tasks in relation to others. The range of NICE values that are possible in Linux range from -20 (most important) to +19 (least important). More important tasks will always take precedence over lower priority tasks when they have instructions to execute. The lower priority tasks will get whatever "leftover" time/resources that are not used by higher priority tasks. In this particular lab, we changed the NICE value of the cpuhog program from 0 (default) to +5 (lower priority). To change the NICE priority value, we can use the BASH command `$ renice <NICE_VAL> <PID>` or we can use the series of instructions (system calls) in C:
```c
  cpu_set_t s;
  CPU_ZERO(&s);
  CPU_SET(0, &s);
  sched_setaffinity(0, sizeof(cpu_set_t), &s);
```

System calls are a concept that involves performing system actions (operations) on behalf of the program. During the execution of a system call, implemented as an interrupt, the execution mode is changed from 'user' mode to 'kernel' mode while the OS essentially goes through an interrupt service routine, an "operating system trap" to carry out the desired system instructions. The primary uses for system calls are: (1) Manage the file system, (2) Control Processes, (3) Communication between different processes during execution. There are C programming language libraries that offer "wrapper" functions allowing a C programmer to invoke system calls on the calling of any C wrapper function without having to manually tinker with the low-level detail involved.

In conclusion, this lab taught me how to: (1) choose a specific CPU Core to execute a program on, (2) change the NICE priority value of a program, and (3) use system calls as part of a C program.

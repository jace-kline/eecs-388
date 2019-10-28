#define _GNU_SOURCE

#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>

// Complile with 'gcc cpuhog.c -o cpuhog'

int main() {
  cpu_set_t s;
  CPU_ZERO(&s);
  CPU_SET(0, &s);
  sched_setaffinity(0, sizeof(cpu_set_t), &s);

  setpriority(PRIO_PROCESS, 0, 5);

  while(1);
}

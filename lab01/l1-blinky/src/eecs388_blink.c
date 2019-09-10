#include <stdint.h>

#include "eecs388_lib.h"

int main()
{
    int gpio = GREEN_LED;

    gpio_mode(gpio, OUTPUT);
    
    while(1)
    {
        gpio_write(gpio, ON);
        delay(1000);
        gpio_write(gpio, OFF);
        delay(300);
    }
}
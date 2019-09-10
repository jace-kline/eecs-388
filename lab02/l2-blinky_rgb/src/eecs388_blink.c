#include <stdint.h>

#include "eecs388_lib.h"

/*
  Task 2.1: Review eecs388_lib.h and eecs388_lib.c 

  Task 2.2: Implement RGB blinky. 

    - set red, green, and blue leds as output mode, using gpio_mode() function
    - in the main loop, turn on/off a different LED at each iteration. 
      for example, 
         at 1st iteration
            turn on RED led
	    delay 500 ms 
	    turn off RED led
	    delay 300 ms.
	 at 2nd iteration
	    do the same but for GREEN led
	 at 3rd iteration
	    do the same but for BLUE led
	 at 4th iteration
	    back to RED led. 
	    ...
    - use array and modulo (%) operator to change the color. 

  Task 2.3. Implement RGBA (bonus: +1 point)
    - rotate red, green, blue, and white (all LEDs are enabled) colors.
*/

int updateGPIO(int lastGPIO) {
   switch (lastGPIO) {
      case RED_LED: return(GREEN_LED);
      case GREEN_LED: return(BLUE_LED);
      default: return(RED_LED);
   }
   return(GREEN_LED);
}

int main()
{
    int gpio = GREEN_LED;

    gpio_mode(gpio, OUTPUT);
    while(1) {
      for(int i = 0; i < 3; i++)
      {
         gpio_write(gpio, ON);
         delay(500);
         gpio_write(gpio, OFF);
         delay(300);

         gpio = updateGPIO(gpio);
         gpio_mode(gpio, OUTPUT);
         
      }

      gpio_write(GREEN_LED, ON);
      gpio_write(BLUE_LED, ON);
      gpio_write(RED_LED, ON);
      delay(500);
      gpio_write(GREEN_LED, OFF);
      gpio_write(BLUE_LED, OFF);
      gpio_write(RED_LED, OFF);
      delay(300);
    }

}

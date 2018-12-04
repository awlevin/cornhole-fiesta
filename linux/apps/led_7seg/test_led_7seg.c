/*
 * stepper.c
 * Signal recving program in the user space
 * Originated from http://people.ee.ethz.ch/~arkeller/linux/code/signal_user.c
 * Modified by daveti
 * Aug 23, 2014
 * root@davejingtian.org
 * http://davejingtian.org
 */
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

#include "../include/ece453.h"

//*****************************************************************************
//*****************************************************************************
int main(int argc, char **argv)
{
  int x = 0, y = 3;
	int led1, led2, led3;
	while(x < 30) {
		if ( (x % 2) == 1) {
			led1 = 6;
			led2 = 9;
			led3 = 6;
		} else {
			led1 = 4;
			led2 = 2;
			led3 = 0;
		}

		led_7seg_write(0, led1);
		led_7seg_write(1, led2);
		led_7seg_write(2, led3);

		x++;
		sleep(1);
	}

  return 0;
}


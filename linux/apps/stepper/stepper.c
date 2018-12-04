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
  char response[80];
  int steps;

  // ADD CODE
  // Disable the Stepper Motor
	ece453_reg_write(CONTROL_REG, 0x5);

  // ADD CODE
  // If there are any outstanding interrupts, clear them.
	ece453_reg_write(IRQ_REG, 0x0);
	ece453_reg_write(IM_REG, 0x0);

  // ADD CODE
  // Set up the speed at which the motor spins
  ece453_reg_write(STEP_PERIOD_REG, 0x3d090);

  // Wait for the user to indicate how many steps to move
  printf("Enter the number of steps: ");
  fgets(response, 79,stdin);
  steps = strtol(response,NULL,10);

  // ADD CODE
  // Set the step count reg to the value in steps
	ece453_reg_write(STEP_COUNT_REG, steps);
	ece453_reg_write(CONTROL_REG, 0x1b);

  // ADD CODE
  // Configure the control register so that the stepper motor stops once the step count reaches 0
  while(ece453_reg_read(STEP_COUNT_REG) != 0);
	ece453_reg_write(CONTROL_REG, 0x5);
    
  return 0;
}


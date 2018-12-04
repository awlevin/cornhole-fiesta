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

#define SIG_TEST 44 /* we define our own signal, hard coded since SIGRTMIN is different in user and in kernel space */ 

bool done = true;
bool complete = true;

//*****************************************************************************
//*****************************************************************************
int set_pid(void)
{

	char buf[10];
	int fd = open(PID_REG, O_WRONLY);
	if(fd < 0) {
		perror("open");
		return -1;
	}
	sprintf(buf, "%i", getpid());
	if (write(fd, buf, strlen(buf) + 1) < 0) {
		perror("fwrite"); 
		return -1;
	}
  close(fd);
  return 0;
}

//*****************************************************************************
//*****************************************************************************
int clear_pid(void)
{

	char buf[10];
	int fd = open(PID_REG, O_WRONLY);
	if(fd < 0) {
		perror("open");
		return -1;
	}
	
 memset(buf,0,10);
 if (write(fd, buf, strlen(buf) + 1) < 0) {
		perror("fwrite"); 
		return -1;
	}
  close(fd);
  return 0;
}


//*****************************************************************************
//*****************************************************************************
void receiveData(int n, siginfo_t *info, void *unused)
{
   printf("Stepping Complete\n\r");
   complete = true;
}

//*****************************************************************************
//*****************************************************************************
void control_c_handler(int n, siginfo_t *info, void *unused)
{
  ece453_reg_write(IM_REG, 0);
  clear_pid();
  done = false;
}


//*****************************************************************************
//*****************************************************************************
int main(int argc, char **argv)
{
	struct sigaction stepper_sig;
	struct sigaction ctrl_c_sig;
  char response[80];
  int steps;

  // Set up handler for information set from the kernel driver
	stepper_sig.sa_sigaction = receiveData;
	stepper_sig.sa_flags = SA_SIGINFO;
	sigaction(SIG_TEST, &stepper_sig, NULL);

  // Set up handler for when the user presses CNTL-C to stop the application
	ctrl_c_sig.sa_sigaction = control_c_handler;
	ctrl_c_sig.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &ctrl_c_sig, NULL);

  // Configure the IP module 
  set_pid();

  // ADD CODE
  // Disable the Stepper Motor
	ece453_reg_write(CONTROL_REG, 0x5);

  // ADD CODE
  // If there are any outstanding interrupts, clear them.
	ece453_reg_write(IRQ_REG, 0xFF);

  // ADD CODE
  // enable reception of an interrupt when the stepper motor stops. 
	ece453_reg_write(IM_REG, 0x1);

  // ADD CODE
  // Set up the speed at which the motor spins
	ece453_reg_write(STEP_PERIOD_REG, 0x3d090);
  
  /* Loop forever, waiting for interrupts */
  while (done) {
    if(complete)
    {
      printf("Enter the number of steps: ");
      fgets(response, 79,stdin);
      steps = strtol(response,NULL,10);
      printf("Steps : 0x%08x\n\r",steps);

      // ADD CODE
      // Set the step count reg to the value in steps
			ece453_reg_write(STEP_COUNT_REG, steps);

      // ADD CODE
      // Configure the control register so that the stepper motor stops once the step count reaches
      // 0 
			ece453_reg_write(CONTROL_REG, 0x1b);
      
      complete = false;
    }
    sleep(1000000);
  }
  
  return 0;
}


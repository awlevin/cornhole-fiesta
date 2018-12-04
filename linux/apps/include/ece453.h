/*
 */

#ifndef __ECE453_APP_H__
#define __ECE453_APP_H__

#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>

#define DEVICE_ID_REG	"/sys/kernel/ece453/device_id"
#define GPIO_IN_REG	"/sys/kernel/ece453/gpio_in"
#define GPIO_OUT_REG	"/sys/kernel/ece453/gpio_out"
#define CONTROL_REG	"/sys/kernel/ece453/control"
#define STEP_COUNT_REG	"/sys/kernel/ece453/step_count"
#define STEP_PERIOD_REG	"/sys/kernel/ece453/step_period"
#define IM_REG		"/sys/kernel/ece453/interrupt_mask"
#define IRQ_REG		"/sys/kernel/ece453/irq"
#define PID_REG		"/sys/kernel/ece453/pid"

//*******************************************************************
// Register Addresses
//*******************************************************************
#define ECE453_DEV_ID_OFFSET        0 
#define ECE453_GPIO_IN_OFFSET       1 
#define ECE453_GPIO_OUT_OFFSET      2 
#define ECE453_CONTROL_OFFSET       3 
#define ECE453_STEP_COUNT_OFFSET    4 
#define ECE453_STEP_PERIOD_OFFSET   5 
#define ECE453_IM_OFFSET            6 
#define ECE453_IRQ_OFFSET           7 

//*******************************************************************
// Register Bit Definitions
//*******************************************************************
#define CONTROL_RESET_N_BIT_NUM	 0
#define CONTROL_DIR_BIT_NUM		   1
#define CONTROL_EN_N_BIT_NUM		 2
#define CONTROL_SLEEP_N_BIT_NUM	 3
#define CONTROL_MODE_BIT_NUM	   4

//*******************************************************************
// Register Masks
//*******************************************************************
#define CONTROL_RESET_N_MASK		 (1 << CONTROL_RESET_N_BIT_NUM)
#define CONTROL_DIR_MASK		     (1 << CONTROL_DIR_BIT_NUM)
#define CONTROL_EN_N_MASK		     (1 << CONTROL_EN_N_BIT_NUM)
#define CONTROL_SLEEP_N_MASK		 (1 << CONTROL_SLEEP_N_BIT_NUM)
#define CONTROL_MODE_MASK		     (1 << CONTROL_MODE_BIT_NUM)


int ece453_reg_read(char *reg_name);

int ece453_reg_write(char *reg_name, int value);

uint8_t convert_to_7seg_hex(int value);


#endif

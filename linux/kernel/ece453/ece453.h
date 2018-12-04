/*
 * Linux 2.6 and 3.0 'parrot' sample device driver
 *
 * Copyright (c) 2011, Pete Batard <pete@akeo.ie>
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#define DRIVER_NAME "ece453_driver"

#define DEVICE_NAME "socfpga"
#define CLASS_NAME "ece453"


#define AUTHOR "ECE453 Staff"
#define DESCRIPTION "Example sysfs driver ECE453 DE1-SoC Interface Mezzanine Card"
#define VERSION "0.2"

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

#define LED_7SEG0_SER_OFF						3
#define LED_7SEG1_SER_OFF						4
#define LED_7SEG2_SER_OFF						5
#define LED_7SEG3_SER_OFF						6
#define LED_7SEG_SEND_DATA_OFF			7

/* We'll use our own macros for printk */
#define dbg(format, arg...) do { if (debug) pr_info(CLASS_NAME ": %s: " format , __FUNCTION__ , ## arg); } while (0)
#define err(format, arg...) pr_err(CLASS_NAME ": " format, ## arg)
#define info(format, arg...) pr_info(CLASS_NAME ": " format, ## arg)
#define warn(format, arg...) pr_warn(CLASS_NAME ": " format, ## arg)

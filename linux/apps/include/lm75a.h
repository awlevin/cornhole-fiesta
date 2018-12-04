#ifndef __LM75A_H__
#define __LM75A_H__
// Copyright (c) 2018, Joe Krachey
// All rights reserved.
//
// Redistribution and use in binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in 
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <linux/i2c-dev.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/types.h>
#include "ece453.h"

#define I2C_DEV "/dev/i2c-2"

// ADD CODE
// Fill in the correct value for the LM75A Slave address
// on the ECE453 Interface Card. When done, delete the #error 
// directive 
#define LM75A_SLAVE_ADDR              0x4F
#define LM75A_REG_SELECT_TEMP					0x00
#define LM75A_REG_SELECT_CONFIG				0x01
#define LM75A_REG_SELECT_TEMP_HYST		0x02
#define LM75A_REG_SELECT_TOS					0x03
#define LM75A_REG_SELECT_PRODUCT_ID		0x07

//*****************************************************************************
//*****************************************************************************
void lm75a_open(void);

//*****************************************************************************
//*****************************************************************************
void lm75a_close(void);

//*****************************************************************************
//*****************************************************************************
void lm75a_write(uint8_t reg, uint8_t value);

//*****************************************************************************
// Reads the Temp Register
//*****************************************************************************
int16_t lm75a_read_temp(void);

//*****************************************************************************
// Reads the Product ID
//*****************************************************************************
uint8_t lm75a_read_product_id(void);

#endif


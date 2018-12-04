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
#include "../include/accelerometer.h"

static int         spi_fd;
static uint8_t     spi_mode;
static uint8_t     spi_bits;
static uint32_t    spi_speed;
static uint16_t    spi_delay;


//*****************************************************************************
//*****************************************************************************
static void pabort(const char *s)
{
	perror(s);
	abort();
}


//*****************************************************************************
// Transfer SPI data 
//*****************************************************************************
static void spi_tx(uint8_t *tx, uint8_t size, uint8_t *rx)
{
	int ret;
  struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = size,
		.delay_usecs = spi_delay,
		.speed_hz = spi_speed,
		.bits_per_word = spi_bits,
	};

	ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");
}

//*****************************************************************************
//*****************************************************************************
static uint8_t accel_reg_read(uint8_t reg)
{
  uint8_t tx_data[2];
  uint8_t rx_data[2];
 
  // ADD CODE
	/* From page 39 of datasheet:
	 *	Bit0     = READ bit = 1
	 *  Bit1-7   = Address AD(6:0) - address field of indexed register
	 *  Bit8-15  = data DO(7:0) (read mode) - the data that will be read from the device (MSb
	 first).
	 *  Bit16-.. = data DO(...-8). Further data in multiple byte reads.
	*/

	// Send a read command (RRRRRRR1)
	tx_data[0] = reg | ACCEL_SPI_READ; // left shift by 1 and set bit0=1 for read

	spi_tx(tx_data, 2, rx_data);
   
  // return the value in the specified register
  return rx_data[1];
}

//*****************************************************************************
//*****************************************************************************
static void accel_reg_write(uint8_t reg, uint8_t data)
{
  uint8_t tx_data[2];
  uint8_t rx_data[2];
  
  // ADD CODE
  tx_data[0] = reg | ACCEL_SPI_WRITE_N; 
  spi_tx(tx_data, 2, rx_data); 
}

//*****************************************************************************
//*****************************************************************************
int16_t accel_read_x(void)
{

  // ADD CODE

  // Using accel_reg_read, read the 2 registers
  // that are used to determine the acceleration
  // in the x direction
  
  int8_t x_low = accel_reg_read(ACCEL_OUTX_L_XL);
  int8_t x_high = accel_reg_read(ACCEL_OUTX_H_XL);

  // Return the value	 
	return ((x_high << 8) | x_low);  
} 

//*****************************************************************************
//*****************************************************************************
int16_t accel_read_y(void)
{
  // ADD CODE

  // Using accel_reg_read, read the 2 registers
  // that are used to determine the acceleration
  // in the y direction
  
  int8_t y_low = accel_reg_read(ACCEL_OUTY_L_XL);
  int8_t y_high = accel_reg_read(ACCEL_OUTY_H_XL);

  // Return the value	 
	return ((y_high << 8) | y_low);

}

//*****************************************************************************
//*****************************************************************************
int16_t accel_read_z(void)
{
    // ADD CODE

  // Using accel_reg_read, read the 2 registers
  // that are used to determine the acceleration
  // in the z direction
  
  int8_t z_low = accel_reg_read(ACCEL_OUTZ_L_XL);
  int8_t z_high = accel_reg_read(ACCEL_OUTZ_H_XL);

  // Return the value	 
	return ((z_high << 8) | z_low);
}

//*****************************************************************************
//*****************************************************************************
uint8_t accel_read_device_id(void)
{
  return accel_reg_read(ACCEL_WHO_AM_I_R); 
}

//*****************************************************************************
// Provided initialization code for the LCD.  Found in the data sheet.
//*****************************************************************************
void config_accel(void)
{
	accel_reg_write(ACCEL_INT1_CTRL_R, 0x00);  // For now, disable interrupts
	      
	accel_reg_write(ACCEL_CTRL1_XL_R, ACCEL_CTRL1_XL_ODR_208HZ | ACCEL_CTRL1_XL_2G | ACCEL_CTRL1_XL_ANTI_ALIAS_50HZ);
	accel_reg_write(ACCEL_CTRL2_G_R, ACCEL_CTRL2_G_ODR_416HZ | ACCEL_CTRL2_G_FS_245_DPS | ACCEL_CTRL2_G_FS_125);
	accel_reg_write(ACCEL_CTRL5_C_R, ACCEL_CTRL5_SLEEP_G | ACCEL_CTRL5_INT2_ON_INT1);
}

//*****************************************************************************
// Initialize the LCD peripheral
//*****************************************************************************
void accel_open(
  char        *device, 
  uint8_t     mode,
  uint8_t     bits,
  uint32_t    speed,
  uint16_t    delay
)
{
  	int ret;
  	spi_delay = delay;
 
  	// Configure the SPI Interface	
	spi_fd = open(device, O_RDWR);
	if (spi_fd < 0)
		pabort("can't open device");
	
	/*
	 * spi mode
	 */
	ret = ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");
	
	ret = ioctl(spi_fd, SPI_IOC_RD_MODE, &spi_mode);
	if (ret == -1)
		pabort("can't get spi mode");
	
	/*
	 * bits per word
	 */
	ret = ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");
	
	ret = ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bits);
	if (ret == -1)
		pabort("can't get bits per word");
	
	/*
	 * max speed hz
	 */
	ret = ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");
	
	ret = ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
	if (ret == -1)
		pabort("can't get max speed hz");
	
	printf("spi mode: %d\n", spi_mode);
	printf("bits per word: %d\n", spi_bits);
	printf("max speed: %d Hz (%d KHz)\n", spi_speed, spi_speed/1000);
	
	// Initialize the registers of the accelerometer
	config_accel();	
}

//*****************************************************************************
// Turn Off the LCD 
//*****************************************************************************
void accel_close(void)
{
  close(spi_fd);
}


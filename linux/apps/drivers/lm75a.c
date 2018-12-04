#include "../include/lm75a.h"

// File handle for I2C /dev entry
static int i2c_fh;

//*****************************************************************************
//*****************************************************************************
static void lm75a_set_slave_address(void)
{
	if (ioctl(i2c_fh, I2C_SLAVE, LM75A_SLAVE_ADDR) < 0) {
		perror("Error setting slave address");
		exit(1);
	}

}


//*****************************************************************************
//*****************************************************************************
void lm75a_open(void)
{
	i2c_fh = open(I2C_DEV, O_RDWR);
	if (i2c_fh < 0) {
		perror("i2c_open failed");
		exit(1);
	}

  // Set the slave address for the lm75a
  lm75a_set_slave_address();
}

//*****************************************************************************
//*****************************************************************************
void lm75a_close(void)
{
  close(i2c_fh);
}

//*****************************************************************************
// Reads the Temp Register
//*****************************************************************************
int16_t lm75a_read_temp(void)
{
  uint8_t reg = LM75A_REG_SELECT_TEMP;
  int8_t return_value[2];
	int16_t ret;
  
  // ADD CODE

	if (write(i2c_fh, &reg, 1) != 1) {
	  perror("Failed to set address registerr");
	}

  if (read(i2c_fh, &return_value, 2) != 2) {
	  perror("1. Failed to read data");
	}

	ret = ((return_value[1] >> 7) | (return_value[0] << 1));

  return ret;
}

//*****************************************************************************
// Reads the Product ID
//*****************************************************************************
uint8_t lm75a_read_product_id(void)
{
  uint8_t reg = LM75A_REG_SELECT_PRODUCT_ID;
  uint8_t data;
	int16_t return_value;
	 
	if (write(i2c_fh, &reg, 1) != 1) {
	  perror("Failed to set address registerr");
		printf("reg: %x", reg);
	}
	if (read(i2c_fh, &data, 1) != 1) {
		perror("Failed to read data");
	}

	return data;  
}


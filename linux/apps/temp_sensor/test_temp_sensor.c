#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../include/lm75a.h"

int
main(int argc, char * argv[]) {

  uint8_t product_id;
  int16_t temp;

  // Open the Touch Controller
  lm75a_open();

  product_id = lm75a_read_product_id();
  printf("Product ID: 0x%02x\n\r",product_id);
  temp = lm75a_read_temp();
  printf("Temp: %d\n\r",temp/2);

  // Close the Touch Controler
  lm75a_close();

  return 0;
}

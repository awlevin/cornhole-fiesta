/*
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */

#include "../include/accelerometer.h"

//*********************************************************
//*********************************************************
static void print_usage(const char *prog)
{
	printf("Usage: %s [-DsbdlHOLC3]\n", prog);
	puts("  -D --device   device to use (default /dev/spidev1.1)\n"
	     "  -s --speed    max speed (Hz)\n"
	     "  -d --delay    delay (usec)\n"
	     "  -b --bpw      bits per word \n"
	     "  -l --loop     loopback\n"
	     "  -H --cpha     clock phase\n"
	     "  -O --cpol     clock polarity\n"
	     "  -L --lsb      least significant bit first\n"
	     "  -C --cs-high  chip select active high\n"
	     "  -3 --3wire    SI/SO signals shared\n");
	exit(1);
}

//*********************************************************
//*********************************************************
static void parse_opts(
  int argc, 
  char *argv[], 
  char *device, 
  uint8_t *mode, 
  uint8_t *bits, 
  uint32_t *speed, 
  uint16_t *delay 
)
{
	while (1) {
		static const struct option lopts[] = {
			{ "device",  1, 0, 'D' },
			{ "speed",   1, 0, 's' },
			{ "delay",   1, 0, 'd' },
			{ "bpw",     1, 0, 'b' },
			{ "loop",    0, 0, 'l' },
			{ "cpha",    0, 0, 'H' },
			{ "cpol",    0, 0, 'O' },
			{ "lsb",     0, 0, 'L' },
			{ "cs-high", 0, 0, 'C' },
			{ "3wire",   0, 0, '3' },
			{ "no-cs",   0, 0, 'N' },
			{ "ready",   0, 0, 'R' },
			{ NULL, 0, 0, 0 },
		};
		int c;

		c = getopt_long(argc, argv, "D:s:d:b:lHOLC3NR", lopts, NULL);

		if (c == -1)
			break;

		switch (c) {
		case 'D':
			device = optarg;
			break;
		case 's':
			*speed = atoi(optarg);
			break;
		case 'd':
			*delay = atoi(optarg);
			break;
		case 'b':
			*bits = atoi(optarg);
			break;
		case 'l':
			*mode |= SPI_LOOP;
			break;
		case 'H':
			*mode |= SPI_CPHA;
			break;
		case 'O':
			*mode |= SPI_CPOL;
			break;
		case 'L':
			*mode |= SPI_LSB_FIRST;
			break;
		case 'C':
			*mode |= SPI_CS_HIGH;
			break;
		case '3':
			*mode |= SPI_3WIRE;
			break;
		case 'N':
			*mode |= SPI_NO_CS;
			break;
		case 'R':
			*mode |= SPI_READY;
			break;
		default:
			print_usage(argv[0]);
			break;
		}
	}
}

//*********************************************************
//*********************************************************
int main(int argc, char *argv[])
{
	int ret = 0;
	uint8_t digit = 0x01;
	int fd;
  
  char *device = SPI_DEVICE;
  uint8_t mode=3;
  uint8_t bits = 8;
  uint32_t speed = 5000000;
  uint16_t delay = 0;

  // Parse any command line options
	parse_opts(
    argc, 
    argv,
    device,
    &mode,
    &bits,
    &speed,
    &delay
  );

  // Initalize the accelerometer
  accel_open(
    device, 
    mode,
    bits,
    speed,
    delay
  );

  // print out the device ID of the accelerometer
  printf("Accelerometer Device ID: 0x%02x\n\r",accel_read_device_id());
  
  accel_close();
  return ret;
}


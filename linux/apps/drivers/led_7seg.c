
#include "../include/led_7seg.h"

uint8_t convert_to_7seg_hex(int value) {
	
	switch (value) {
		case 0:
			return 0xDE;
		case 1:
			return 0x06;
		case 2:
			return 0xBA;
		case 3:
			return 0xAE;
		case 4:
			return 0x66;
		case 5:
			return 0xEC;
		case 6:
			return 0xFC;
		case 7:
			return 0x86;
		case 8:
			return 0xFE;
		case 9:
			return 0xE6;
		case -1: // LED_OFF
			return 0x00;
		default:
			perror("invalid value for 7seg");
			return -1;
	}
}

//*****************************************************************************
//*****************************************************************************
int led_7seg_write(int seg_num, int value)
{
	// open file descriptor
	char *path = strdup(LED_7SEG_REG);

	int fd = open(path, O_WRONLY); 
	if(fd < 0) {
		perror("open");
		return -1;
	}
	
	// convert number we're writing to hex
	uint8_t hex_val = convert_to_7seg_hex(value);

	char string_val[9];

	sprintf(string_val, "%x %x", seg_num, hex_val);

	if (write(fd, string_val,sizeof(string_val)) < 0) {
		return -1;
	}
}

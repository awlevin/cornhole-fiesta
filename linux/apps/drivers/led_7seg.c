
#include "../include/led_7seg.h"

int convert_val_to_7seg_hex(uint8_t value) {
	if (value > 9 || value < 0) 
		return -1;
	else
		return SEG_HEX_ARR[value]; 
}

int convert_7seg_hex_to_val(uint8_t hex) {

	for (int i = 0; i < 10; i++) { 
		printf("SEG_HEX_ARR[%d] = %02x,\t hex=%02x\n",i,SEG_HEX_ARR[i],hex);
		if (SEG_HEX_ARR[i] == hex) {
			return i;
		}
	}

	return -1;
}

//*****************************************************************************
//*****************************************************************************
int led_7seg_write(uint8_t seg_num, uint8_t value)
{
	// open file descriptor
	char *path = strdup(LED_7SEG_REG);
	uint8_t hex_val;

	int fd = open(path, O_WRONLY); 
	if (fd < 0) {
		perror("open 7seg_write");
		return -1;
	}
	
	// Turn off LED's or convert number we're writing to hex
	hex_val = (value == 23) ? 0x00 : convert_val_to_7seg_hex(value);

	char string_val[9];

	sprintf(string_val, "%x %02x", seg_num, hex_val);

	if (write(fd, string_val,sizeof(string_val)) < 0) {
		return -1;
	}

	return close(fd);
}

/*
// This function actually reads all of the 7segs but then returns only the specified one
int led_7seg_read(uint8_t seg_num) {
	char *path = strdup(LED_7SEG_REG);

	printf("7SEG_READ: %d\n", seg_num);

	int fd = open(path, O_RDONLY);
	if (fd < 0) {
		perror("open 7seg_read");
		return -1;
	}

	char buf[12];
	char *hex_char_ptr;
	int rdlen;
	uint32_t hex_val;
	
	rdlen = read(fd, buf, sizeof(buf) - 1);

	if (rdlen <= 0)
		return -1;

	printf("buf: %s\n", buf);
	const char s[2] = " ";
	hex_char_ptr = strtok(buf, s);

	for (size_t i = 0; i <= seg_num; i++) {
		if (hex_char_ptr != NULL && i == seg_num) { 
			hex_val = atoll(hex_char_ptr);
			printf("hex_val: %02x\n", hex_val);
			uint8_t value = convert_7seg_hex_to_val(hex_val);
			printf("value: %d", value);
			return convert_7seg_hex_to_val(hex_val);
		}
		hex_char_ptr = strtok(NULL, " ");
	}
	return -1;
}
*/

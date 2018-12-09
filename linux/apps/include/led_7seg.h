
#ifndef __LED_7SEG_H__
#define __LED_7SEG_H__

#include "ece453.h"

#define LED_7SEG_REG	"/sys/kernel/ece453/led_7seg"

const uint8_t SEG_HEX_ARR[10] = {0xDE,0x06,0xBA,0xAE,0x66,0xEC,0xFC,0x86,0xFE,0xE6};

int convert_val_to_7seg_hex(uint8_t value);
int convert_7seg_hex_to_val(uint8_t hex); 

int led_7seg_write(uint8_t seg_num, uint8_t value);
int led_7seg_read(uint8_t seg_num);

#endif

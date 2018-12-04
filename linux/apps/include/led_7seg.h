
#ifndef __LED_7SEG_H__
#define __LED_7SEG_H__

#include "ece453.h"

#define LED_7SEG_REG	"/sys/kernel/ece453/led_7seg"

uint8_t convert_to_7seg_hex(int value);
int led_7seg_write(int seg_num, int value);

#endif

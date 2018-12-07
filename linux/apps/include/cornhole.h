
#ifndef __CORNHOLE_H__
#define __CORNHOLE_H__

#include "cornhole_types.h"
#include "ece453.h"
#include "xbee.h"

int led_7seg_write(int seg_num, int value);
int led_7seg_read(int seg_num);
int open_xbee();
int close_xbee();

#endif

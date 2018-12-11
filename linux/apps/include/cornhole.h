
#ifndef __CORNHOLE_H__
#define __CORNHOLE_H__

#include "cornhole_types.h"
#include "ece453.h"
#include "xbee.h"
#include <stdio.h>
#include <stdlib.h>
int led_7seg_write(int seg_num, int value);
int led_7seg_read(int seg_num);
int process_hole();
int process_long();
int open_xbee();
int close_xbee();
int write_xbee(LED_COLOR color);
int led_7seg_write_team(int team_num, int score);
int init_game();
void *blink_func();
int edit_mode(CORN_OP op);
int normal_mode(CORN_OP op);
void *team_sw_func();
void update_segs();
#endif

/*
 * cornhole.c
 */
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>

#include "../include/cornhole.h"

#define DEBUG_ON

CORN_OP op;
volatile uint8_t score0, score1; 
volatile int curr_team = 0;
volatile CORN_MODE curr_mode = MODE_PLAY;
	
// Background thread for blinking a team's LED's when in edit mode
pthread_t blink_thread;

//*****************************************************************************
//*****************************************************************************

int main(int argc, char **argv)
{

	init_game();

	while (1) {
		op = read_xbee();

		printf("op: %d\n", op);
	
		if (curr_mode == MODE_EDIT) {
			edit_mode(op);
		} else if (curr_mode == MODE_PLAY) {
			normal_mode(op);	
		}
	}

	// Need to close xbee somewhere, program wont reach here but we'll leave it here anyways for now
	close_xbee();
	
  return 0;
}

// Handle operations in edit mode
int edit_mode(CORN_OP op) {
	
	switch(op) {
		case TEAM1_HOLE:
			curr_team=0;
			break;
		case	TEAM2_HOLE:
			curr_team=1;
			break;
		case	PEDAL_SINGLE:
			if(curr_team==0) score0++;
			else if(curr_team==1)	score1++;
			break;
		case PEDAL_DOUBLE:
			if(curr_team==0) score0--;
			else if(curr_team==1)	score1--;
			break;
		case	PEDAL_LONG:
			curr_mode^=1; // toggle mode
			break;
	}
}

// Handle operations in normal mode
int normal_mode(CORN_OP op) {
	switch(op) {
		case TEAM1_HOLE:
			printf("TEAM1\n");
			process_hole(op);
			break;
		case TEAM2_HOLE:
			printf("TEAM2\n");
			process_hole(op);
			break;
		case PEDAL_SINGLE:
			printf("SNGLE\n");
			break;
		case PEDAL_DOUBLE:
			printf("DOUBL\n");
			break;
		case PEDAL_LONG:
			printf("LONG\n");
			curr_mode ^= 1;
			break;
		default: 
			printf("%d", op);
			break;
	}
}

int init_game() {
	
	// Write zeroes to scoreboard
	led_7seg_write_team(0, 0);
	usleep(1000);
	led_7seg_write_team(1, 0);
		
	// Turn on thread to handle blinking during edit mode
	pthread_create( &blink_thread, NULL, blink_func, NULL);

	// Open XBee file descriptor
	open_xbee();
}


int process_hole(CORN_OP op) {
	if (op == TEAM1_HOLE) {
		score0++;
		int dig1 = score0/10;
		int dig2 = score0%10;
		score0 = (score0 == 22) ? 0  : score0;
		led_7seg_write(0, dig1);
		led_7seg_write(1, dig2);
#ifdef DEBUG_ON
		printf("score0: %d\n", score0);
#endif
	} else if (op == TEAM2_HOLE) {
		score1++;
		int dig1 = score1/10;
		int dig2 = score1%10;
		score1 = (score1 == 22) ? 0 : score1;
		led_7seg_write(2, dig1);
		led_7seg_write(3, dig2);
#ifdef DEBUG_ON
		printf("score1: %d\n", score1);
#endif
	}
}

void *blink_func() {	
	
	while(1) {

		if (curr_mode == MODE_PLAY) {
			continue;
		}
		
		int dig1_team0 = score0/10;
		int dig2_team0 = score0%10;
		int dig1_team1 = score1/10;
		int dig2_team1 = score1%10;
		int blink_rate = 500000;

		if (curr_team == 0) {
			led_7seg_write(0, 23);
			led_7seg_write(1, 23);
			usleep(blink_rate);
			led_7seg_write(0, dig1_team0);
			led_7seg_write(1, dig2_team0);
			usleep(blink_rate);
		} 
		else if (curr_team == 1) {
			led_7seg_write(2, 23);
			led_7seg_write(3, 23);
			usleep(blink_rate);
			led_7seg_write(2, dig1_team1);
			led_7seg_write(3, dig2_team1);
			usleep(blink_rate);
		}
	}
}

int led_7seg_write_team(int team_num, int score) {

	int dig1 = score/10;
	int dig2 = score%10;

	if (team_num == 0) {
		led_7seg_write(0, dig1);
		led_7seg_write(1, dig2);
	}
	else if (team_num == 1) {
		led_7seg_write(2, dig1);
		led_7seg_write(3, dig2);
	} else {
		return -1;
	}
}

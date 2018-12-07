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

#include "../include/cornhole.h"

#define DEBUG_ON

//*****************************************************************************
//*****************************************************************************

void blink_7seg(int seg) {
	
}

int main(int argc, char **argv)
{

	CORN_OP op;
	int score1, score2;
	while (1) {
		op = read_xbee();
		printf("op: %d\n", op);
		if (op == TEAM1_HOLE) {
			score1++;
			score1 = (score1 == 10) ? -1 : score1;
			led_7seg_write(0, score1);
#ifdef DEBUG_ON
			printf("score1: %d\n", score1);
#endif
		} else if (op == TEAM2_HOLE) {
			score2++;
			score2 = (score2 == 10) ? -1 : score2;
			led_7seg_write(2, score2);
#ifdef DEBUG_ON
			printf("score2: %d\n", score2);
#endif
		}
	}

  return 0;
}


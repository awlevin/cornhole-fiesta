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

int main(int argc, char **argv)
{
	CORN_OP op;
	uint8_t score1, score2; 

	open_xbee();

	while (1) {
		op = read_xbee();
		printf("op: %d\n", op);
		if (op == TEAM1_HOLE) {
			score1++;
			score1 = (score1 == 11) ? 0  : score1;
			led_7seg_write(0, score1);
#ifdef DEBUG_ON
			printf("score1: %d\n", score1);
#endif
		} else if (op == TEAM2_HOLE) {
			score2++;
			score2 = (score2 == 11) ? 0 : score2;
			led_7seg_write(2, score2);
#ifdef DEBUG_ON
			printf("score2: %d\n", score2);
#endif
		}
	}

	// need to close xbee somewhere, program wont reach here but we'll leave it here anyways for now
	close_xbee();

  return 0;
}


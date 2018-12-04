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

//*****************************************************************************
//*****************************************************************************

int main(int argc, char **argv)
{

	CORN_OP op;
	int score1, score2;
	while (1) {
		op = read_xbee();
		printf("op: %d\n", op);
		if (op == TEAM1_HOLE) {
			printf("score1: %d", score1);
			score1 = (score1 == 10) ? -1 : score1+1;
			led_7seg_write(0, score1);
		} else if (op == TEAM2_HOLE) {
			printf("score2: %d", score2);
			score2 = (score2 == 10) ? -1 : score2+1;
			led_7seg_write(2, score2);
		}
	}

  return 0;
}


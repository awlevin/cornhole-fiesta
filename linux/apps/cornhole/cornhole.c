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
#include <unistd.h>
#include <sys/wait.h>

#include "../include/cornhole.h"

#define DEBUG_ON

CORN_OP op;
volatile int curr_team = 0;
volatile uint8_t score0, score1, hole_score0, hole_score1, cv_score0, cv_score1;
volatile int  net_cv_score;
volatile int check_for_hole0=0;
volatile int check_for_hole1=0;
volatile int turn_count=0;
volatile int round_count=0;
volatile int signal0,signal1=0;
volatile int update_score=0;
volatile CORN_MODE curr_mode = MODE_PLAY;	
// Background thread for blinking a team's LED's when in edit mode
pthread_t blink_thread;
pthread_t team_sw_thread;
TEAM_STATE state, n_state = TEAM0;
//*****************************************************************************
//*****************************************************************************

int exec_python() {
	char *argv[2];
	int returnStatus;

	argv[0] = "/usr/bin/python3";
	argv[1] = "pyscript.py";
	argv[2] = NULL;

	pid_t child_pid, wpid;
	child_pid = fork();
	if (child_pid == 0) {
		// if child
		printf("child starting\n");
		execvp(argv[0],argv);
		printf("should never get here\n");
	} 

	// wait for child
	printf("parent waiting, child_pid: %d\n", child_pid);
	waitpid(child_pid, &returnStatus, 0);
	printf("parent done waiting... returnStatus: %d\n", returnStatus);

	if (returnStatus < 0) {
		printf("error in child\n");
		exit(1);
	}

	FILE *fp;	
	fp = fopen("cvData.txt","r");
	if(fp==NULL) {
		printf("file error");
		return -1;
	}
//	int team0_cv_score, team1_cv_score;
	fscanf(fp, "%d %d", &cv_score0, &cv_score1);
	printf("/////\n team0 CVscore: %d\nteam1 CVscore: %d",cv_score0,cv_score1); 
	net_cv_score= cv_score0- cv_score1;
	update_segs();
	return 0;
}


int main(int argc, char **argv)
{
	
	printf("initting game\n");
	init_game();

	while (1) {
	//	update_segs();
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
			if(curr_team==0) (score0>0) ? score0-- : score0;

			else if(curr_team==1)	(score1>0) ? score1-- : score1;
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
		case HOLE_BREAK :
			process_hole(op);
		default: 
			printf("%d", op);
			break;
	}
}

int init_game() {
cv_score0 = 0;
cv_score1 = 0;
	// Write zeroes to scoreboard
	led_7seg_write_team(0, 0);
	usleep(1000);
	led_7seg_write_team(1, 0);
		
	// Turn on thread to handle blinking during edit mode
	pthread_create( &blink_thread, NULL, blink_func, NULL);

	//Turn on thread to handle team switching
	pthread_create ( &team_sw_thread, NULL, team_sw_func,NULL);
	//create thread -> &team_sw_thread
	// Open XBee file descriptor
	open_xbee();
}


int process_hole(CORN_OP op) {
//	int dig1,dig2,dig3,dig4;
	
	if(op==TEAM1_HOLE) {
		signal0=1;	
	} 
	else if(op==TEAM2_HOLE) {
		signal1=1;
	}
	else if(op==HOLE_BREAK) {
		if(check_for_hole0==1){
			hole_score0 = hole_score0+3;
			printf("TEAM0 hole break\n");
			check_for_hole0=0;
	}
		
		else if(check_for_hole1==1) {
			hole_score1 = hole_score1+3;
			printf("TEAM1 hole break\n");
			check_for_hole1=0;
		}
	}	
	

}

void update_segs()
{
		int temp_score0,temp_score1=0;
		int round_net_score= net_cv_score + hole_score0 - hole_score1;
		if(round_net_score>0)
		{
			temp_score0=round_net_score;
			temp_score1 = 0;
		}
		else
		{
			temp_score1 = -1*round_net_score;
			temp_score0 = 0;
		
		}

		printf("net score: %d", round_net_score);
		int dig1_team0 = temp_score0/10;
		int dig2_team0 = temp_score0%10;
		int dig1_team1 = temp_score1/10;
		int dig2_team1 = temp_score1%10;
	
		led_7seg_write(0, dig1_team0);
		led_7seg_write(1, dig2_team0);
		led_7seg_write(2, dig1_team1);
		led_7seg_write(3, dig2_team1);
}
	
void *team_sw_func() {
	while(1)
	{
		if(curr_mode==MODE_EDIT) {
			continue;
		}
		n_state = state;
	check_for_hole0=0;
	check_for_hole1=0;
		switch(state)
		{
			case TEAM0:	
				if(signal0 == 1) // when team0 steps on the footplate (signal0==1)
				{
					printf("team0 chance to score\n");
					signal0=0;
					signal1=0;
					check_for_hole0=1;	
					sleep(3);									//wait 3 sec for hole break, than check score

					exec_python();
					printf("TEAM0 chance over, update score\n");
					curr_team=1;
					n_state = TEAM1;
				}
				break;

			case TEAM1:
				if(signal1==1)
				 {
					printf("team1 chance to score\n");
					signal1=0;
					signal0=0;
					check_for_hole1=1;
					sleep(3);							//wait 3 sec for hole break, than check score


					exec_python();
					printf("team1 chance over, update score\n");
					update_score=1;			
					curr_team=0;
					n_state = TEAM0;
				}
				break;
		}
			state = n_state;
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



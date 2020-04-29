#define _GNU_SOURCE  
#include <stdio.h>
#include <sched.h>
#include "schedular.h"

int FIFO(Proc proc_list[], int proc_num, int now_time){
	for (int i = 0; i < proc_num; ++i){
		if (proc_list[i].r_time <= now_time && proc_list[i].e_time > 0)
			return i;
	}
	return -1;
}
int RR_push(Proc proc_list[], int proc_num, int now_time, int cur_exec, int RR_queue_in[30],int RR_queue[30],int * RR_sp, int * RR_ep){
	for (int i = 0; i < proc_num; ++i){
		if (i==cur_exec || RR_queue_in[i]==1)//running or in queue
			continue;
		if (proc_list[i].r_time <= now_time && proc_list[i].e_time > 0){//add in queue
			RR_queue_in[i] ==1;
			RR_queue[(*RR_ep)%30] = i;
			*RR_ep += 1;
			*RR_ep %= 30;
		}
	}
	if (*RR_sp != *RR_ep)//has things
		return RR_queue[*RR_sp];
	else
		return -1;
}
void RR_pop(int cur_exec, int RR_queue_in[30],int RR_queue[30],int * RR_sp, int * RR_ep){
	RR_queue_in[cur_exec]=0;
	*RR_sp +=1;
	*RR_sp %= 30;
}
int SJF(Proc proc_list[], int proc_num, int now_time){
	int min_id=-1;
	int min_e_time=100000000;
	for (int i = 0; i < proc_num; ++i){
		if (proc_list[i].r_time <= now_time && proc_list[i].e_time > 0){//can run
			if (proc_list[i].e_time < min_e_time){
				min_e_time = proc_list[i].e_time;
				min_id = i;
			}
		}
	}
	return min_id;
}
int PSJF(Proc proc_list[], int proc_num, int now_time){
	int min_id=-1;
	int min_e_time=100000000;
	for (int i = 0; i < proc_num; ++i){
		if (proc_list[i].r_time <= now_time && proc_list[i].e_time > 0){//can run
			if (proc_list[i].e_time < min_e_time){
				min_e_time = proc_list[i].e_time;
				min_id = i;
			}
		}
	}
	return min_id;
}

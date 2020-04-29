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
void RR(){
	printf("RR");
}
void SJF(){
	printf("SJF");
}
void PSJF(){
	printf("PSJF");
}

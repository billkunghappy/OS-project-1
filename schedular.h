#ifndef _SCHEDULAR_C_
#define _SCHEDULAR_C_
#define _GNU_SOURCE  
#include <stdio.h>
#include <sched.h>
#include "proc.h"

int FIFO(Proc proc_list[], int proc_num, int now_time);
int RR_push(Proc proc_list[], int proc_num, int now_time, int cur_exec, int RR_queue_in[30],int RR_queue[30],int * RR_sp, int * RR_ep);
void RR_pop(int cur_exec, int RR_queue_in[30],int RR_queue[30],int * RR_sp, int * RR_ep);
int SJF(Proc proc_list[], int proc_num, int now_time);
int PSJF(Proc proc_list[], int proc_num, int now_time);

#endif
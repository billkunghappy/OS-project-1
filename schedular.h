#ifndef _SCHEDULAR_C_
#define _SCHEDULAR_C_
#define _GNU_SOURCE  
#include <stdio.h>
#include <sched.h>
#include "proc.h"

int FIFO(Proc proc_list[], int proc_num, int now_time);
void RR();
void SJF();
void PSJF();

#endif
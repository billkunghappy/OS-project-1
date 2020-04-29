#define _GNU_SOURCE  
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include "proc.h"
#include <sys/time.h>
#include <sys/syscall.h>
#define GET_TIME 333
#define PRINTK 334
void run_unit(){
	volatile unsigned long i; for(i=0;i<1000000UL;i++);
}
void set_cpu(int pid, int core_index)
{
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(core_index, &mask);
	//set sched mask
	sched_setaffinity(pid, sizeof(mask), &mask);
}

void set_unblock(int pid)
{
	struct sched_param param;
	param.sched_priority = 0;
	sched_setscheduler(pid, SCHED_OTHER, &param);
}

void set_block(int pid)
{
	struct sched_param param;
	param.sched_priority = 0;
	sched_setscheduler(pid, SCHED_IDLE, &param);
}

int run_proc(Proc proc_i){
	int pid = fork();
	if (pid == 0) {//child!
		
		while(1){
			if(sched_getcpu()==1)
				break;
		}
		long long int start_time, end_time;
		start_time = syscall(GET_TIME);
		for (int i = 0; i < proc_i.e_time; i++){
			run_unit();
   		}
		end_time = syscall(GET_TIME);
		syscall(PRINTK, getpid(), start_time, end_time);
		exit(0);
	}

	printf("%s %d\n",proc_i.proc_name, pid);
	fflush(stdout);
	return pid;
}

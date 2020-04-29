#define _GNU_SOURCE  
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include "proc.h"
#include <sys/time.h>
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

void write_log(Proc proc_i, char stat[]){
	char buff[100] ;
	struct timeval  tv;
	FILE *fp;
	fp = fopen("test.txt", "a");//opening file  
	gettimeofday(&tv, NULL);
	double time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
	sprintf(buff,"%s Proc[%s], time: %lf\n", stat,proc_i.proc_name,time_in_mill);
	fprintf(fp, buff);//writing data into file
	fflush(fp);
}
int run_proc(Proc proc_i){
	int pid = fork();
	if (pid == 0) {//child!
		
		write_log(proc_i,"START");
		for (int i = 0; i < proc_i.e_time; i++){
			run_unit();
			write_log(proc_i,"RUN");	
   		}
		write_log(proc_i,"DONE");
		exit(0);
	}
	set_cpu(pid, 1);//child cpu is 1
	return pid;
}

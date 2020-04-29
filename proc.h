#ifndef _PROC_C_
#define _PROC_C_

#define _GNU_SOURCE  
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <sys/time.h>
struct proc
{
	pid_t pid;
	char proc_name[33];
	int r_time;
	int e_time;
} typedef Proc;

void run_unit();
void set_cpu(int pid, int core_index);

void set_unblock(int pid);

void set_block(int pid);

int run_proc(Proc proc_i);
void write_log(Proc proc_i, char stat[]);

#endif
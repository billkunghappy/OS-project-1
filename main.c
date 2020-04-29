#define _GNU_SOURCE  
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "proc.h"
#include "schedular.h"


int cmp(const void *a, const void *b){
	int ret_val =  ((struct proc *)a)->r_time - ((struct proc *)b)->r_time;
	if (ret_val==0)//do not swap if r_time same
		ret_val=-1;
	return ret_val;
}

int main(int argc, char const *argv[])
{
	// set highest priority for this proc
	set_cpu(getpid(),0);
	set_unblock(getpid());
	char sched_type[6];
	int proc_num;
	scanf("%s %d",sched_type,&proc_num);
	Proc proc_list[proc_num];

	for (int i = 0; i < proc_num; ++i)
		scanf("%s %d %d",proc_list[i].proc_name, &(proc_list[i].r_time), &(proc_list[i].e_time));
	//sort proc list with ready time
	qsort(proc_list,proc_num,sizeof(Proc),cmp);
	for (int i = 0; i < proc_num; ++i)
	{
		printf("%s\n",proc_list[i].proc_name );
	}
	//schedulaing
	int cur_exec  = -1; // currently executing proc, -1 means none proc executing
	int next_exec = -1;
	int now_time = 0;
	while(1){
		//let's fork all the available proc!
		for (int i = 0; i < proc_num; ++i){
			if (proc_list[i].e_time > 0 && proc_list[i].r_time == now_time){
				printf("run %s !\n",proc_list[i].proc_name );
				//let's run!
				proc_list[i].pid = run_proc(proc_list[i]);
				//block it first
				set_block(proc_list[i].pid);
				write_log(proc_list[i], "BLOCK");

				printf("block %s\n",proc_list[i].proc_name );
			}
		}
		//check which proc to run
		if (strcmp(sched_type,"FIFO") == 0)
			next_exec = FIFO(proc_list, proc_num, now_time);// if can't run any, return -1
		printf("%d, %d\n",now_time,next_exec );
		// if (strcmp(sched_type,"RR") == 0)
		// 	RR(proc_list);
		// if (strcmp(sched_type,"SJF") == 0)
		// 	SJF(proc_list);
		// if (strcmp(sched_type,"PSJF") == 0)
		// 	PSJF(proc_list);
		//run next
		if (next_exec != cur_exec){//context switch
			if (cur_exec!=-1)//has runnging proc, give low priority
				set_block(proc_list[cur_exec].pid);
			printf("unblock %s\n",proc_list[next_exec].proc_name );
			set_unblock(proc_list[next_exec].pid);
			write_log(proc_list[next_exec], "UNBLOCK");
		}
		cur_exec = next_exec;
		run_unit();
		if (cur_exec != -1)
			proc_list[cur_exec].e_time -= 1;
		//Check if its done
		if(proc_list[cur_exec].e_time == 0)//done
			waitpid(proc_list[cur_exec].pid, NULL, 0);
		//check all process done
		int all_done=1;
		for (int i = 0; i < proc_num; ++i){
			if (proc_list[i].e_time > 0)
				all_done=0;
		}
		if (all_done==1)
			break;
		//add now time
		now_time+=1;
	}

	return 0;
}
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
	//schedulaing
	int cur_exec  = -1; // currently executing proc, -1 means none proc executing
	int next_exec = -1;
	int now_time = 0;
	int RR_turn = 0;
	int RR_queue[30]={0};
	int RR_queue_in[30]={0};
	int RR_s=0;
	int RR_e=0;
	while(1){
		//let's fork all the available proc!
		for (int i = 0; i < proc_num; ++i){
			if (proc_list[i].e_time > 0 && proc_list[i].r_time == now_time){
				//let's run!
				proc_list[i].pid = run_proc(proc_list[i]);
				//block it first
				set_block(proc_list[i].pid);

			}
		}
		//check which proc to run
		if (strcmp(sched_type,"FIFO") == 0)
			next_exec = FIFO(proc_list, proc_num, now_time);// if can't run any, return -1

		if (strcmp(sched_type,"RR") == 0){
			if (cur_exec != -1){//has proc
				if (RR_turn>=500){//pop
					RR_pop(cur_exec, RR_queue_in,RR_queue, &RR_s, &RR_e);
					next_exec = RR_push(proc_list,proc_num, now_time,cur_exec, RR_queue_in,RR_queue, &RR_s, &RR_e);
					if(next_exec==-1){
						next_exec = RR_push(proc_list,proc_num, now_time,-1, RR_queue_in,RR_queue, &RR_s, &RR_e);
					}
					RR_turn = 0;

				}
				else
					next_exec = cur_exec;
			}
			else// no current proc
				next_exec = RR_push(proc_list,proc_num, now_time,cur_exec, RR_queue_in,RR_queue, &RR_s, &RR_e);
			if (next_exec != -1)//run something
				RR_turn+=1;
		}
		if (strcmp(sched_type,"SJF") == 0){
			if (cur_exec != -1 && proc_list[cur_exec].e_time > 0){//some proc running
				next_exec = cur_exec;
			}
			else
				next_exec = SJF(proc_list, proc_num, now_time);
		}
		if (strcmp(sched_type,"PSJF") == 0)
			next_exec = PSJF(proc_list, proc_num, now_time);
		//run next
		if (next_exec != cur_exec){//context switch
			if (cur_exec!=-1)//has runnging proc, give low priority
				set_block(proc_list[cur_exec].pid);

			if (next_exec!=-1){
				set_unblock(proc_list[next_exec].pid);
				set_cpu(proc_list[next_exec].pid, 1);//child cpu is 1
			}
		}
		cur_exec = next_exec;
		//run a unit of time
		run_unit();
		if (cur_exec != -1)
			proc_list[cur_exec].e_time -= 1;
		//Check if its done
		if(cur_exec !=-1 && proc_list[cur_exec].e_time == 0){//done
			waitpid(proc_list[cur_exec].pid, NULL, 0);
		}
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

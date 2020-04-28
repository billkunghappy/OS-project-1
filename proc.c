#include <sched.h>
#include <unistd.h>
struct proc
{
	pid_t pid;
	char proc_name[33];
	int r_time;
	int e_time;
} typedef Proc;

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
		for (int i = 0; i < proc_i.e_time; i++)
			run_unit();
		exit(0);
	}
	set_cpu(pid, 1);//child cpu is 1
	return pid;
}
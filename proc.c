#define __USE_GNU  
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
		char buff[100] ;
		struct timeval  tv;
		FILE *fp;
		fp = fopen("test.txt", "a");//opening file  
		gettimeofday(&tv, NULL);
		double time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
		sprintf(buff,"Start Proc[%s], time: %lf\n", proc_i.proc_name,time_in_mill);
    	fprintf(fp, buff);//writing data into file

		for (int i = 0; i < proc_i.e_time; i++){
			run_unit();
			fprintf(fp, proc_i.proc_name);//writing data into file
   		}
		gettimeofday(&tv, NULL);
		time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
		sprintf(buff,"Proc[%s], time: %lf\n", proc_i.proc_name,time_in_mill);
    	fprintf(fp, buff);//writing data into file
		exit(0);
	}
	set_cpu(pid, 1);//child cpu is 1
	return pid;
}
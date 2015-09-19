#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <string.h>

void init_syscall_limit()
{
	//empty
}

int file_exist(const char* path)
{
	FILE *file;
	if (file = fopen(path, "r"))
	{
		fclose(file);
		return 1;
	}
	return 0;
}

void run_solution() 
{
	const char *exe_name = "./a.out";
	const char *stdin_name = "in";
	const char *stdout_name = "out";
	const char *stderr_name = "err";

	freopen(stdin_name, "r", stdin);
	freopen(stdout_name, "w", stdout);
	freopen(stderr_name, "a+", stderr);
	
	ptrace(PTRACE_TRACEME, 0, NULL, NULL);
	execl(exe_name, exe_name, (char *)NULL);

	exit(0);
}

void watch_solution(int pid)
{
	int status;
	struct user_regs_struct reg;
	int w;
	time_t s_start;
	time_t s_now;
	struct timespec spec;
	time_t s_limit;
	struct rlimit file_limit;	
	struct rlimit cpu_limit;
	struct rlimit old_limit;	

	//elapsed time check
	s_limit = 10;
	clock_gettime(CLOCK_REALTIME, &spec);	
	s_start = spec.tv_sec;
	
	//file limit
	file_limit.rlim_cur = 5 * 1024;
	file_limit.rlim_max = file_limit.rlim_cur;
	prlimit(pid, RLIMIT_FSIZE, &file_limit, &old_limit);

	//cpu usage time limit
	cpu_limit.rlim_cur = 3;
	cpu_limit.rlim_max = cpu_limit.rlim_max;
	prlimit(pid, RLIMIT_CPU, &cpu_limit, &old_limit);

	do
	{
		ptrace(PTRACE_GETREGS, pid, NULL, &reg);
		
		w = waitpid(pid, &status, 0);
		if (w==-1)
		{
			printf("error\n");
			exit(EXIT_FAILURE);
		}

		//elapsed time check
		clock_gettime(CLOCK_REALTIME, &spec);
		s_now = spec.tv_sec;
		if (s_now > s_start + s_limit) 
		{
			ptrace(PTRACE_KILL, pid, NULL, NULL);
		}		

		ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
	} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	exit(EXIT_SUCCESS);
}

int main()
{
	int pid;

	init_syscall_limit();
	pid = fork();
	if (pid == 0)
	{
		run_solution();
	} 
	else 
	{
		watch_solution(pid);
	}

	return 0;
}

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

const char* exe_cmd = "./a.out";

void init_syscall_limit()
{
	//nothing
}

void run_solution()
{
	freopen("in", "r", stdin);
	freopen("out", "w", stdout);
	freopen("err", "a+", stderr);

	ptrace(PTRACE_TRACEME, 0, NULL, NULL);
	execl(exe_cmd, exe_cmd, (char *)NULL);
	exit(0);
}

void watch_solution(int pid)
{
	int status;
	int w;
	//struct user_regs_struct reg;
	time_t s_start;
	time_t s_now;
	time_t s_limit;
	struct timespec spec;
	struct rlimit file_limit;
	struct rlimit cpu_limit;
	struct rlimit old_limit;

	//elapsed time limit
	s_limit = 10;
	clock_gettime(CLOCK_REALTIME, &spec);
	s_start = spec.tv_sec;


	//file access size limit
	file_limit.rlim_cur = 5 * 1024 * 1024;
	file_limit.rlim_max = file_limit.rlim_cur;
	prlimit(pid, RLIMIT_FSIZE, &file_limit, &old_limit);

	//cpu usage time limit
	cpu_limit.rlim_cur = 10;
	cpu_limit.rlim_max = cpu_limit.rlim_max;
	prlimit(pid, RLIMIT_CPU, &cpu_limit, &old_limit);

	do
	{
		ptrace(PTRACE_GETREGS, pid, NULL, NULL);
		w = waitpid(pid, &status, 0);
		if (w==-1)
		{
			printf("error on waitpid\n");
			exit(-1);
		}

		//elapsed time limit
		clock_gettime(CLOCK_REALTIME, &spec);
		s_now = spec.tv_sec;
		if (s_now > s_start + s_limit)
		{
			ptrace(PTRACE_KILL, pid, NULL, NULL);
		}

		ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
	} while (!WIFEXITED(status) && !WIFSIGNALED(status));

	exit(0);
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
		watch_solution();
	}

	return 0;
}
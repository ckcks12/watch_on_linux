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

const char *src_path = "src.c";
const char *exe_path = "a.out";
const char *stdin_path = "in";
const char *stdout_path = "out";
const char *stderr_path = "err";
const char *compile_cmd = "gcc";

size_t get_file_size(const char *path)
{
	FILE *fp;
	size_t size;

	fp = fopen(path, "r");
	if (fp)
	{
		fseek(fp, 0L, SEEK_END);
		size = ftell(fp);
		fclose(fp);
		return size;		
	}
	
	return -1; //failed
}

int compile()
{
	int pid;

	pid=fork();
	if (pid==0)
	{
		freopen(stdout_path, "w", stderr);
		execlp(compile_cmd, compile_cmd, src_path, "-o", exe_path, NULL);
		printf("execlp error\n");
	}
	else
	{
		waitpid(pid, 0, 0);
		if (get_file_size(stdout_path))
		{
			exit(0); 
		}
	}
}

void init_syscall_limit()
{
	//empty
}

void run_solution() 
{
	freopen(stdin_path, "r", stdin);
	freopen(stdout_path, "w", stdout);
	freopen(stderr_path, "a+", stderr);
	
	ptrace(PTRACE_TRACEME, 0, NULL, NULL);
	execl(exe_path, exe_path, (char *)NULL);

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
	int p;

	//elapsed time check
	s_limit = 10;
	clock_gettime(CLOCK_REALTIME, &spec);	
	s_start = spec.tv_sec;
	
	//file limit
	file_limit.rlim_cur = 1 * 1024 * 1024;
	file_limit.rlim_max = file_limit.rlim_cur;
	prlimit(pid, RLIMIT_FSIZE, &file_limit, &old_limit);

	//cpu usage time limit
	cpu_limit.rlim_cur = 3;
	cpu_limit.rlim_max = cpu_limit.rlim_max;
	prlimit(pid, RLIMIT_CPU, &cpu_limit, &old_limit);

	do
	{
		p = (int)ptrace(PTRACE_GETREGS, pid, NULL, &reg);
		
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
		//printf("s_start=%lld\t\ts_now=%lld\n", (long long)s_start, (long long)s_now); 		

		ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
	} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	exit(EXIT_SUCCESS);
}

int main()
{
	int pid;

	compile();
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
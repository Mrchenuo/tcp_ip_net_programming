//
// Created by nuo_nuaa on 2022/6/1.
//
#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char* argv[])
{
	int fds[2];
	const char* str = "who are you?";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds);
	pid = fork();
	if (pid==0) { // 子进程
		write(fds[1], str, sizeof buf);
	}
	else {
		read(fds[0], buf, BUF_SIZE);
		puts(buf);
	}
	return 0;
}

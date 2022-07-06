//
// Created by nuo_nuaa on 2022/6/1.
//
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define BUF_SIZE 30

int main(int argc, char* argv[])
{
	int fds[2];
	const char* str = "who are you?";
	const char* str2 = "Thank you for your message";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds);
	pid = fork();
	if (pid==0) { // 子进程
		write(fds[1], str, strlen(str));
//		sleep(2);
		memset(buf, 0, sizeof(buf));
		read(fds[0], buf, BUF_SIZE);
		printf("Child proc output: %s \n", buf);
	}
	else {
		memset(buf, 0, sizeof(buf));
		read(fds[0], buf, BUF_SIZE);
		printf("Parent proc output: %s \n", buf);
		write(fds[1], str2, strlen(str2));
//		sleep(3);
	}
	return 0;
}

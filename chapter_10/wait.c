//
// Created by nuo_nuaa on 2022/5/30.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	int status;
	pid_t pid = fork();
	if (pid==0) {
		return 3;
	}
	else {
		printf("Child Process ID: %d \n", pid);
		pid = fork();
		if (pid==0)
			exit(7);
		else {
			printf("Child Process ID: %d \n", pid);
			wait(&status);
			if (WIFEXITED(status))
				printf("Child send one: %d \n", WEXITSTATUS(status));

			wait(&status);
			if (WIFEXITED(status))
				printf("Child send one: %d \n", WEXITSTATUS(status));

			sleep(30);
		}
	}

	if (pid==0) {
		puts("End child process");
	}
	else {
		puts("End parent process");
	}
	return 0;
}

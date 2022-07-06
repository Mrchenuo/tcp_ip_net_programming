//
// Created by nuo_nuaa on 2022/6/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <wait.h>
#include <fcntl.h>

#define BUF_SIZE 1024

int serv_sock;
int clnt_sock;

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void urg_handler(int sig)
{
	int str_len;
	char buf[BUF_SIZE];
	str_len = recv(clnt_sock, buf, sizeof(buf)-1, MSG_OOB);
	buf[str_len] = 0;
	printf("Urgent message: %s \n", buf);
}

int main(int argc, char* argv[])
{
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	int fds[2];

	pid_t pid;
	struct sigaction act;
	socklen_t clnt_addr_size;
	char message[BUF_SIZE];
	int str_len, state;

	if (argc!=2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	act.sa_handler = urg_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock==-1)
		error_handling("socket() error");
	printf("Server socket: %d \n", serv_sock);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // convert between host and network byte order
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("bind() error");

	if (listen(serv_sock, 5)==-1)
		error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

	fcntl(clnt_sock, F_SETOWN, getpid());
	state = sigaction(SIGURG, &act, 0);

	while ((str_len = recv(clnt_sock, message, BUF_SIZE, 0))!=0) {
		if (str_len==-1)
			continue;
		message[str_len] = 0;
		puts(message);
	}

	close(clnt_sock);
	close(serv_sock);
	return 0;
}

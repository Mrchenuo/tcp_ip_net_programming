//
// Created by nuo_nuaa on 2022/6/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int sock;
	char message[BUF_SIZE];
	int str_len = 0;
	struct sockaddr_in serv_addr;

	if (argc!=3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock==-1)
		error_handling("socket() error");

	memset(&message, 0, sizeof(message));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected.......");

	write(sock, "123", strlen("123"));
	send(sock, "4", strlen("4"), MSG_OOB);
	write(sock, "567", strlen("567"));
	send(sock, "890", strlen("890"), MSG_OOB);

	close(sock);
	return 0;
}

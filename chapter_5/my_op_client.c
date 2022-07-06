//
// Created by nuo_nuaa on 2022/5/19.
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

	while (1) {
		fputs("Operand count: ", stdout);
		char op_count = getchar();

		if ((op_count=='q') || (op_count=='Q'))
			break;

		if (op_count<'0' || op_count>'9') {
			printf("Error input\n");
			continue;
		}

		message[0] = op_count;
		int op_co = op_count-'0';
		int i = 1;
		for (; i<=op_co; i++) {
			printf("Operand %d:", i);
			scanf(&(message+i));

		}
		message[op_count] = '\0';
		write(sock, message, strlen(message));
		str_len = read(sock, message, BUF_SIZE-1);
		message[str_len] = 0;
		printf("Message from server: %s", message);
	}

	close(sock);
	return 0;
}

//
// Created by nuo_nuaa on 2022/5/22.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4 //运算结果的字节数
#define OPSZ 4 //待运算数字的字节数

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
	int result, opnd_cnt, i;
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

	fputs("Operand count: ", stdout);
	scanf("%d", &opnd_cnt);
	message[0] = (char)opnd_cnt;

	for (i = 0; i<opnd_cnt; i++) {
		printf("Operand %d: ", i+1);
		scanf("%d", (int*)&message[i*OPSZ+1]);
	}
	fgetc(stdin);
	fputs("Operator: ", stdout);
	scanf("%c", &message[opnd_cnt*OPSZ+1]);

	write(sock, message, opnd_cnt*OPSZ+2);
	read(sock, &result, RLT_SIZE);
	printf("Operator result: %d \n", result);

	close(sock);
	return 0;
}

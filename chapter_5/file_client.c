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

	fputs("Input file name(Q to quit): ", stdout);
	fgets(message, BUF_SIZE, stdin);

	if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
		return 0;

	write(sock, message, strlen(message)-1);

	int fileSize = 0;
	read(sock, &fileSize, sizeof(fileSize));
	char* fileContent = (char*)malloc(fileSize+1);//动态分配str内存
	fileContent[0] = 0;//字符串置空
	char txt[1000];
	memset(txt, 0, sizeof(txt));
	int recv_len = 0;
	while (fileSize>recv_len) {
		int recv_cnt = read(sock, &fileContent[recv_len], BUF_SIZE-1);
		recv_len += recv_cnt;
	}
	close(sock);

	int j = 0;
	for (; j<fileSize; j++)
		printf("%c", fileContent[j]);
	//将文件写入
//    FILE *fp = NULL;
//    fp = fopen("../2.txt", "w");
//    fputs(fileContent,fp);
//    fclose(fp);

	return 0;
}

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
	FILE* fp;

	char buf[BUF_SIZE];
	int read_cnt;
	struct sockaddr_in serv_addr;

	if (argc!=3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	fp = fopen("receive.bat", "wb");
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock==-1)
		error_handling("socket() error");

	memset(&buf, 0, sizeof(buf));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected.......");

	printf("%ld\n", ftell(fp));
	while ((read_cnt = read(sock, buf, BUF_SIZE))!=0) {
		//每写一部分，fp会偏移到文件尾部；读同理
		fwrite((void*)buf, 1, read_cnt, fp);
		printf("%ld\n", ftell(fp));
	}
	puts("Recvived file data");
	write(sock, "Thank you", 10);
	fclose(fp);
	close(sock);
	return 0;
}

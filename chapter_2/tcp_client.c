//
// Created by nuo_nuaa on 2022/5/19.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len = 0;
	int idx = 0, read_len = 0;

	if (argc!=3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock==-1)
		error_handling("socket() error");
	printf("Client socket: %d \n", sock);

	memset(&message, 0, sizeof(message));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	time_t result = time(NULL);
	if (result!=(time_t)(-1))
		printf("Before connect() time is %s\n", asctime(gmtime(&result)));

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error!");

	result = time(NULL);
	if (result!=(time_t)(-1))
		printf("After connect() time is %s\n", asctime(gmtime(&result)));

	result = time(NULL);
	if (result!=(time_t)(-1))
		printf("Before read() time is %s\n", asctime(gmtime(&result)));

	while ((read_len = read(sock, &message[idx++], 1))) {
		if (read_len==-1)
			error_handling("read() error");

		str_len += read_len;
	}
	result = time(NULL);
	if (result!=(time_t)(-1))
		printf("After read() time is %s\n", asctime(gmtime(&result)));

	printf("Message from server: %s \n", message);
	printf("Function read call count: %d \n", str_len);
	close(sock);
	return 0;
}

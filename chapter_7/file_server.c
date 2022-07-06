#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define BUF_SIZE 30

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int serv_sd;
	int clnt_sd;
	FILE* fp;
	char buf[BUF_SIZE];
	memset(&buf, 0, BUF_SIZE);
	int read_cnt;

	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_size;

	if (argc!=2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	fp = fopen("file_server.c", "rb");
	serv_sd = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sd==-1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); // convert between host and network byte order
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");

	if (listen(serv_sd, 5)==-1)
		error_handling("listen() error");

	clnt_adr_size = sizeof(clnt_adr);
	int i;
	for (i = 0; i<5; i++) {
		clnt_sd = accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_size);
		if (clnt_sd==-1)
			error_handling("accept() error");
		else
			printf("Connected client: %d \n", i+1);

		while (1) {
			memset(&buf, 0, BUF_SIZE);
			read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
			// 说明是最后一波了
			if (read_cnt<BUF_SIZE) {
				write(clnt_sd, buf, read_cnt);
				break;
			}
			write(clnt_sd, buf, BUF_SIZE);
		}

		shutdown(clnt_sd, SHUT_WR);

		memset(&buf, 0, BUF_SIZE);
		read(clnt_sd, buf, BUF_SIZE);
		printf("Message from client: %s \n", buf);
		fclose(fp);
		close(clnt_sd);
	}

	close(serv_sd);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define BUF_SIZE 1024

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int file_size2(const char* filename)
{
	struct stat statbuf;
	stat(filename, &statbuf);
	int size = statbuf.st_size;

	return size;
}

int main(int argc, char* argv[])
{
	int serv_sock;
	int clnt_sock;
	char message[BUF_SIZE];
	memset(&message, 0, BUF_SIZE);
	int str_len = 0, i;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	if (argc!=2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

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
	for (i = 0; i<5; i++) {
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		if (clnt_sock==-1)
			error_handling("accept() error");
		else
			printf("Connected client: %d \n", i+1);

		if ((str_len = read(clnt_sock, message, BUF_SIZE))<0) {
			error_handling("accept() error");
		}
		const char* constc = message;
		FILE* fp = fopen(constc, "r");
		if (fp==NULL) {
			printf("No such file\n");
		}
		else {
			int fileSize = file_size2(constc);
			write(clnt_sock, &fileSize, sizeof(fileSize)); //先传输文件大小
			memset(message, 0, BUF_SIZE);
			int rdSize = 0;
			while ((rdSize = fread(message, fileSize, 1, fp))>0) //传输文件
			{
				if (write(clnt_sock, message, sizeof(message))<0) {
					error_handling("transfer file error");
				}
				memset(message, 0, BUF_SIZE);
			}
			fclose(fp);
		}
		close(clnt_sock);
	}

	close(serv_sock);
	return 0;
}

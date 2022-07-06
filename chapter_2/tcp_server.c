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
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
	char message[] = "hello world";

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

	time_t result = time(NULL);
	if (result!=(time_t)(-1))
		printf("Before accept() time is %s\n", asctime(gmtime(&result)));

	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if (clnt_sock==-1)
		error_handling("accept() error");
	printf("Accept socket: %d \n", clnt_sock);

	result = time(NULL);
	if (result!=(time_t)(-1))
		printf("After accept() time is %s\n", asctime(gmtime(&result)));

	result = time(NULL);
	if (result!=(time_t)(-1))
		printf("Before write() time is %s\n", asctime(gmtime(&result)));

	write(clnt_sock, message, sizeof(message));

	result = time(NULL);
	if (result!=(time_t)(-1))
		printf("After write() time is %s\n", asctime(gmtime(&result)));
	close(clnt_sock);
	close(serv_sock);
	return 0;
}

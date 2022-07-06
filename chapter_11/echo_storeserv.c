#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <wait.h>

#define BUF_SIZE 1024
void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void read_childproc(int sig)
{
	int status;
	pid_t id = waitpid(-1, &status, WNOHANG);
	if (WIFEXITED(status)) {
		printf("Removed proc id: %d \n", id);
		printf("Child send: %d \n", WEXITSTATUS(status));
	}
}

int main(int argc, char* argv[])
{
	int serv_sock;
	int clnt_sock;
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

	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0);

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

	pipe(fds);
	pid = fork();
	if (pid==0) {
		FILE* fp = fopen("echomsg.txt", "w+");
		char msgbuf[BUF_SIZE];
		int i, len;

		for (i = 0; i<10; i++) {
			len = read(fds[0], msgbuf, BUF_SIZE);
			fwrite((void*)msgbuf, 1, len, fp);
		}
		fclose(fp);
		return 0;
	}

	while (1) {
		clnt_addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		if (clnt_sock==-1)
			continue;
		else
			puts("new  client: connected \n");

		pid = fork();
		if (pid==-1) {
			close(clnt_sock);
			continue;
		}

		if (pid==0) {
			close(serv_sock);
			while ((str_len = read(clnt_sock, message, BUF_SIZE))!=0) {
				write(clnt_sock, message, str_len);
				write(fds[1], message, str_len);
			}
			close(clnt_sock);
			puts("client disconnected...");
			return 0;
		}
		else
			close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}

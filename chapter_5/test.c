//
// Created by nuo_nuaa on 2022/5/22.
//
#include <stdio.h>
#define BUF_SIZE 1024
#define OPSZ 4
int main()
{
//    FILE *fp = fopen("/media/storage/nuo_chen/developLearning/tcp_ip_net_programming/chapter_5/1.txt", "r");
	FILE* fp = fopen("../chapter_5/1.txt", "r");
	if (fp==NULL) {
		printf("No such file\n");
	}

	char message[BUF_SIZE];
//    fputs("Operand count: ",stdout);
	printf("Input:\n");
	char op_count;
	scanf("%c", &op_count);

	if ((op_count=='q') || (op_count=='Q'))
		printf("Error input\n");

	if (op_count<'0' || op_count>'9') {
		printf("Error input\n");
	}

	message[0] = op_count;
	int op_co = op_count-'0';
	int i = 0;
	for (; i<=op_co; i++) {
		printf("Operand %d:", i+1);
		scanf("%c", &message[OPSZ*i+1]);
	}
	message[OPSZ*op_co+1] = '\0';
	printf("%s:", message);
	return 0;
}

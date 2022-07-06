#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	printf("hello ");
	if (0==fork()) {
		printf("child\n");
		return 0;
	}
	printf("parent\n");
	return 0;
}

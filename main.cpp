#include "tools.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

int main()
{
	if (IsLittleIndian()) {
		printf("small\n");
	} else {
		printf("big\n");
	}
	char buf[50];
	unsigned int test = (unsigned int)&buf;

	int l = 0x01020304;
	char *str = "hello world\n";
	char *input;
	unsigned int len;
	GetLocalTime(&buf[0]);
	printf("pid:%u, %s start\n", getpid(), buf);
	ReadFileToSingleBuffer("input.jpg", &input, len);
	GetLocalTime(&buf[0]);
	printf("pid:%u, %s read end\n", getpid(), buf);
	if (len > 0)
		WriteToFile("output.jpg", input, len);
	GetLocalTime(&buf[0]);
	printf("pid:%u, %s write end\n", getpid(), buf);

	delete[]input;

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char* argv[])
{
	if (argc < 3) {
		fprintf(stderr, "Usage: inf tag interval\n");
	} else {
		const char* tag = argv[1];
		int interval = atoi(argv[2]);
		int count = 0;
		while(1) {
			printf("%s\n", tag);
			count++;
			sleep(interval);
		}
	}
}
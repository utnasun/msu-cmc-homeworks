#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	for (int i=0; i < 5; i++) {
		printf("My pid: %d, parent pid: %d, my arg: %s\n", getpid(),getppid(), argv[1]);
		sleep(1);
	}
	return 0;
}

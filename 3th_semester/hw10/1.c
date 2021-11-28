#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void sigint_handler(int sig) {
	puts("Handling SIGINT...");
}

int main() {
	signal(SIGINT, sigint_handler); 
	printf("Sending SIGINT...\n");
	kill(getpid(),SIGINT);					// посылаем SIGINT самому себе
	printf("Sending SIGINT again...\n");
	kill(getpid(),SIGINT);					// посылаем SIGINT ещё раз
	printf("Exiting program\n");
	return 0;
}


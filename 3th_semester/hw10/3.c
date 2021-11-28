#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

void sigint_handler(int sig) {	
	printf("Handling SIGINT...\n");
}

int main() {
	signal(SIGINT, sigint_handler);	
	kill(0, SIGINT);				// посылаем сигнал группе
	return 0;
}

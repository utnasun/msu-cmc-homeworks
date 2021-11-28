#include <stdio.h>
#include <signal.h>

void sigtstp_handler(int sig) { // обработчик SIGTSTP
	printf("Handling SIGTSTP...\n");
}

void sigint_handler(int sig) {	// обработчик SIGINT, выполняется бесконечно долго
	printf("Handling SIGINT infinitely long...\n");
	while (1) {
		
	}
}

int main() {
	signal(SIGINT, sigint_handler);		
	signal(SIGTSTP, sigtstp_handler);
	while (1) {							// входим в бесконечный цикл и ждём сигнала

	}
	return 0;
}

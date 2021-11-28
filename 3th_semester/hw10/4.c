#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

void sigint_handler(int sig) {	
	printf("Handling SIGINT...\n");
}

void child(pid_t parentpid) {	
	kill(parentpid, SIGINT);	
	while (1) {

	}
}

void parent() {					
	int status;
	signal(SIGINT, sigint_handler);
	pid_t pid = wait(&status);	
	if (pid == -1) {
		printf("Произошла ошибка с wait\n");
	} else {
		printf("Ошибки с wait не произошло\n");
	}
}

int main() {
	pid_t pid, parentpid;
	parentpid = getpid(); 
	pid = fork();			
	if (pid == 0) {			// son
		child(parentpid);
	} else if (pid > 0) {	// father
		parent();
	} else {
		printf("Error");
		exit(0);
	}
	return 0;
}

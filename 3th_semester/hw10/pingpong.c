// ping-pong
// порождение процессов по схеме сын-внук-правнук-...
// использование одного канала -- pipe, синхронизация с помощью сигнала SIGUSR1

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

long int read_number(const char * s) { // функция для считывания числа из строки, обрабатывает ошибки
	if (*s == '\0') {
		puts("Передана пустая строка");
		exit(EXIT_FAILURE);
	}
	long int ret;
	char* endptr;
	errno = 0;
	ret = strtol(s, &endptr, 10);	// само считывание числа
	if (*endptr != '\0') {	// указатель на первый невалидный символ должен указывать на конец строки
		puts("Переданный аргумент не является целым числом");
		exit(EXIT_FAILURE);
	}
	if (errno == ERANGE) {	// если переданное число больше, чем может поместиться в long
		puts("Переданный аргумент выходит за пределы допустимых значений");
		exit(EXIT_FAILURE);
	} else if (errno != 0) {// остальные ошибки
		puts("Произошла какая-то ошибка");
		exit(EXIT_FAILURE);
	}
	return ret;
}

void ping_pong(pid_t nextpid, int read_fd, int write_fd, long int b, sigset_t sigset) {	// основная логика программы
	int sig;
	long int a;
	sigwait(&sigset, &sig); // ждём сигнала
	read(read_fd, &a, sizeof(long int)); // считываем число из pipe
	while (a <= b) {
		printf("pid = %d, число: %ld\n", getpid(), a);	// печатаем число
		a++;	// увеличиваем
		write(write_fd, &a, sizeof(long int)); // пишем следующее число в pipe
		kill(nextpid, SIGUSR1); // отсылаем сигнал следующему процессу
		sigwait(&sigset, &sig); // ждём сигнала
		read(read_fd, &a, sizeof(long int)); // считываем число
	}
	kill(nextpid, SIGUSR1);	// отсылаем сигнал следующему процессу, чтобы он проснулся и завершился
	write(write_fd, &a, sizeof(long int)); // и отсылаем ему число
}

int main(int argc, char ** argv) {
	long n, a, b, i;
	int fd[2]; // дескрипторы для pipe
	if (argc < 4) {	// проверяем, что в командной строке передано достаточно аргументов
		printf("Использование: %s N A B\nN -- число процессов; A, B -- начальное и конечное целые числа\n", argv[0]);
		return 0;
	}
	n = read_number(argv[1]);	// считываем числа из аргументов командной строки
	a = read_number(argv[2]);
	b = read_number(argv[3]);
	if (n <= 1) {
		puts("Число процессов должно быть как минимум 2");
		exit(EXIT_FAILURE);
	}
	if (pipe(fd) != 0) { // пытаемся создать pipe
		puts("Ошибка при создании pipe");
		exit(EXIT_FAILURE);
	}
	sigset_t sigset;// создаём sigset -- маску для ожидания сигнала SIGUSR1
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	sigprocmask(SIG_BLOCK, &sigset, NULL); // блокирование сигналов из sigset
	pid_t parentpid = getpid();	// запоминаем pid изначального процесса
	printf("parentpid = %d\n", parentpid);
	pid_t nextpid;				// pid следующего процесса (того, которому этот будет передавать число)
	pid_t pid;					// переменная для выполнения функции fork()
	for (i = 1; i < n; ++i) {
		pid = fork();	// делаем fork
		if (pid == 0) { // новый процесс -- именно он будет дальше forkать
			if (i == n-1) {	// но если все процессы создались -- то можно начинать ping-pong
				nextpid = parentpid; // для него следующий процесс будет первым -- чтобы замкнуть круг
				write(fd[1], &a, sizeof(long int)); // пишем стартовое число
				kill(parentpid, SIGUSR1);			// и отсылаем сигнал изначальному процессу
				ping_pong(nextpid, fd[0], fd[1], b, sigset); // входим в ping-pong
				break;
			}
		} else if (pid > 0) { // старый процесс -- заходит в функцию и ждёт сигнала
			nextpid = pid;
			ping_pong(nextpid, fd[0], fd[1], b, sigset);
			break;
		} else {
			puts("Ошибка при создании нового процесса");
			exit(EXIT_FAILURE);
		}
	}
	if (getpid() == parentpid) { // изначальный процесс закрывает созданные дескрипторы
		close(fd[0]);
		close(fd[1]);
	}
	// строчка для того, чтобы убедиться, что все процессы завершили работу
	// printf("Process %d exited\n", getpid());
	return 0;
}

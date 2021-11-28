#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int status;
    char exe_1[80], exe_2[80];
    if (argc != 6) {
        printf("Error not enough arguments\n");
        return 1;
    }
    sprintf(exe_1, "%s%s %s", "./", argv[1], argv[2]);
    sprintf(exe_2, "%s%s %s", "./", argv[3], argv[4]);
    if (!strcmp(argv[5], "1")) {
        pid_t pid = fork();
        if (!pid) {
            char *const argv_[] = {"bash", "-c", exe_1, NULL};
            int ret = execv("/bin/bash", argv_);
            if (ret == -1) {
                printf("Error\n");
                return 1;
            }
        } else if (pid) {
            pid_t pid_ = fork();
            if (!pid_) {
                char *const argv_[] = {"bash", "-c", exe_2, NULL};
                int ret = execv("/bin/bash", argv_);
                if (ret == -1) {
                    printf("Error\n");
                    return 1;
                }
            } else if (pid_) {
                pid = wait(&status);
                if (WIFEXITED(status)) {
                    printf("WEXIT STATUS1=%d\n",WEXITSTATUS(status));
                }
                pid_ = wait(&status);
                if (WIFEXITED(status)) {
                    printf("WEXIT STATUS2=%d\n",WEXITSTATUS(status));
                }
            } else if (pid == -1) {
                printf("Error\n");
                return 1;
            }
        } else if (pid == -1) {
            printf("Error\n");
            return 1;
        }
    } else if (!strcmp(argv[5], "2")) {
        pid_t pid = fork();
        if (!pid) {
            int ret = execl("/bin/bash", "bash", "-c", exe_1, NULL);
            if (ret == -1) {
                printf("Error\n");
                return 1;
            }
        } else if (pid) {
            pid = wait(&status); // ждем завершение первой программы
            if (WIFEXITED(status)) {
                printf("WEXIT STATUS1=%d\n",WEXITSTATUS(status));
            }

            pid_t pid_ = fork();
            if (!pid_) {
                int ret = execl("/bin/bash", "bash", "-c", exe_2, NULL);
                if (ret == -1) {
                    printf("Error\n");
                    return 1;
                }
            } else if (pid_) {
                pid_ = wait(&status);
                if (WIFEXITED(status)) {
                    printf("WEXIT STATUS2=%d\n",WEXITSTATUS(status));
                }
            } else if (pid_ == -1) {
                printf("Error\n");
                return 1;
            }
        } else if (pid == -1) {
            printf("Error\n");
            return 1;
        }
    } else {
        printf("Error\n");
        return 1;
    }
    return 0;
}

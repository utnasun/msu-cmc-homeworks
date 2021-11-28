#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MSGMAX 16384

struct {
    long mtype;
    char data[MSGMAX];
} message;


int main(int argc, char *argv[]) {
    key_t key;
    int msgid,i;
    FILE *fp;

    key = ftok(argv[1], 's');

    /* подключаемся к очереди */
    msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }
    while (1) {
        for (i = 0 ; i < MSGMAX ; i++) {
            message.data[i] = 0;
        }
        msgrcv(msgid,&message, MSGMAX, 2, 0);
        if (message.data[0] == EOF ) {
            break;
        }
        fp = fopen(argv[2], "a+");
        fprintf(fp, "%d %s\n", getpid(), message.data);
        fclose(fp);
        message.mtype = 5;
        msgsnd(msgid,&message, strlen(message.data), 0);//отправляем сообщение серверу,что получили сообщение.
    }
    /* отправляем сообщение серверу о том что клиент завершил свою работу */
    message.mtype = 5;
    msgsnd(msgid,&message, strlen(message.data), 0);
    printf ("Client2 has finished work\n");
    //fclose(fp);
    return 0;
}

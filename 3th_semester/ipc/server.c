#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSGMAX 16384



struct {
    long mtype;
    char data[MSGMAX];
} message;


int main(int argc, char *argv[]) {
    key_t key;
    int msgid, i=0, who=1;
    char str[MSGMAX],c;
    FILE *fp;
    fp = fopen(argv[2], "w");
    fclose(fp); // для очистки файла вывода,после каждого запуска
    fp = fopen(argv[1], "r");
    key = ftok(argv[1], 's');
    msgid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL);// создаем новую очередь
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }
    while ((c = fgetc(fp)) != EOF) {
        if (c != '\n')
        {
            str[i++] = c;
        }
        else {
            str[i++] = '\0';
            message.mtype = who;
            strcpy(message.data, str);
            msgsnd(msgid, &message, strlen(str)+1, 0);
            msgrcv(msgid,&message, MSGMAX, 5, 0);
            who = (who == 1) ? 2 : 1;
            i = 0;
        }
    }
    message.data[0] = EOF;
    message.mtype = 1;
    msgsnd(msgid, &message, strlen(message.data)+1, 0);
    message.data[0] = EOF;
    message.mtype = 2;
    msgsnd(msgid, &message, strlen(message.data)+1, 0);

    msgrcv(msgid,&message, MSGMAX, 5, 0);
    msgrcv(msgid,&message, MSGMAX, 5, 0);

    msgctl(msgid, IPC_RMID, NULL);
    fclose(fp);
    printf("Server has finished work\n");
    return 0;
}

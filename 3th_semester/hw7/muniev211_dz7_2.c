#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#define mem 8
char * read_fstr(FILE * f);

int main(int argc, char ** argv) {
    FILE * file;
    int num = 10, linenum = 1, flag = 0;
    if (argc == 2) { // 0 parametrov , 1 file
        file = fopen(argv[1], "r");
    } else if (argc == 3) { // 1 parametr
        if (argv[1][0] == '-') {
            if (argv[1][1] == 's') {
                flag = 1;
            } else {
                num = atoi(argv[1]);
                num = num * (-1);
            }
        } else {
            linenum = atoi(argv[1]);
        }
        file = fopen(argv[2], "r");
    } else if (argc == 4) { // 2 parametra
        if (argv[1][0] == '-') {
            if (argv[1][1] == 's') {
                flag = 1;
            } else {
                num = atoi(argv[1]);
                num = num * (-1);
            }
        } else {
            linenum = atoi(argv[1]);
        }
        if (argv[2][0] == '-') {
            if (argv[2][1] == 's') {
                flag = 1;
            } else {
                num = atoi(argv[2]);
                num = num * (-1);
            }
        } else {
            linenum = atoi(argv[2]);
        }
        file = fopen(argv[3], "r");
    } else if (argc == 5) { // 3 parametra
        if (argv[1][0] == '-') {
            if (argv[1][1] == 's') {
                flag = 1;
            } else {
                num = atoi(argv[1]);
                num = num * (-1);
            }
        } else {
            linenum = atoi(argv[1]);
        }
        if (argv[2][0] == '-') {
            if (argv[2][1] == 's') {
                flag = 1;
            } else {
                num = atoi(argv[2]);
                num = num * (-1);
            }
        } else {
            linenum = atoi(argv[2]);
        }
        if (argv[3][0] == '-') {
            if (argv[3][1] == 's') {
                flag = 1;
            } else {
                num = atoi(argv[3]);
                num = num * (-1);
            }
        } else {
            linenum = atoi(argv[3]);
        }
        file = fopen(argv[4], "r");
    }
    //printf("NUM=%d\n",num);
    //printf("Linenum=%d\n",linenum);
    //printf("flag=%d\n",flag);
    char c;
    c = getchar();
    char * string;
    int i, linenum_flag = 1, empty_flag = 0;
    while ((c != 'q') && (!feof(file))) {
        if (flag == 1) {
            if (linenum_flag) {
                for (i = 1; i < linenum; i++) {
                    string = read_fstr(file);
                    free(string);
                }
            }
            linenum_flag = 0;
            if (c == ' ') {
                for (i = 1;
                    (i <= num) && (!feof(file)); i++) {
                    string = read_fstr(file);
                    if (strcmp(string, "\0") == 0) {
                        empty_flag++;
                        //printf("%d\n",empty_flag);
                    } else {
                        if (empty_flag > 0) {
                            //printf("%d\n",empty_flag);
                            //printf("Empty string");
                            printf("\n");
                            empty_flag = 0;
                        }
                    }
                    if (strcmp(string, "\0") != 0) {
                        printf("%s\n", string);
                    }
                    free(string);
                }
            }
            c = getchar();
        } else {
            if (linenum_flag) {
                for (i = 1; i < linenum; i++) {
                    string = read_fstr(file);
                    free(string);
                }
            }
            linenum_flag = 0;
            if (c == ' ') {
                for (i = 1;
                    (i <= num) && (!feof(file)); i++) {
                    string = read_fstr(file);
                    printf("%s\n", string);
                    free(string);
                }
            }
            c = getchar();
        }
    }
    fclose(file);
    return 0;
}
char * read_fstr(FILE * f) {
    int len = 0, c;
    char * s;
    c = fgetc(f);
    s = (char * ) malloc(sizeof(char));
    while ((c != EOF) && (c != '\n')) {
        *(s + len) = c;
        len++;
        if (len % 4 == 1) {
            s = realloc(s, (len / 4 + 1) * mem);
        }
        c = fgetc(f);
    }
    if (len % 4 == 0) {
        s = realloc(s, (len / 4) * mem + 1);
    }
    *(s + len) = '\0';
    return s;
}

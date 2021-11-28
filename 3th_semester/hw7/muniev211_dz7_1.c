#include <stdio.h>

#include <stdlib.h>

#define mem 8

int main(int argc, char * argv[]) {
    FILE * f1, * ptrfile;
    f1 = fopen(argv[1], "r");
    int n;
    n = atoi(argv[2]);
    ptrfile = tmpfile();
    if (f1 == NULL) {
        puts("Error");
        system("pause");
        return 1;
    }
    int len, c;
    char * s;
    while (!feof(f1)) {
        len = 0;
        c = fgetc(f1);
        s = (char * ) malloc(sizeof(char));
        while ((c != EOF) && (c != '\n')) {
            *(s + len) = c;
            len++;
            if (len % 4 == 1) {
                s = realloc(s, (len / 4 + 1) * mem);
            }
            c = fgetc(f1);
        }
        if (len % 4 == 0) {
            s = realloc(s, (len / 4) * mem + 1);
        }
        *(s + len) = '\0';
        if (len >= n) {
            fprintf(ptrfile, "%s\n", s);
        }
        free(s);
    }
    fclose(f1);
    f1 = fopen(argv[1], "w");
    fseek(ptrfile, 0, SEEK_SET);
    while (!feof(ptrfile)) {
        len = 0;
        c = fgetc(ptrfile);
        s = (char * ) malloc(sizeof(char));
        while ((c != EOF) && (c != '\n')) {
            *(s + len) = c;
            len++;
            if (len % 4 == 1) {
                s = realloc(s, (len / 4 + 1) * mem);
            }
            c = fgetc(ptrfile);
        }
        if (len % 4 == 0) {
            s = realloc(s, (len / 4) * mem + 1);
        }
        *(s + len) = '\0';
        fprintf(f1, "%s\n", s);
        free(s);
    }
    fclose(f1);
    fclose(ptrfile);
    return 0;
}

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <dirent.h>

#include <pwd.h>

char file_type(mode_t st_mode);
void print_rights(mode_t st_mode);
void print_all(struct stat buf);
void print_file(char * name, struct stat stbuf, int aflag, int lflag, int hflag);
void print_directory(char * dir_name, int a, int l, int R);

int main(int argc, char * argv[]) {
    int i, aflag = 0, lflag = 0, Rflag = 0, numflags = 0;
    for (i = 1; i < argc; i++) {
        if (strlen(argv[i]) <= 2) {
            if ((argv[i][0] == '-') && (argv[i][1] == 'a')) {
                aflag = 1;
                numflags++;
            }
            if ((argv[i][0] == '-') && (argv[i][1] == 'l')) {
                lflag = 1;
                numflags++;
            }
            if ((argv[i][0] == '-') && (argv[i][1] == 'R')) {
                Rflag = 1;
                numflags++;
            }
        } else break;
    }
    if (argc <= numflags + 1) {
        char * curdir = (char * ) malloc(512 * sizeof(char));
        getcwd(curdir, 512);
        print_directory(curdir, aflag, lflag, Rflag);
        free(curdir);
    } else {
        char * b = NULL;
        b = argv[numflags + 1];
        struct stat buf;
        if (!stat(b, & buf)) {
            if ((buf.st_mode & S_IFMT) == S_IFDIR) {
                print_directory(b, aflag, lflag, Rflag);
            } else {
                print_file(b, buf, 1, lflag, 0);
            }
        } else {
            puts("Error");
            return 1;
        }
    }
    return 0;
}
void print_directory(char * dir_name, int a, int l, int R) {
    DIR * mydir;
    mydir = opendir(dir_name);
    struct dirent * curfile;
    while ((curfile = readdir(mydir)) != NULL) {
        if (strcmp(curfile -> d_name, ".") && strcmp(curfile -> d_name, "..")) {
            int hflag = ( * (curfile -> d_name) == '.');
            struct stat stbuf;
            char * name = (char * ) malloc(512 * sizeof(char));
            strcpy(name, curfile -> d_name);
            //printf("Name:%s\n",name);
            stat(name, & stbuf);
            if ((stbuf.st_mode & S_IFMT) == S_IFDIR) {
                if (R) {
                    printf("Catalog: %s\n", name);
                    print_directory(name, a, l, R);
                }
            } else {
                print_file(name, stbuf, a, l, hflag);
            }
            free(name);
        }
    }
    free(mydir);
}
void print_all(struct stat buf) {
    struct passwd * user;
    printf("Type:");
    printf("%c\n", file_type(buf.st_mode));
    print_rights(buf.st_mode);
    printf("Nlinks=%ld\n", buf.st_nlink);
    printf("Bytes:%ld\n", buf.st_size);
    user = getpwuid(buf.st_uid);
    printf("Username:%s\n", user -> pw_name);
}
char file_type(mode_t st_mode) {
    if (st_mode & S_IFMT) {
        if ((st_mode & S_IFMT) == S_IFDIR) {
            return 'd';
        }
        if ((st_mode & S_IFMT) == S_IFCHR) {
            return 'c';
        }
        if ((st_mode & S_IFMT) == S_IFBLK) {
            return 'b';
        }
        if ((st_mode & S_IFMT) == S_IFREG) {
            return 'r';
        }
        if ((st_mode & S_IFMT) == S_IFLNK) {
            return 'l';
        }
        if ((st_mode & S_IFMT) == S_IFSOCK) {
            return 's';
        }
        if ((st_mode & S_IFMT) == S_IFIFO) {
            return 'f';
        }
    }
}
void print_file(char * name, struct stat stbuf, int aflag, int lflag, int hflag) {
    if ((aflag) || (hflag == 0)) {
        if (lflag) {
            printf("Name: %s\n", name);
            print_all(stbuf);
        } else {
            printf("%s\n", name);
        }
    }
}

void print_rights(mode_t st_mode) {
    printf("User rights:");
    if (st_mode & S_IRUSR) {
        printf(" R");
    }
    if (st_mode & S_IWUSR) {
        printf(" W");
    }
    if (st_mode & S_IXUSR) {
        printf(" X");
    }
    printf("  Group rights:");
    if (st_mode & S_IRGRP) {
        printf(" R");
    }
    if (st_mode & S_IWGRP) {
        printf(" W");
    }
    if (st_mode & S_IXGRP) {
        printf(" X");
    }
    printf("  Others rights:");
    if (st_mode & S_IROTH) {
        printf(" R");
    }
    if (st_mode & S_IWOTH) {
        printf(" W");
    }
    if (st_mode & S_IXOTH) {
        printf(" X");
    }
    printf("\n");
    return;
}

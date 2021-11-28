#include <stdio.h>

#include <stdlib.h>

#include <string.h>


int space_counter(char * string);
int my_strcmp(const char * s1,
  const char * s2, int sc1, int sc2);
int my_strcmp_bf(const char * s1,
  const char * s2, int sc1, int sc2);
int my_strcmp_f(const char * s1,
  const char * s2);
void main_sort(char * strings[], int len[], int n, int rflag, int fflag, int bflag);
int main(int argc, char * argv[]) {
  FILE * f1, * output;
  char c;
  int n = 0, i, bflag = 0, rflag = 0, fflag = 0, oflag = 0, numflags = 0, b = 0, backlen = 0;
  char * str = NULL;
  char ** strings = NULL;
  char ** backup = NULL;
  int * len = NULL;
  for (i = 1; i < argc; i++) {
    if (strlen(argv[i]) <= 2) {
      if ((argv[i][0] == '-') && (argv[i][1] == 'r')) {
        rflag = 1;
        numflags++;
      }
      if ((argv[i][0] == '-') && (argv[i][1] == 'f')) {
        fflag = 1;
        numflags++;
      }
      if ((argv[i][0] == '-') && (argv[i][1] == 'o')) {
        oflag = 1;
        numflags++;
      }
      if ((argv[i][0] == '-') && (argv[i][1] == 'b')) {
        bflag = 1;
        numflags++;
      }
    } else break;
  }
  //printf("Fflag=%d\n",fflag);
  //printf("Oflag=%d\n",oflag);
  //printf("Bflag=%d\n",bflag);
  //printf("Rflag=%d\n",rflag);
  if (oflag == 0) {
    for (i = 1 + numflags; i < argc; i++) {
      f1 = fopen(argv[i], "r");
      if (f1 == NULL) {
        puts("Error");
        system("pause");
        return 1;
      }
      while (!feof(f1)) {
        len = realloc(len, (n + 1) * sizeof(int)); // динамический массив длин строк
        len[n] = 0;
        while (((c = fgetc(f1)) != '\n') && (!feof(f1))) {
          len[n]++; // массив длин строк файла
        }
        n++; //число строк
      }
      //printf("Debug");
      fseek(f1, 0, SEEK_SET);
      strings = (char ** ) malloc(n * sizeof(char * )); // массив строк
      for (int j = 0; j < n - 1; j++) {
        str = (char * ) malloc((len[j] + 1) * sizeof(char)); //
        while (((c = fgetc(f1)) != '\n') && (!feof(f1))) {
          *(str + b) = c;
          b++;
        }
        *(str + b) = '\0';
        *(strings + j) = (char * ) malloc((len[j] + 1) * sizeof(char));
        strcpy(strings[j], str);
        free(str);
        b = 0;
      }
      main_sort(strings, len, n, rflag, fflag, bflag);
      for (int j = 0; j < n - 1; j++) {
        printf("%s\n", *(strings + j));
        free( * (strings + j));
      }
      free(strings);
      fclose(f1);
      n = 0;
      free(len);
      len = NULL;
    }
  } else {
    for (i = 2 + numflags; i < argc; i++) {
      if (strcmp(argv[i], argv[1 + numflags]) == 0) { // если один из файлов совпадает с файлом для вывода
        f1 = fopen(argv[i], "r");
        if (f1 == NULL) {
          puts("Error");
          system("pause");
          return 1;
        }
        while (!feof(f1)) {
          len = realloc(len, (n + 1) * sizeof(int));
          len[n] = 0;
          while (((c = fgetc(f1)) != '\n') && (!feof(f1))) {
            len[n]++; // массив длин строк файла
          }
          n++; //число строк
        }
        //printf("Debug");
        fseek(f1, 0, SEEK_SET);
        backup = (char ** ) malloc(n * sizeof(char * )); // сохраняем содержимое этого файла
        for (int j = 0; j < n - 1; j++) {
          str = (char * ) malloc((len[j] + 1) * sizeof(char)); //
          while (((c = fgetc(f1)) != '\n') && (!feof(f1))) {
            *(str + b) = c;
            b++;
          }
          *(str + b) = '\0';
          *(backup + j) = (char * ) malloc((len[j] + 1) * sizeof(char));
          strcpy(backup[j], str);
          free(str);
          b = 0;
        }
        main_sort(backup, len, n, rflag, fflag, bflag);
        fclose(f1);
        backlen = n;
        n = 0;
        free(len);
        len = NULL;
      }
    }
    output = fopen(argv[1 + numflags], "w");
    for (i = 2 + numflags; i < argc; i++) {
      if (strcmp(argv[i], argv[1 + numflags]) == 0) {
        for (int j = 0; j < backlen - 1; j++) {
          fprintf(output, "%s\n", *(backup + j));
          free( * (backup + j));
        }
        free(backup);
      } else {
        f1 = fopen(argv[i], "r");
        if (f1 == NULL) {
          puts("Error");
          system("pause");
          return 1;
        }
        while (!feof(f1)) {
          len = realloc(len, (n + 1) * sizeof(int)); // динамический массив длин строк
          len[n] = 0;
          while (((c = fgetc(f1)) != '\n') && (!feof(f1))) {
            len[n]++; // массив длин строк файла
          }
          n++; //число строк
        }
        //printf("Debug");
        fseek(f1, 0, SEEK_SET);
        strings = (char ** ) malloc(n * sizeof(char * )); // массив строк
        for (int j = 0; j < n - 1; j++) {
          str = (char * ) malloc((len[j] + 1) * sizeof(char)); //
          while (((c = fgetc(f1)) != '\n') && (!feof(f1))) {
            *(str + b) = c;
            b++;
          }
          *(str + b) = '\0';
          *(strings + j) = (char * ) malloc((len[j] + 1) * sizeof(char));
          strcpy(strings[j], str);
          free(str);
          b = 0;
        }
        main_sort(strings, len, n, rflag, fflag, bflag);
        for (int j = 0; j < n - 1; j++) {
          fprintf(output, "%s\n", *(strings + j));
          free( * (strings + j));
        }
        free(strings);
        fclose(f1);
        n = 0;
        free(len);
        len = NULL;
      }
    }
  }
  return 0;
}

int space_counter(char * string) {
  int k = 0;
  for (int i = 0; i < strlen(string); i++) {
    if (string[i] == ' ') {
      k++;
    } else {
      break;
    }
  }
  return k;
}

int my_strcmp(const char * s1,
  const char * s2, int sc1, int sc2) { // сравнение с -f
  if (( * (s1 + sc1) != '\0') && ( * (s1 + sc1) == * (s2 + sc2))) {
    return my_strcmp(s1 + 1, s2 + 1, sc1, sc2);
  } else {
    return (unsigned char) * (s1 + sc1) - (unsigned char) * (s2 + sc2);
  }
}
int my_strcmp_bf(const char * s1,
  const char * s2, int sc1, int sc2) { // сравнение с -b -f 
  if ((( * (s1 + sc1) != '\0') && ( * (s1 + sc1) == * (s2 + sc2))) || ( * (s1 + sc1) + 'A' - 'a' == * (s2 + sc2)) || ( * (s1 + sc1) == * (s2 + sc2) + 'A' - 'a')) {
    return my_strcmp_bf(s1 + 1, s2 + 1, sc1, sc2);
  } else {
    if ((( * (s1 + sc1) >= 'a') && ( * (s1 + sc1) <= 'z')) && ((( * (s2 + sc2) >= 'a') && ( * (s2 + sc2) <= 'z')))) {
      return ((unsigned char) * (s1 + sc1) + 'A' - 'a') - ((unsigned char) * (s2 + sc2) + 'A' - 'a');
    }
    if (( * (s1 + sc1) >= 'a') && ( * (s1 + sc1) <= 'z')) {
      return ((unsigned char) * (s1 + sc1) + 'A' - 'a') - ((unsigned char) * (s2 + sc2));
    }
    if (( * (s2 + sc2) >= 'a') && ( * (s2 + sc2) <= 'z')) {
      return ((unsigned char) * (s1 + sc1) - ((unsigned char) * (s2 + sc2) + 'A' - 'a'));
    }
    return (unsigned char) * (s1 + sc1) - (unsigned char) * (s2 + sc2);
  }
}

int my_strcmp_f(const char * s1,
  const char * s2) { // сравнение с -f
  if ((( * (s1) != '\0') && ( * (s1) == * (s2))) || ( * (s1) + 'A' - 'a' == * (s2)) || ( * (s1) == * (s2) + 'A' - 'a')) {
    return my_strcmp_f(s1 + 1, s2 + 1);
  } else {
    if ((( * (s1) >= 'a') && ( * (s1) <= 'z')) && ((( * (s2) >= 'a') && ( * (s2) <= 'z')))) {
      return ((unsigned char) * (s1) + 'A' - 'a') - ((unsigned char) * (s2) + 'A' - 'a');
    }
    if (( * (s1) >= 'a') && ( * (s1) <= 'z')) {
      return ((unsigned char) * (s1) + 'A' - 'a') - ((unsigned char) * (s2));
    }
    if (( * (s2) >= 'a') && ( * (s2) <= 'z')) {
      return ((unsigned char) * (s1) - ((unsigned char) * (s2) + 'A' - 'a'));
    }
    return (unsigned char) * (s1) - (unsigned char) * (s2);
  }
}
void main_sort(char * strings[], int len[], int n, int rflag, int fflag, int bflag) {
  int i, j, temp, scount1, scount2;
  char * str;
  if (rflag == 0) {
    if (bflag == 1) {
      if (fflag == 1) {
        for (i = 1; i < n - 1; i++) { // -b -f
          for (j = 0; j < n - i - 1; j++) {
            scount1 = space_counter(strings[j]);
            scount2 = space_counter(strings[j + 1]);
            if (my_strcmp_bf(strings[j], strings[j + 1], scount1, scount2) > 0) {
              str = (char * ) malloc((len[j] + 1) * sizeof(char));
              strcpy(str, strings[j]);
              free( * (strings + j));
              *(strings + j) = malloc((len[j + 1] + 1) * sizeof(char));
              strcpy(strings[j], strings[j + 1]);
              free( * (strings + j + 1));
              *(strings + j + 1) = malloc((len[j] + 1) * sizeof(char));
              strcpy(strings[j + 1], str);
              temp = len[j];
              len[j] = len[j + 1];
              len[j + 1] = temp;
              free(str);
            }
            scount1 = 0;
            scount2 = 0;
          }
        }
      } else { // -b
        for (i = 1; i < n - 1; i++) {
          for (j = 0; j < n - i - 1; j++) {
            scount1 = space_counter(strings[j]);
            scount2 = space_counter(strings[j + 1]);
            if (my_strcmp(strings[j], strings[j + 1], scount1, scount2) > 0) {
              str = (char * ) malloc((len[j] + 1) * sizeof(char));
              strcpy(str, strings[j]);
              free( * (strings + j));
              *(strings + j) = malloc((len[j + 1] + 1) * sizeof(char));
              strcpy(strings[j], strings[j + 1]);
              free( * (strings + j + 1));
              *(strings + j + 1) = malloc((len[j] + 1) * sizeof(char));
              strcpy(strings[j + 1], str);
              temp = len[j];
              len[j] = len[j + 1];
              len[j + 1] = temp;
              free(str);
            }
            scount1 = 0;
            scount2 = 0;
          }
        }
      }

    } else {
      if (fflag == 1) {
        for (i = 1; i < n - 1; i++) { // -f
          for (j = 0; j < n - i - 1; j++) {
            if (my_strcmp_f(strings[j], strings[j + 1]) > 0) {
              str = (char * ) malloc((len[j] + 1) * sizeof(char));
              strcpy(str, strings[j]);
              free( * (strings + j));
              *(strings + j) = malloc((len[j + 1] + 1) * sizeof(char));
              strcpy(strings[j], strings[j + 1]);
              free( * (strings + j + 1));
              *(strings + j + 1) = malloc((len[j] + 1) * sizeof(char));
              strcpy(strings[j + 1], str);
              temp = len[j];
              len[j] = len[j + 1];
              len[j + 1] = temp;
              free(str);
            }
          }
        }
      } else { // нет никаких пареметров из -b -f -r
        for (i = 1; i < n - 1; i++) {
          for (j = 0; j < n - i - 1; j++) {
            if (strcmp(strings[j], strings[j + 1]) > 0) {
              str = (char * ) malloc((len[j] + 1) * sizeof(char));
              strcpy(str, strings[j]);
              free( * (strings + j));
              *(strings + j) = malloc((len[j + 1] + 1) * sizeof(char));
              strcpy(strings[j], strings[j + 1]);
              free( * (strings + j + 1));
              *(strings + j + 1) = malloc((len[j] + 1) * sizeof(char));
              strcpy(strings[j + 1], str);
              temp = len[j];
              len[j] = len[j + 1];
              len[j + 1] = temp;
              free(str);
            }
          }
        }
      }
    }
  } else { // -r
    if (bflag == 1) {
      if (fflag == 1) {
        for (i = 1; i < n - 1; i++) { // -b -f -r
          for (j = 0; j < n - i - 1; j++) {
            scount1 = space_counter(strings[j]);
            scount2 = space_counter(strings[j + 1]);
            if (my_strcmp_bf(strings[j], strings[j + 1], scount1, scount2) <= 0) {
              str = (char * ) malloc((len[j] + 1) * sizeof(char));
              strcpy(str, strings[j]);
              free( * (strings + j));
              *(strings + j) = malloc((len[j + 1] + 1) * sizeof(char));
              strcpy(strings[j], strings[j + 1]);
              free( * (strings + j + 1));
              *(strings + j + 1) = malloc((len[j] + 1) * sizeof(char));
              strcpy(strings[j + 1], str);
              temp = len[j];
              len[j] = len[j + 1];
              len[j + 1] = temp;
              free(str);
            }
            scount1 = 0;
            scount2 = 0;
          }
        }
      } else { // -b -r
        for (i = 1; i < n - 1; i++) {
          for (j = 0; j < n - i - 1; j++) {
            scount1 = space_counter(strings[j]);
            scount2 = space_counter(strings[j + 1]);
            if (my_strcmp(strings[j], strings[j + 1], scount1, scount2) <= 0) {
              str = (char * ) malloc((len[j] + 1) * sizeof(char));
              strcpy(str, strings[j]);
              free( * (strings + j));
              *(strings + j) = malloc((len[j + 1] + 1) * sizeof(char));
              strcpy(strings[j], strings[j + 1]);
              free( * (strings + j + 1));
              *(strings + j + 1) = malloc((len[j] + 1) * sizeof(char));
              strcpy(strings[j + 1], str);
              temp = len[j];
              len[j] = len[j + 1];
              len[j + 1] = temp;
              free(str);
            }
            scount1 = 0;
            scount2 = 0;
          }
        }
      }

    } else {
      if (fflag == 1) {
        for (i = 1; i < n - 1; i++) { // -f -r
          for (j = 0; j < n - i - 1; j++) {
            if (my_strcmp_f(strings[j], strings[j + 1]) <= 0) {
              str = (char * ) malloc((len[j] + 1) * sizeof(char));
              strcpy(str, strings[j]);
              free( * (strings + j));
              *(strings + j) = malloc((len[j + 1] + 1) * sizeof(char));
              strcpy(strings[j], strings[j + 1]);
              free( * (strings + j + 1));
              *(strings + j + 1) = malloc((len[j] + 1) * sizeof(char));
              strcpy(strings[j + 1], str);
              temp = len[j];
              len[j] = len[j + 1];
              len[j + 1] = temp;
              free(str);
            }
          }
        }
      } else { // нет никаких пареметров кроме -r
        for (i = 1; i < n - 1; i++) {
          for (j = 0; j < n - i - 1; j++) {
            if (strcmp(strings[j], strings[j + 1]) <= 0) {
              str = (char * ) malloc((len[j] + 1) * sizeof(char));
              strcpy(str, strings[j]);
              free( * (strings + j));
              *(strings + j) = malloc((len[j + 1] + 1) * sizeof(char));
              strcpy(strings[j], strings[j + 1]);
              free( * (strings + j + 1));
              *(strings + j + 1) = malloc((len[j] + 1) * sizeof(char));
              strcpy(strings[j + 1], str);
              temp = len[j];
              len[j] = len[j + 1];
              len[j + 1] = temp;
              free(str);
            }
          }
        }
      }
    }
  }
}

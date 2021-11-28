#include <stdio.h>

#define mem 4
char * last_substr(char * str, const char * substr);
char * read_str(void);
int main() {
  char * s1, * s2, * out;
  printf("Input string:");
  s1 = read_str();
  printf("Input substring:");
  s2 = read_str();
  printf("Last substring:");
  out = last_substr(s1, s2);
  if (out != NULL) {
    int i = 0;
    while ( * (out + i) != '\0') {
      putc( * (out + i), stdout);
      i++;
    }
  } else {
    printf("NULL");
  };
  return 0;
}

char * last_substr(char * str, const char * substr) {
  char * p = NULL;
  char * temp;
  if (( * str == '\0') && ( * substr == '\0')) {
    return str;
  }
  while ( * str != '\0') {
    for (int i = 0; substr[i] != '\0'; i++) {
      if (substr[i] != str[i]) {
        temp = NULL;
        break;
      }
      temp = str;
    }
    if (temp != NULL) {
      p = temp;
    }
    str++;
  }

  return p;
}

char * read_str(void) {
  int len = 0;
  char c = getchar();
  char * s = (char * ) malloc(sizeof(char));
  while ((c != EOF) && (c != '\n')) {
    *(s + len) = c;
    len++;
    if (len % 4 == 1) {
      s = realloc(s, (len / 4 + 1) * mem);
    }
    c = getchar();
  }
  if (len % 4 == 0) {
    s = realloc(s, (len / 4) * mem + 1);
  }
  *(s + len) = '\0';
  return s;
}

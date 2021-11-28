#include <stdio.h>

#include <stdlib.h>

#include <string.h>

typedef struct Word {
    char c;
    struct Word * next;
}
Word;

typedef struct words_list {
    char * word;
    struct words_list * next;
}
words_list;

words_list * get_list();
void print_reverse(words_list * head);
void print_list(words_list * head);
void sort_list(words_list ** head);
void free_wlist(words_list * head);
void free_chlist(Word * head);
int list_len(Word * head);

int main() {
    words_list * list = NULL;
    list = get_list();
    print_reverse(list);
    printf("\n");
    sort_list( & list);
    print_list(list);
    free_wlist(list);
    return 0;
}

void addchar(Word ** head, char ac) {
    if ( * head == NULL) {
        * head = malloc(sizeof(Word));
        ( * head) -> c = ac;
        ( * head) -> next = NULL;
    } else {
        addchar( & ( * head) -> next, ac);
    }
}

void addword(words_list ** head, char * adw) {
    if ( * head == NULL) {
        * head = malloc(sizeof(words_list));
        ( * head) -> word = adw;
        ( * head) -> next = NULL;
    } else {
        addword( & ( * head) -> next, adw);
    }
}

char * list_to_word(Word * head) {
    int len = list_len(head);
    char * word = malloc((len + 1) * sizeof(char));
    for (int i = 0; i < len; i++) {
        word[i] = head -> c;
        head = head -> next;
    }
    word[len] = '\0';
    return word;
}

char * read_word(char * end) {
    Word * list = NULL;
    char * word = NULL;
    char c;
    c = getchar();
    while (c == ' ') {
        c = getchar();
    }
    if (c == '\n' || c == EOF) {
        * end = c;
        return NULL;
    }
    while ((c != '\n') && (c != EOF) && (c != ' ')) {
        addchar( & list, c);
        c = getchar();
    }
    * end = c;
    word = list_to_word(list);
    free_chlist(list);
    return word;
}

words_list * get_list() {
    words_list * list = NULL;
    char * word = NULL;
    char end;
    while ((end != '\n') && (end != EOF)) {
        word = read_word( & end);
        if (word != NULL) {
            addword( & list, word);
        }
    }
    return list;
}

void print_reverse(words_list * head) {
    if (head == NULL) {
        return;
    }
    if (head -> next == NULL) {
        printf("%s ", head -> word);
    } else {
        print_reverse(head -> next);
        printf("%s ", head -> word);
    }
}

void print_list(words_list * head) {
    if (head == NULL) {
        return;
    }
    words_list * p;
    p = head;
    do {
        printf("%s ", p -> word);
        p = p -> next;
    } while (p != NULL);
}

void insert_in_list(words_list ** head, words_list * wordPtr) {
    if ( * head == NULL) {
        wordPtr -> next = NULL;
        * head = wordPtr;
    } else {
        if (strcmp(( * head) -> word, wordPtr -> word) >= 0) {
            wordPtr -> next = * head;
            * head = wordPtr;
            return;
        }
        insert_in_list( & ( * head) -> next, wordPtr);
    }
}

void sort_list(words_list ** head) {
    words_list * wordPtr = NULL, * sorted_list = NULL;
    while ( * head != NULL) {
        wordPtr = * head;
        * head = ( * head) -> next;
        insert_in_list( & sorted_list, wordPtr);
    }
    * head = sorted_list;
}

void free_wlist(words_list * head) {
    words_list * p = NULL;
    while (head != NULL) {
        p = head;
        head = head -> next;
        free(p -> word);
        free(p);
    }
}

void free_chlist(Word * head) {
    Word * p = NULL;
    while (head != NULL) {
        p = head;
        head = head -> next;
        free(p);
    }
}

int list_len(Word * head) {
    int len = 0;
    while (head != NULL) {
        len++;
        head = head -> next;
    }
    return len;
}

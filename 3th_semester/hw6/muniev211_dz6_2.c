#include <stdio.h>

#include <stdlib.h>

#include <string.h>

typedef struct words_tree {
    char * word;
    struct words_tree * tleft;
    struct words_tree * tright;
}
words_tree;

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

words_tree * get_wtree(words_list * head);
words_tree * add_wtree(char * adwt, words_tree * wtree);
void tree_print(words_tree * wtree);
void del_tree(words_tree * tree);

void addchar(Word ** head, char ac);
void addword(words_list ** head, char * adw);
char * list_to_word(Word * head);
char * read_word(int * end);
words_list * get_list();
void free_wlist(words_list * head);
void free_chlist(Word * head);
void print_list(words_list * head);
int list_len(Word * head);

int main() {
    words_list * list = NULL;
    words_tree * my_tree = NULL;
    printf("Input list:\n");
    list = get_list();
    printf("Output list:\n");
    print_list(list);
    printf("\n");
    printf("Output tree:\n");
    my_tree = get_wtree(list);
    tree_print(my_tree);
    free_wlist(list);
    del_tree(my_tree);
}

words_tree * get_wtree(words_list * head) {
    words_tree * temp = NULL;
    if (head == NULL) return temp;
    words_list * p;
    p = head;
    do {
        temp = add_wtree(p -> word, temp);
        p = p -> next;
    } while (p != NULL);
    return temp;
}

words_tree * add_wtree(char * adwt, words_tree * wtree) {
    if (wtree == NULL) {
        wtree = malloc(sizeof(words_tree));
        wtree -> word = adwt;
        wtree -> tleft = NULL;
        wtree -> tright = NULL;
    } else {
        if (strcmp(wtree -> word, adwt) >= 0)
            wtree -> tleft = add_wtree(adwt, wtree -> tleft);
        else
            wtree -> tright = add_wtree(adwt, wtree -> tright);
    }
    return (wtree);
}

void tree_print(words_tree * wtree) {
    if (wtree != NULL) {
        tree_print(wtree -> tleft);
        printf("%s ", wtree -> word);
        tree_print(wtree -> tright);
    }
}

void del_tree(words_tree * tree) {
    if (tree != NULL) {
        del_tree(tree -> tleft);
        del_tree(tree -> tright);
        free(tree);
    }
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

char * read_word(int * end) {
    Word * list = NULL;
    char * word = NULL;
    int c;
    c = getchar();
    while (c == ' ') {
        c = getchar();
    }
    if ((c == '\n') || (c == EOF)) {
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
    int end = 0;
    while ((end != '\n') && (end != EOF)) {
        word = read_word( & end);
        if (word != NULL) {
            addword( & list, word);
        }
    }
    return list;
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

int list_len(Word * head) {
    int len = 0;
    while (head != NULL) {
        len++;
        head = head -> next;
    }
    return len;
}

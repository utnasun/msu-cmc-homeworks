#include <stdio.h>
#include <stdlib.h>

#define mem 4

// посимвольное получение динамической строки с пробелами c получением её реальной длины
char * get_string() {
    int len = 0; // изначально строка пуста
    char * s = (char * ) malloc(1 * sizeof(char)); // динамическая пустая строка
    char c = getchar(); // символ для чтения данных

    // читаем символы, пока не получим символ переноса строки (\n)
    while ((c != '\n') && (c != EOF)) {
        s[len++] = c; // заносим в строку новый символ
        // если реальный размер больше размера контейнера, то увеличим его размер
        if (len >= size_t) {
            s = (char * ) realloc(s, mem * sizeof(char)); // создаём новую строку с увеличенной ёмкостью  
        }

        c = getchar(); // считываем следующий символ          
    }

    s[ * len] = '\0'; // завершаем строку символом конца строки
    return s; // возвращаем указатель на считанную строку
}

int main() {
    char * s = get_string(); // считываем динамическую строку
    printf("You've wrote: '%s'", s); // выводим строку и её длину
    free(s); // освобождаем динамическую память
    return 0;
}

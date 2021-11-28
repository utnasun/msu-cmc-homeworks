#include <stdio.h>

void binary_notation(int n);
int count_one(int n);
void binary_sort(int ar[], int dim);
void arr_print(int ar[], int dim);

int main() {
    int n;
    printf("Enter input array's size = ");
    scanf("%d", & n);
    int ar[n];
    printf("Enter array:\n");
    for (int i = 0; i < n; i++) {
        printf("ar[%d] = ", i);
        scanf("%d", & ar[i]);
    }
    arr_print(ar, n);
    binary_sort(ar, n);
    printf("Sorted array:\n");
    arr_print(ar, n);
    return 0;
}
void arr_print(int ar[], int dim) {
    printf("Decimal array:\n");
    for (int i = 0; i < dim; i++) {
        printf("%d\n", ar[i]);
    }

    printf("Binary array:\n");
    for (int i = 0; i < dim; i++) {
        binary_notation(ar[i]);
    }
}
int count_one(int n) {
    int k = 0;
    while (n != 0) {
        k += n % 2;
        n /= 2;
    }
    return k;
}

void binary_sort(int ar[], int dim) {
    int temp, i, j, binary_arr[dim];
    for (i = 0; i < dim; i++) {
        binary_arr[i] = count_one(ar[i]);
    }
    for (i = 0; i < dim - 1; i++) {
        for (j = dim - 1; j > i; j--) {
            if ((binary_arr[j - 1] > binary_arr[j]) || ((binary_arr[j - 1] == binary_arr[j]) && ((ar[j - 1] < ar[j])))) {
                temp = binary_arr[j - 1];
                binary_arr[j - 1] = binary_arr[j];
                binary_arr[j] = temp;
                temp = ar[j - 1];
                ar[j - 1] = ar[j];
                ar[j] = temp;
            }
        }
    }
}

void binary_notation(int n) {
    int binary_n[8 * sizeof(int)], i = 0;
    for (int j = 0; j < 8 * sizeof(int); j++) {
        binary_n[j] = 0;
    }
    while (n != 0) {
        binary_n[i++] = n % 2;
        n /= 2;
    }
    if (i > 0) {
        i = 8 * sizeof(int);
        do {
            printf("%i", binary_n[--i]);
        } while (i > 0);
        printf("\n");
    } else
        printf("0");
}

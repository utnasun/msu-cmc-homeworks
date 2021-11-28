#include <stdio.h>

void spiral(int col, int lines, int Arr[][col]);
void matrix_print(int col, int lines, int Arr[][col]);

int main() {
    int m, n;
    printf("Enter matrix MxN size:\n");
    printf("M = ");
    scanf("%d", & m);
    printf("N = ");
    scanf("%d", & n);
    int ar[m][n];
    spiral(m, n, ar);
    matrix_print(m, n, ar);
    return 0;
}
void spiral(int lines, int col, int Arr[][col]) {
    int i = 0, j, k = 0, p = 1;
    while (i < lines * col) {
        k++;
        for (j = k - 1; j <= lines - k - 1; j++) // left columns
        {
            if (p > col * lines) {
                break;
            }
            Arr[j][k - 1] = p;
            p++;
            i++;
        }
        for (j = k - 1; j < col - k + 1; j++) // bottom lines
        {
            if (p > col * lines) {
                break;
            }
            Arr[lines - k][j] = p;
            p++;
            i++;
        }
        for (j = lines - k - 1; j > k - 1; j--) // right columns
        {
            if (p > col * lines) {
                break;
            }
            Arr[j][col - k] = p;
            p++;
            i++;
        }
        for (j = col - k; j > k - 1; j--) // top lines
        {
            if (p > col * lines) {
                break;
            }
            Arr[k - 1][j] = p;
            p++;
            i++;
        }
    }
}
void matrix_print(int lines, int col, int Arr[][col]) {
    for (int i = 0; i < lines; i++) {
        printf("\n");
        for (int j = 0; j < col; j++) {
            printf("%4d ", Arr[i][j]);
        }
    }
}

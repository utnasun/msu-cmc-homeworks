#include <stdio.h>

int palindrome(int n1, int n2);
int main() {
    int N1, N2, amount;
    do {
        printf("Enter N1 and N2:\n");
        scanf("%d%d", & N1, & N2);
        if (N1 > N2) {
            printf("Incorrect input N1, N2");
        } else {
            amount = palindrome(N1, N2);
            printf("Number of palindromes = %d\n", amount);
        }
    } while ((N1 != 0) && (N2 != 0));
    return 0;
}
int palindrome(int n1, int n2) {
    int reversed, temp, count = 0;
    for (int i = n1; i <= n2; i++) {
        temp = i;
        reversed = 0;
        while (temp > 0) {
            reversed = reversed * 10 + temp % 10;
            temp = temp / 10;
        }
        if (reversed == i) {
            count++;
            printf("%d\n", i);
        }
    }
    return count;
}

# include < stdio.h >

    void binary_notation(unsigned int n);
int cycle_shift(unsigned int x, int k, int direction);
int main() {
    unsigned int n;
    int k, dir;
    printf("Enter input number =");
    scanf("%u", & n);
    while (n != 0) {
        printf("Input number in binary notation=");
        binary_notation(n);
        printf("Enter the shift amount=");
        scanf("%d", & k);
        printf("Select the direction of the shift ( >0 - right , <=0 - left) =");
        scanf("%d", & dir);
        n = cycle_shift(n, k, dir);
        printf("Output number in binary notation=");
        binary_notation(n);
        printf("Enter input number = ");
        scanf("%u", & n);
    }
    return 0;
}
void binary_notation(unsigned int n) {
    int binary_n[8 * sizeof(unsigned int)], i = 0;
    for (int j = 0; j <= 8 * sizeof(unsigned int); j++) {
        binary_n[j] = 0;
    }
    while (n != 0) {
        binary_n[i++] = n % 2;
        n /= 2;
    }
    if (i > 0) {
        i = 8 * sizeof(unsigned int);
        do {
            printf("%i", binary_n[--i]);
        } while (i > 0);
        printf("\n");
    } else
        printf("0");
}
int cycle_shift(unsigned int x, int k, int direction) {
    unsigned int temp = x;
    if (direction > 0) {
        x >>= k;
        temp <<= (8 * sizeof(unsigned int) - k);
        x |= temp;
    } else {
        x <<= k;
        temp >>= (8 * sizeof(unsigned int) - k);
        x |= temp;
    }
    return x;
}

#include <stdio.h>

#include "mulprec.h"

void print_elapsed_time(clock_t begin, clock_t end) {
    float elapsed = (float)(end - begin) / CLOCKS_PER_SEC;
    printf("Time    : %1.7f sec\n", elapsed);
}

int main(int argc, char **argv) {
    struct NUMBER a;

    clearByZero(&a);

    printf("progress: ");
    clock_t begin = clock();
    newton_no_sqrt(&a);
    clock_t end = clock();

    printf("========== result ==========\n");
    dispNumberZeroSuppress(&a);
    printf("\n");
    print_elapsed_time(begin, end);
    printf("Matched : %d\n", checkPi(&a));

    return 0;
}

#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel for
    for (int i = 0; i < 4; i++) {
        printf("Hello from iteration %d\n", i);
    }
    return 0;
}

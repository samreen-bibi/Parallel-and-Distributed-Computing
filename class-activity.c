#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        int id = omp_get_thread_num();  //each core tells its own id
        printf("Hello from core %d\n", id);
    }
    return 0;
}

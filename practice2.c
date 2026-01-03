#include <stdio.h>
#include <omp.h>

int main() {
    omp_set_num_threads(4);  
    int sum = 0;

    #pragma omp parallel
    {
        #pragma omp for reduction(+:sum)
        for (int i = 0; i < 1000; i++) {
            sum += i;
        }
        printf("Sum of first 1000 numbers = %d\n", sum);
    }

    // printf("Sum of first 1000 numbers = %d\n", sum);
    return 0;
}



// #include <stdio.h>
// #include <omp.h>

// int main() {
//     omp_set_num_threads(4);
//     int sum = 0;
//     int local_sum[4] = {0};   // har thread ka apna sum

//     #pragma omp parallel
//     {
//         int id = omp_get_thread_num();

//         for (int i = id + 0; i < 1000; i += 4) {
//             local_sum[id] += i;
//         }
//     }

//     for (int i = 0; i < 4; i++) {
//         sum += local_sum[i];
//     }

//     printf("Sum of first 1000 numbers = %d\n", sum);
//     return 0;
// }

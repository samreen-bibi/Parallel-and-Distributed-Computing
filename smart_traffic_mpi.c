#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);               // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Process ID
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Total processes

    srand(time(NULL) + rank);  // Different random seed for each node

    if (rank == 0) {
        printf("\nSMART TRAFFIC FLOW MANAGEMENT SYSTEM - CONTROLLER MODE\n");
        printf("------------------------------------------------------------\n");
        printf("Total Nodes (including Controller): %d\n", size);
        printf("Waiting for traffic updates from all signals...\n\n");

        for (int cycle = 1; cycle <= 3; cycle++) { // run 3 update cycles
            int cars;
            double green_times[size];
            int traffic_data[size];

            // Receive traffic count from all worker nodes
            for (int i = 1; i < size; i++) {
                MPI_Recv(&cars, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                traffic_data[i] = cars;
                printf("Cycle %d | Received from Node %d: %d vehicles waiting\n", cycle, i, cars);
            }

            // Compute green light durations
            for (int i = 1; i < size; i++) {
                green_times[i] = 10 + 0.5 * traffic_data[i];
            }

            printf("\nCalculating optimal signal timings...\n");
            for (int i = 1; i < size; i++) {
                if (traffic_data[i] > 40)
                    printf(" Node %d: Heavy Traffic! %d cars → Green = %.1fs\n", i, traffic_data[i], green_times[i]);
                else if (traffic_data[i] > 20)
                    printf(" Node %d: Moderate Traffic (%d cars) → Green = %.1fs\n", i, traffic_data[i], green_times[i]);
                else
                    printf(" Node %d: Light Traffic (%d cars) → Green = %.1fs\n", i, traffic_data[i], green_times[i]);
            }

            // Send updated green times to all nodes
            for (int i = 1; i < size; i++) {
                MPI_Send(&green_times[i], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            }

            printf("\nCycle %d complete. Timings updated for all nodes.\n", cycle);
            printf("------------------------------------------------------------\n\n");
            sleep(3); // wait before next update
        }
    } 
    else {
        char signal_name[20];
        sprintf(signal_name, "Signal-%d", rank);

        for (int cycle = 1; cycle <= 3; cycle++) {
            int cars_waiting = rand() % 51;  // random 0–50 cars
            MPI_Send(&cars_waiting, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

            double green_time;
            MPI_Recv(&green_time, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            printf("%s | Cycle %d | Cars Waiting: %2d | Green Time: %.1fs\n",
                   signal_name, cycle, cars_waiting, green_time);
            sleep(3);
        }
    }

    MPI_Finalize();
    return 0;
}

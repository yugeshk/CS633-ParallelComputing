#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char *argv[]){
    
    //Initialize variables
    int data_size[5] = {128, 1024, 65536, 1048576, 4194304};
    double start, end;
    int i = atoi(argv[3]);

    //Initialize MPI
    MPI_Init(&argc, &argv);

    //Get number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    //Get the rank of the process
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Now for the parallel computing part
    
    if (my_rank){ //code for all the sending processes
        //Start blocking call.
        start = MPI_Wtime();
        for(int j=0;j<5;j++){
            char *data_send = (char *)(calloc(data_size[i], sizeof(char)));
            MPI_Gather(data_send, data_size[i], MPI_UNSIGNED_CHAR, NULL, NULL, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
            free(data_send);
        }
        end = MPI_Wtime();
        printf("Sending blocking call time %f\n", (end-start)/5);

        //start non-blocking call
        double start = MPI_Wtime();
        for(int j=0;j<5;j++){
            char *data_send = (char *)(calloc(data_size[i], sizeof(char)));
            MPI_Igather(data_send, data_size[i], MPI_UNSIGNED_CHAR, NULL, NULL, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD, NULL);
            free(data_send);
        }
        end = MPI_Wtime();
        printf("Sending non-blocking call time %f\n", (end-start)/5);

    }
    
    if (my_rank == 0){  //code for process 0 - the receiving process
        start = MPI_Wtime();
        for(int j=0;j<5;j++){
            char *data_recv = (char *)(calloc((world_size-1)*data_size[i], sizeof(char)));
            MPI_Gather(NULL, NULL, MPI_UNSIGNED_CHAR, data_recv, data_size[i], MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
            free(data_recv);
        }
        end = MPI_Wtime();
        printf("Receiving blocking call %f\n", (end-start)/5);

        start = MPI_Wtime();
        MPI_Request request;
        MPI_Status status;
        for(int j=0;j<5;j++){
            char *data_recv = (char *)(calloc((world_size-1)*data_size[i], sizeof(char)));
            MPI_Igather(NULL, NULL, MPI_UNSIGNED_CHAR, data_recv, data_size[i], MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD, &request);
            free(data_recv);
        }
        MPI_Wait(&request, &status);
        end = MPI_Wtime();
        printf("Receiving blocking call %f\n", (end-start)/5);

    }

    MPI_Finalize();
    return 0;


}

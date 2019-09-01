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
    char *data_send = (char *)(calloc(data_size[i], sizeof(char)));
    char *data_recv = (char *)(calloc((world_size)*data_size[i], sizeof(char)));
    MPI_Request request;
    MPI_Status status;

    if (my_rank){ //code for all the sending processes
        //Start blocking call.
        start = MPI_Wtime();
        for(int j=0;j<5;j++){
            MPI_Gather(data_send, data_size[i], MPI_UNSIGNED_CHAR, data_recv, data_size[i], MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
        }
        end = MPI_Wtime();
        printf("Sending blocking call time %f\n", (end-start)/5);

        //start non-blocking call
        start = MPI_Wtime();
        for(int j=0;j<5;j++){
            MPI_Igather(data_send, data_size[i], MPI_UNSIGNED_CHAR, data_recv, data_size[i], MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD, &request);
        }
        end = MPI_Wtime();
        printf("Sending non-blocking call time %f\n", (end-start)/5);
        MPI_Wait(&request, &status);

    }
    
    if (my_rank == 0){  //code for process 0 - the receiving process
        start = MPI_Wtime();
        for(int j=0;j<5;j++){
            MPI_Gather(data_send, data_size[i], MPI_UNSIGNED_CHAR, data_recv, data_size[i], MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
        }
        end = MPI_Wtime();
        printf("Receiving blocking call %f\n", (end-start)/5);

        start = MPI_Wtime();
        for(int j=0;j<5;j++){
            MPI_Igather(data_send, data_size[i], MPI_UNSIGNED_CHAR, data_recv, data_size[i], MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD, &request);
        }
        end = MPI_Wtime();
        printf("Receiving non-blocking call %f\n", (end-start)/5);

    }

    MPI_Finalize();
    free(data_recv);
    free(data_send);
    return 0;


}

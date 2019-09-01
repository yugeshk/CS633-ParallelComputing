#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char *argv[]){              

    //Initialize variables
    int data_size[5] = {128, 1024, 65536, 1048576, 4194304};
    int i = atoi(argv[3]);

    //Initialize the MPI environment
    MPI_Init(&argc, &argv);

    //Get number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    //Get the rank of the process
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Now for the parallel computing part
    //for(int i=0;i<5;i++){
    if( my_rank ==0 ){ // code for process 0 - the sending process
        double start = MPI_Wtime();
        for(int j=0;j<5;j++){
            char *data_send = (char *)(calloc(data_size[i], sizeof(char)));
            MPI_Send(data_send, data_size[i], MPI_UNSIGNED_CHAR, 1, 99, MPI_COMM_WORLD);
            free(data_send);
        }
        double end = MPI_Wtime();
        printf("Sending process time %f\n", (end-start)/5);
    }

    else if( my_rank == 1 ){
        double start = MPI_Wtime();
        for(int j=0;j<5;j++){
            char *data_recv = (char *)(calloc(data_size[i], sizeof(char)));
            int count;
            MPI_Status status;
            MPI_Recv(data_recv, data_size[i], MPI_UNSIGNED_CHAR, 0, 99, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_UNSIGNED_CHAR, &count);
            /* printf("Received %d bytes of data\n", count); */
            /* fflush(stdout); */
            free(data_recv);
        }
        double end = MPI_Wtime();
        printf("Receiving process time %f\n", (end-start)/5);
    }
    //}

    MPI_Finalize();
    return 0;

}






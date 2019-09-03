#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char *argv[]){              

    //Initialize variables
    int data_size[5] = {128, 1024, 65536, 1048576, 4194304};
    int i = atoi(argv[1]);
    double start, end;

    //Initialize the MPI environment
    MPI_Init(&argc, &argv);

    //Get number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    //Get the rank of the process
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Now for the parallel computing part
    if( my_rank ==0 ){ // code for process 0 - the sending process
        char *data_send = (char *)(calloc(data_size[i], sizeof(char)));
        for(int j=0;j<5;j++){  //No. of experiments
            start = MPI_Wtime();
            for(int p=0;p<100;p++){  //Iterations per experiment
                MPI_Send(data_send, data_size[i], MPI_UNSIGNED_CHAR, 1, 99, MPI_COMM_WORLD);
            }
        end = MPI_Wtime();
        printf("Sending%d process time %f\n", j, (end-start));
        }
        free(data_send);
    }

    else if( my_rank == 1 ){
        char *data_recv = (char *)(calloc(data_size[i], sizeof(char)));
        for(int j=0;j<5;j++){
            start = MPI_Wtime();
            for(int p=0;p<100;p++){
                int count;
                MPI_Status status;
                MPI_Recv(data_recv, data_size[i], MPI_UNSIGNED_CHAR, 0, 99, MPI_COMM_WORLD, &status);
                MPI_Get_count(&status, MPI_UNSIGNED_CHAR, &count);
            }
            end = MPI_Wtime();
            printf("Receiving%d process time %f\n", j, (end-start));
        }
        free(data_recv);
    }
    //}

    MPI_Finalize();
    return 0;

}






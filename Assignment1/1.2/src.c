#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char *argv[]){
    
    //Initialize variables
    int data_size[5] = {128, 1024, 65536, 1048576, 4194304};
    double start, end;
    int count;
    int i = atoi(argv[1]);

    //Initialize MPI
    MPI_Init(&argc, &argv);

    //Get number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    //Get the rank of the process
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Now for the parallel computing part
    MPI_Request request;
    MPI_Status status;
    float max_time;
    float time_taken;

    if (my_rank){ //code for all the sending processes
        
        //Start blocking call.
        char *data_send = (char *)(calloc(data_size[i], sizeof(char)));
        for(int j=0;j<5;j++){
        start = MPI_Wtime();
            for(int l=0;l<100;l++){
                MPI_Send(data_send, data_size[i], MPI_UNSIGNED_CHAR, 0, 99, MPI_COMM_WORLD);
            }
        end = MPI_Wtime();
        time_taken = (end-start)/5;
        MPI_Reduce(&time_taken, &max_time, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);
        }

        //start non-blocking call
        for(int j=0;j<5;j++){
        start = MPI_Wtime();
            for(int l=0;l<100;l++){
                MPI_Isend(data_send, data_size[i], MPI_UNSIGNED_CHAR, 0, 99, MPI_COMM_WORLD, &request);
                MPI_Wait(&request, MPI_STATUS_IGNORE);
            }
        end = MPI_Wtime();
        time_taken = (end-start)/5;
        MPI_Reduce(&time_taken, &max_time, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);
        }
        free(data_send);

    }
    
    else if (my_rank == 0){  //code for process 0 - the receiving process

        //process the blocking calls
        char *data_recv = (char *)(calloc(data_size[i], sizeof(char)));
        for(int j=0;j<5;j++){
            start = MPI_Wtime();
            for(int l=0;l<100;l++){
                for(int p=1;p<world_size;p++){
                MPI_Recv(data_recv, data_size[i], MPI_UNSIGNED_CHAR, p, 99, MPI_COMM_WORLD, &status);
                MPI_Get_count(&status, MPI_UNSIGNED_CHAR, &count);
                }
            }
            end = MPI_Wtime();
            time_taken = (end-start)/5;
            MPI_Reduce(&time_taken, &max_time, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);
            printf("Blocking call1 time %f\n", max_time);
        }

        //process the non-blocking calls
        char **data_irecv = (char **)(malloc((world_size-1)*sizeof(char *)));
        MPI_Request r[world_size-1];
        for(int j=0;j<5;j++){
            start = MPI_Wtime();
            for(int l=0;l<100;l++){
                for(int p=0;p<world_size-1;p++){
                    data_irecv[p] = (char *)(malloc(data_size[i]*sizeof(char)));
                    MPI_Irecv(data_irecv[p], data_size[i], MPI_UNSIGNED_CHAR, p+1, 99, MPI_COMM_WORLD, &r[p]);
                }
                MPI_Waitall(world_size-1, r, MPI_STATUSES_IGNORE);
            }
            end = MPI_Wtime();
            time_taken = (end-start)/5;
            MPI_Reduce(&time_taken, &max_time, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);
            printf("Non-blocking call2 time %f\n", max_time);
        }

        free(data_recv);
        free(data_irecv);

    }

    MPI_Finalize();
    return 0;


}

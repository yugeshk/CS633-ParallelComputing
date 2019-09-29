#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char *argv[]){

    //Initialize variables
    int data_size[6] = {65536, 262144, 524288, 2097152, 4194304, 65536};

    double start, end;

    //Initialize the MPI environment
    MPI_Init(&argc, &argv);

    //Get number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    //Get the rank of the process
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int recv_ptr, send_ptr;
    //Now for the parellel computing part
    for(int i=0;i<6;i++){
        char *data_send = (char*)(calloc(data_size[i], sizeof(char)));
        char *data_recv = (char *)(calloc(data_size[i]/world_size, sizeof(char)));
        char *recv_buff = (char *)(calloc(data_size[i], sizeof(char)));
        if(my_rank == 0){
            for(int j=0;j<data_size[i];j+=4){
                data_send[j] = (float)(rand());
            }

            //do the regular Bcast
            start = MPI_Wtime();
            MPI_Bcast(data_send, data_size[i], MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
            end = MPI_Wtime();

            double max_time = end-start;
            MPI_Reduce(MPI_IN_PLACE, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); 
            if(i!=5) {printf("%f ", max_time);}

            MPI_Barrier(MPI_COMM_WORLD);

            //Do the new Bcast

            start = MPI_Wtime();
            MPI_Scatter(data_send, data_size[i]/world_size, MPI_UNSIGNED_CHAR, data_recv, data_size[i]/world_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
            //Do ring Allgather
            //copy own scatter buffer to recv_buff
            for(int j=0;j<data_size[i]/world_size;j++){
                recv_buff[data_size[i]*my_rank/world_size+j] = data_recv[j];
            }
            MPI_Status status;

            for(int iter = 0; iter < world_size -1 ; iter++){
                recv_ptr = data_size[i]*(my_rank - iter - 1)/world_size;
                recv_ptr = recv_ptr < 0 ? ( world_size + my_rank - iter - 1 ) * data_size[i]/world_size : recv_ptr;

                send_ptr = recv_ptr + data_size[i]/world_size; 

                MPI_Request request;
                MPI_Isend(recv_buff+send_ptr, data_size[i]/world_size, MPI_UNSIGNED_CHAR, my_rank+1, 99, MPI_COMM_WORLD, &request);
                MPI_Irecv(recv_buff+recv_ptr, data_size[i]/world_size, MPI_UNSIGNED_CHAR, world_size-1, 99, MPI_COMM_WORLD, &request);
                MPI_Wait(&request, &status);
            }

            end = MPI_Wtime();
            max_time = end-start;
            MPI_Reduce(MPI_IN_PLACE, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); 
            if(i!=5) {printf("%f\n", max_time);}

        }
        else{
            //do the regular Bcast
            start = MPI_Wtime();
            MPI_Bcast(data_send, data_size[i], MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
            end = MPI_Wtime();

            double max_time = end-start;
            MPI_Reduce(&max_time, NULL, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

            MPI_Barrier(MPI_COMM_WORLD);

            //Do the new Bcast

            start = MPI_Wtime();
            MPI_Scatter(data_send, data_size[i]/world_size, MPI_UNSIGNED_CHAR, data_recv, data_size[i]/world_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
            //Do ring Allgather
            //copy own scatter buffer to recv_buff
            for(int j=0;j<data_size[i]/world_size;j++){
                recv_buff[data_size[i]*my_rank/world_size+j] = data_recv[j];
            }
            MPI_Status status;

            for(int iter = 0; iter < world_size -1 ; iter++){
                recv_ptr = data_size[i]*(my_rank - iter - 1)/world_size;
                recv_ptr = recv_ptr < 0 ? ( world_size + my_rank - iter - 1 ) * data_size[i]/world_size : recv_ptr;

                send_ptr = recv_ptr + data_size[i]/world_size; 

                MPI_Request request;
                MPI_Isend(recv_buff+send_ptr, data_size[i]/world_size, MPI_UNSIGNED_CHAR, (my_rank+1)%world_size, 99, MPI_COMM_WORLD, &request);
                MPI_Irecv(recv_buff+recv_ptr, data_size[i]/world_size, MPI_UNSIGNED_CHAR, my_rank-1, 99, MPI_COMM_WORLD, &request);
                MPI_Wait(&request, &status);

            }

            end = MPI_Wtime();
            max_time = end-start;
            MPI_Reduce(&max_time, NULL, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); 
        }
    }
}

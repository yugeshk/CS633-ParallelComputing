#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main(int argc, char *argv[]){

    //Initialize variables
    int data_size[3] = {65536, 524288, 2097152};

    double start, end;

    //Initialize the MPI environment
    MPI_Init(&argc, &argv);
    int i = (atoi)(argv[1]);

    //Get number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    //Get the rank of the process
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    //Now for the parallel computing part

    //generate pairs for communication
    char *data_send = (char *)(calloc(data_size[i], sizeof(char)));
    char *data_recv = (char *)(calloc(data_size[i], sizeof(char)));
    MPI_Status status;
    int count;
    double times[30][30];
    for(int t=0;t<30;t++){
        for(int t1=0;t1<30;t1++){
            times[t][t1]=0;
        }
    }

    //do this for the given data size for all node distances

    for(int dist=1;dist<=29; dist++){
        int limit=0;
        int pairs[15][2];
        int curr_node = 0;
        int skip_at = curr_node+dist;
        /* printf("Generating paris\n"); */
        while(curr_node+dist<=29){
            pairs[limit][0]=curr_node;
            pairs[limit][1]=curr_node+dist;
            limit++;
            curr_node++;
            if(curr_node==skip_at){
                curr_node+=dist;
                skip_at = curr_node+dist;
            }
        }
        //pairs generated upto limit in pairs[][]
        for(int l=0;l<limit;l++){
            /* printf("Trying comm with %d %d %d\n", l, pairs[l][0], pairs[l][1]); */
            if(my_rank == pairs[l][0]){
                start = MPI_Wtime();
                MPI_Send(data_send, data_size[i], MPI_UNSIGNED_CHAR, pairs[l][1], 99, MPI_COMM_WORLD);
                end = MPI_Wtime();
                times[pairs[l][0]][pairs[l][1]] = end-start;

                start = MPI_Wtime();
                MPI_Recv(data_recv, data_size[i], MPI_UNSIGNED_CHAR, pairs[l][1], 99, MPI_COMM_WORLD, &status);
                end = MPI_Wtime();
                times[pairs[l][1]][pairs[l][0]] = end-start;
            }
            else if(my_rank == pairs[l][1]){
                start = MPI_Wtime();
                MPI_Recv(data_recv, data_size[i], MPI_UNSIGNED_CHAR, pairs[l][0], 99, MPI_COMM_WORLD, &status);
                end = MPI_Wtime();
                times[pairs[l][0]][pairs[l][1]] = end-start;

                start = MPI_Wtime();
                MPI_Send(data_send, data_size[i], MPI_UNSIGNED_CHAR, pairs[l][0], 99, MPI_COMM_WORLD);
                end = MPI_Wtime();
                times[pairs[l][1]][pairs[l][0]] = end-start;
            }
        }

        //generate the next list of pairs for same dist
        int pairs_new[limit][2];
        for(int l=0;l<limit;l++){
            pairs_new[l][0]=pairs[l][1];
            pairs_new[l][1]=pairs[l][1]+dist;
        }

        //pairs generated upto limit in pairs_new[][]
        for(int l=0;l<limit;l++){
            /* printf("Trying comm with %d %d %d\n", l, pairs[l][0], pairs[l][1]); */
            if(pairs_new[l][1] > 29){
                continue;
            }
            if(my_rank == pairs_new[l][0]){
                start = MPI_Wtime();
                MPI_Send(data_send, data_size[i], MPI_UNSIGNED_CHAR, pairs_new[l][1], 99, MPI_COMM_WORLD);
                end = MPI_Wtime();
                times[pairs_new[l][0]][pairs_new[l][1]] = end-start;

                start = MPI_Wtime();
                MPI_Recv(data_recv, data_size[i], MPI_UNSIGNED_CHAR, pairs_new[l][1], 99, MPI_COMM_WORLD, &status);
                end = MPI_Wtime();
                times[pairs_new[l][1]][pairs_new[l][0]] = end-start;
            }
            else if(my_rank == pairs_new[l][1]){
                start = MPI_Wtime();
                MPI_Recv(data_recv, data_size[i], MPI_UNSIGNED_CHAR, pairs_new[l][0], 99, MPI_COMM_WORLD, &status);
                end = MPI_Wtime();
                times[pairs_new[l][0]][pairs_new[l][1]] = end-start;

                start = MPI_Wtime();
                MPI_Send(data_send, data_size[i], MPI_UNSIGNED_CHAR, pairs_new[l][0], 99, MPI_COMM_WORLD);
                end = MPI_Wtime();
                times[pairs_new[l][1]][pairs_new[l][0]] = end-start;
            }
        }
    }
        
    //Communication with self
    MPI_Request request;
    start = MPI_Wtime();
    MPI_Isend(data_send, data_size[i], MPI_UNSIGNED_CHAR, my_rank, 99, MPI_COMM_WORLD, &request);
    MPI_Irecv(data_recv, data_size[i], MPI_UNSIGNED_CHAR, my_rank, 99, MPI_COMM_WORLD, &request);
    MPI_Wait(&request, &status);
    end = MPI_Wtime();
    times[my_rank][my_rank] = end-start;

    if(my_rank == 0){
        MPI_Reduce(MPI_IN_PLACE, times, 900, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    }
    else{
        MPI_Reduce(times, NULL, 900, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    }


    //Print the reduced times at root.
    if(my_rank == 0){
        for(int a=0;a<30;a++){
            for(int b=0;b<30;b++){
                printf("%f ",times[a][b]);
            }
            printf("\n");
        }
    printf("\n");
    }
}

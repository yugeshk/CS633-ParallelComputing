#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <mpi.h>
#include<float.h>

#define max_iterations 400
#define threshold 0.001
#define num_Dimensions 3

typedef struct {
  double *dataset;
  unsigned int *members;
  int leading_dim;
  int secondary_dim;
} data_struct;

double distance_points(double *v1, double *v2, int length){
	double dist = 0;

    int i=0;
	for (i = 0; i < length; i++){
		dist += (v1[i] - v2[i]) * (v1[i] - v2[i]);
	}

	return(dist);
}


void kmeans_process(data_struct *data_in, data_struct *clusters, double *newCentroids, double* SumOfDist)
{
	double tmp_dist = 0;
	int tmp_index = 0;
	double min_dist = 0;
	double *dataset = data_in->dataset;
	double *centroids = clusters->dataset;
	unsigned int *Index = data_in->members;
	unsigned int *cluster_size = clusters->members;

    int i=0, j=0, k=0;
	for (i = 0; i < clusters->secondary_dim; i++){
		cluster_size[i] = 0;
	}

	for (i = 0; i < data_in->secondary_dim; i++){
		tmp_dist = 0;
		tmp_index = 0;
		min_dist = FLT_MAX;

		/*find nearest center*/
		for (k = 0; k < clusters->secondary_dim; k++){
			tmp_dist = distance_points(dataset + i * data_in->leading_dim, centroids + k * clusters->leading_dim, data_in->leading_dim);

			if (tmp_dist<min_dist){
				min_dist = tmp_dist;
				tmp_index = k;
			}
		}

		Index[i] = tmp_index;
		SumOfDist[0] += min_dist;
		cluster_size[tmp_index]++;
		for (j = 0; j < data_in->leading_dim; j++)
		{
			newCentroids[tmp_index * clusters->leading_dim + j] += dataset[i * data_in->leading_dim + j];
		}
	}
}

void read_dataset_file(data_struct *data_in, int world_size, int my_rank, char *inp_file){
    int dims = data_in->leading_dim;
    int pts = data_in->secondary_dim;
    double *t_dataset = data_in->dataset;
    unsigned int *t_Index = data_in->members;
    MPI_File cFile;
    MPI_Status status;

    MPI_File_open(MPI_COMM_WORLD, inp_file, MPI_MODE_RDONLY, MPI_INFO_NULL, &cFile);
    
    double *buf = (double *)(malloc(pts*4*sizeof(double)));
    MPI_File_read(cFile, buf, pts*4, MPI_DOUBLE, &status);

    int i=0, j=0;
    for(i=0;i<pts;i++){ //Read all points for all processes
        t_Index[i]=0;
        for(j=0;j<dims;j++){
            t_dataset[i*dims + j] = buf[i*(dims+1) + j + 1];
        }
    }   
}


void initialize_clusters(data_struct *data_in, data_struct *cluster_in){
	int pick = 0;
	int dims = cluster_in->leading_dim;
	int k = cluster_in->secondary_dim;
	int pts = data_in->secondary_dim;
	double *t_Centroids = cluster_in->dataset;
	double *t_dataset = data_in->dataset;
	unsigned int *tmp_Sizes = data_in->members;

	int step = pts / k;

	/*randomly pick initial cluster centers*/
    int i=0, j=0;
	for (i = 0; i < k; i++){
		for (j = 0; j < dims; j++){
      		t_Centroids[i * dims + j] = t_dataset[pick * dims + j];
    	}
		pick += step; 
	}	
}

void print_centroids(double* centroids){
    printf("Printing Centroids \n");
    int i=0;
    for(i=0;i<17;i++){
       printf("C%d: %lf %lf %lf\n", i, centroids[3*i], centroids[3*i+1], centroids[3*i+2]); 
    }
}

void save_data_to_file(int time_set, int num_clusters, data_struct* cluster_data, int world_size, char *out_file, double pre_processing_time, double processing_time){

    FILE *fptr;
    FILE *fptr1, *fptr2;
    fptr=fopen(out_file, "a+");
    fptr1=fopen("pre_processing_time.txt", "a+");
    fptr2=fopen("processing_time.txt", "a+");
    double *centroids = cluster_data->dataset;

    //fprintf(fptr, "Number of processes: %d\n", world_size);
    fprintf(fptr, "T%d: %d, ", time_set, num_clusters);
    fprintf(fptr, "<%d, %lf, %lf, %lf>", cluster_data->members[0], centroids[0]/cluster_data->members[0], centroids[1]/cluster_data->members[0], centroids[2]/cluster_data->members[0]);
    int i;
    for(i=1 ; i < num_clusters ; i++){
        fprintf(fptr, " ,<%d, %lf, %lf, %lf>", cluster_data->members[i], centroids[3*i]/cluster_data->members[i], centroids[3*i+1]/cluster_data->members[i], centroids[3*i+2]/cluster_data->members[i]);
    }
    fprintf(fptr, "\n");
    fprintf(fptr1, "%lf\n", pre_processing_time);
    fprintf(fptr2, "%lf\n", processing_time);
    
    fclose(fptr);
    fclose(fptr1);
    fclose(fptr2);
}

void clean(data_struct* data1){
  free(data1->dataset);
  free(data1->members);
}

void print_data_points(data_struct* data_in){
    int dims = data_in->leading_dim;
    int pts  = data_in->secondary_dim;
    printf("Printing read data points\n");
    int i,j;
    for(i=0;i<pts;i++){
        for(j=0;j<dims;j++){
            printf("P: %lf ", data_in->dataset[i*dims + j]);
        }
	printf("\n");
    }       
}


int main(int argc, char **argv){ 
    double start, end;
	
    //Loop variables
    int i,j,k,iter;

    //Initialize the MPI environment
	MPI_Init(&argc, &argv);

    //Get number of processes
    int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    //Get the rank of the process
    int my_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	int num_points = atoi(argv[1]);  //Compute this in the bash script
    char *inp_file = argv[2]; //name of the input data file
	int num_clusters = atoi(argv[3]); // This is passed as arguement for trial in bash script
    int time_set = atoi(argv[4]); //The number T1, T2, etc
    char *out_file = argv[5]; //Name of the output file

	data_struct data_in;
	data_struct clusters;

	/*=======Memory Allocation=========*/
	data_in.leading_dim = num_Dimensions;
	data_in.secondary_dim = num_points;
	data_in.dataset = (double*)malloc(num_points * num_Dimensions * sizeof(double));
	data_in.members = (unsigned int*)malloc(num_points * sizeof(unsigned int));

	clusters.leading_dim = num_Dimensions;
	clusters.secondary_dim = num_clusters;
	clusters.dataset = (double*)malloc(num_clusters * num_Dimensions * sizeof(double));
	clusters.members = (unsigned int*)malloc(num_clusters * sizeof(unsigned int)); 


	/*=============initialize ==========*/                      
    // The root rank reads complete data all other ranks read their own chunk
	start = MPI_Wtime();
    read_dataset_file(&data_in, world_size, my_rank, inp_file);

	if (my_rank == 0){
		initialize_clusters(&data_in, &clusters);
 	}
	
	MPI_Barrier(MPI_COMM_WORLD);

	// Broadcasts a message from the process with my_rank "root" to all other processes of the communicator 	
	MPI_Bcast(clusters.dataset, num_clusters*num_Dimensions, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	// Create a struct for each process. Each process has its own dataset (a subdata of the initial one)
	data_struct p_data;

	p_data.leading_dim = num_Dimensions; 

	// The elements associated with each process are n/p
	int n_split =  (int)(num_points / world_size);
	int n_temp = num_points - (world_size-1)*n_split;
	if (my_rank != world_size-1){
		p_data.secondary_dim = n_split; //Number of points in process
		p_data.dataset = (double*)malloc(n_split * num_Dimensions * sizeof(double));
		p_data.members = (unsigned int*)malloc(n_split * sizeof(unsigned int)); 
	}
	else{
		p_data.secondary_dim = n_temp; //Number of points in process
		p_data.dataset = (double*)malloc(p_data.secondary_dim * num_Dimensions * sizeof(double));
		p_data.members = (unsigned int*)malloc(p_data.secondary_dim * sizeof(unsigned int)); 	
	}

	// Here, every process creates a sub-dataset of its elements
	if(my_rank != world_size-1){
        for(i = 0; i < p_data.secondary_dim * p_data.leading_dim; i++){ 
            p_data.dataset[i] = data_in.dataset[my_rank * p_data.secondary_dim * p_data.leading_dim+i]; 
        }
    }
    else{
        for(i=0;i <p_data.secondary_dim * p_data.leading_dim; i++){
            p_data.dataset[i] = data_in.dataset[my_rank * n_split * p_data.leading_dim + i];
        }
    }

	/************************* Clustering *************************/

	/***** Initializations *****/

	// SumOfDist is the sum for the old iteration 
	// new_SumOfDist is the sum for the current iteration 
	// The difference of them is compared with the threshold to see if we have
    // converged sufficiently
	double SumOfDist = 0, new_SumOfDist=0, temp_SumOfDist=0;
	double* newCentroids;
	int* temp_clusterSize;
	unsigned int*temp_dataMembers;

	// Temporary array for cluster sizes 
	// Used in reduction, as we can't use the same argument for send and receive
	temp_clusterSize = (int*)malloc(num_clusters * sizeof(int));

	// Temporary array for data members -in which cluster they belong to- (used in reduction)
	temp_dataMembers = (unsigned int*)malloc(num_points * sizeof(unsigned int));

	// Our new centroids after each kmeans iteration step
	newCentroids = (double*)malloc(num_Dimensions * num_clusters * sizeof(double));

	// Initialize all cluster sizes to zero
	for (i = 0; i < num_clusters; i++){
		temp_clusterSize[i] = 0;
	}

	// Make sure that all processes start clustering at the same time
	MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();
    double pre_processing_time = end-start;
    if(my_rank == 0){
    	MPI_Reduce(MPI_IN_PLACE, &pre_processing_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    }
    else{
	MPI_Reduce(&pre_processing_time, NULL, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    }
	if (my_rank == 0){
        printf("Running time step %d on %d processes\n", time_set, world_size);
		printf("Clastering is going to start!\n");
	}
    start = MPI_Wtime();
	/***** Start clustering *****/
	for(iter = 0; iter < max_iterations; iter++){
		new_SumOfDist = 0;
		temp_SumOfDist = 0;

		for (i = 0; i < num_clusters * num_Dimensions; i++){
			newCentroids[i] = 0;
		}

		kmeans_process(&p_data, &clusters, newCentroids, &new_SumOfDist);

		// "Pass" centroids to the struct
		MPI_Allreduce(newCentroids, clusters.dataset, num_clusters*num_Dimensions, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

		// Each process has its own cluster sizes, so we have to sum them up!
		MPI_Allreduce(clusters.members, temp_clusterSize, num_clusters, MPI_UNSIGNED, MPI_SUM, MPI_COMM_WORLD);  

		// "Pass" cluster sizes to the struct
		for (i = 0; i < num_clusters; i++){
       		clusters.members[i] = temp_clusterSize[i];
		}

		// Update cluster centers
		for (i = 0; i < num_clusters; i++){
			for (j = 0; j < num_Dimensions; j++){
				clusters.dataset[i * num_Dimensions + j] /= (double) clusters.members[i];
			}
		}
		
		// Calculate total sum from local sums of processes
		MPI_Allreduce(&new_SumOfDist, &temp_SumOfDist, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

		// If we have reached threshold, stop clustering
		if (fabs(SumOfDist - temp_SumOfDist) < threshold){
			break;
		}

		// Get the new sum
		SumOfDist = temp_SumOfDist;
	}

	// Free some space
	/* free(newCentroids); */
	free(temp_clusterSize);
	
	MPI_Barrier(MPI_COMM_WORLD);

	// Each process saves its own data in a temporary array
	for (i = 0; i < p_data.secondary_dim; i++){ 
		temp_dataMembers[my_rank * p_data.secondary_dim + i] = p_data.members[i]; 
	} 

	// Merge the above data
	MPI_Allreduce(temp_dataMembers, data_in.members, num_points, MPI_UNSIGNED, MPI_SUM, MPI_COMM_WORLD);

	free(temp_dataMembers);

	// Wait for all processes to reach this point
	MPI_Barrier(MPI_COMM_WORLD);
    
    //Processing over, compute time
    end = MPI_Wtime();
    double processing_time = end-start;
    if(my_rank == 0){
    	MPI_Reduce(MPI_IN_PLACE, &processing_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    }
    else{
	MPI_Reduce(&processing_time, NULL, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    }

    double total_time = pre_processing_time + processing_time;

    //Clustering has been completed
	if (my_rank == 0){
		printf("Finished after %d iterations\n", iter);
  
        save_data_to_file(time_set, num_clusters, &clusters, world_size, out_file, pre_processing_time, processing_time);
		printf("Saved!\n");
	}

    //Clean memory
	clean(&p_data);	
	clean(&data_in);
	clean(&clusters);
    free(newCentroids);

	if (my_rank == 0){
		printf("Program has finished!\n");
	}
	
	MPI_Finalize();
}

#include "gendata.h"
#include "quicksort.h"
#include "merge.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>

#define N_dflt 1000
#define HOT 0

void print_data(data_t*,int);


int main(int argc,char** argv){	

	MPI_Init(&argc, &argv);
        int numtasks, rank;

	int n = N_dflt;
	// check command-line arguments 
	int a = 0;
	char p = 0;
	if ( argc > ++a ) n = atoi(*(argv+a));
	if ( argc > ++a ) p = (*(argv+a))[0];
	
	// share the workload with other processes
       
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
  	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if(rank == 0){
		printf("starting %d processes...\n",numtasks);
	}
	
	n = n/numtasks + (n%numtasks > 0 ? 1 : 0);
	data_t* arr_data = generate_data(n);
	printf("Data have been generated \n");
	//print_data(arr_data,n);
	
	
	switch(p)
	{
		case 'p':
			printf("Starting the parallel quicksort...\n");
        		parallel_quicksort( arr_data, 0, n-1);
			break;
		case 'n':
			printf("Starting the nested parallel quicksort...\n");
			parallel_quicksort_nested( arr_data, 0, n-1);
			break;
		default: 
			printf("Starting the serial quicksort...\n");
			serial_quicksort( arr_data, 0, n-1);
	}
		
	//print_data(arr_data,n);
	
  	if ( verify_sorting( arr_data, 0, n-1, 0) )
    		printf("the array is sorted\n");
		//printf("%d\t%g sec\n", nthreads, tend-tstart);
  	else
    		printf("the array is not sorted correctly\n");

	/* 
	 * Process with rank 0 has to gather the distributed 
	 * sorted arrays and merge them
	 *
	 * int MPI_Gather(const void *sendbuf, int sendcount, 
	 * MPI_Datatype sendtype, void *recvbuf, int recvcount, 
	 * MPI_Datatype recvtype, int root, MPI_Comm comm)
	 *
	 */

	data_t* gathered_data;
  	if (rank == 0){
		gathered_data = (data_t*) malloc(n*numtasks*sizeof(data_t));
		if (gathered_data == NULL) {
            		printf("Memory allocation failed for gathered_data\n");
            		MPI_Abort(MPI_COMM_WORLD, 1);
		}
	}
	
	//create an MPI data type for the data_t struct
	MPI_Datatype struct_data;
	MPI_Type_contiguous(DATA_SIZE, MPI_DOUBLE, &struct_data);
	MPI_Type_commit(&struct_data);
	
	// MPI_Gather(arr_data, n, struct_data, gathered_data, n, struct_data, 0, MPI_COMM_WORLD);
	if(MPI_SUCCESS != MPI_Gather(arr_data,n,struct_data,gathered_data, n, struct_data, 0, MPI_COMM_WORLD)){
		printf("MPI_gather error...\n");
		return 0;
	}
		
	if (rank == 0){
		printf("data have been gathered by process %d \n",rank);
		merge_gathered_data(gathered_data,n,numtasks);
		if ( verify_sorting( gathered_data, 0, n*numtasks-1, 0) )
                	printf("the array is sorted\n");
        	else
                printf("the array is not sorted correctly\n");
	}
	
	MPI_Finalize();
	free( arr_data );

	return 0;
}

void print_data(data_t* arr_data,int n){
	for(int i = 0; i < n; i++){
		printf("%f ",arr_data[i].data[HOT]);
	}
}

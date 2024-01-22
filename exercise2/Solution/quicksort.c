#include "gendata.h"
#include "quicksort.h"
#include "merge.h"
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int verify_sorting( data_t *data, int start, int end, int not_used )
{
  	int i = start;
  	while( (++i < end) && (data[i].data[HOT] >= data[i-1].data[HOT]) );
  	return ( i == end );
}

int compare_ge( const void *A, const void *B )
{
        data_t *a = (data_t*)A;
        data_t *b = (data_t*)B;
	//printf("compare_ge \n");
        return ((a->data)[HOT] >= (b->data)[HOT]);
}

int partitioning( data_t *data, int start, int end){
	int middle = (end + start) / 2;

    	// Ensure data[start] <= data[middle] <= data[end]
    	if (compare_ge((void*)&data[start],(void*)&data[middle]))
        	SWAP((void*)&data[start], (void*)&data[middle], sizeof(data_t));
    	if(compare_ge((void*)&data[middle],(void*)&data[end]))
        	SWAP((void*)&data[middle], (void*)&data[end], sizeof(data_t));
    	if(compare_ge((void*)&data[start],(void*)&data[middle]))
        	SWAP((void*)&data[start], (void*)&data[middle], sizeof(data_t));
	
	SWAP((void*)&data[end],(void*)&data[middle],sizeof(data_t));
	// now the pivot is the last element of the list
	void* pivot = (void*)&data[end];
	
	// now we have to execute the partition with the pivot as the last element
	int pointbreak = end-1;
  	for ( int i = start; i <= pointbreak; i++ )
    		if( compare_ge( (void*)&data[i], pivot ) )
      		{
        		while( (pointbreak > i) && compare_ge( (void*)&data[pointbreak], pivot ) ) 
				pointbreak--;
        		if (pointbreak > i ){
          			SWAP( (void*)&data[i], (void*)&data[pointbreak], sizeof(data_t) );          
				pointbreak--;
        		}
      		}
	pointbreak += !compare_ge( (void*)&data[pointbreak], pivot ) ;
  	SWAP( (void*)&data[pointbreak], pivot, sizeof(data_t) );	
	return pointbreak;
}

void serial_quicksort( data_t *data, int start, int end)
{
	//printf("\n Quicksort call \n");
	int size = end-start+1;
  	if ( size > 2 )
    	{
      		int mid = partitioning( data, start, end);

		serial_quicksort( data, start, mid-1 );    // sort the left half
		serial_quicksort( data, mid+1, end );   // sort the right half
    	}
  	else
    	{	
      		if ( (size == 2) && compare_ge ( (void*)&data[start], (void*)&data[end] ) )
        		SWAP( (void*)&data[start], (void*)&data[end], sizeof(data_t) );
    	}	
}

void parallel_quicksort_nested( data_t* data, int start, int end)
{
	
	int size = end-start+1;
  	if ( size > 2 )
    	{
      		int mid = partitioning( data, start, end);

		omp_set_dynamic(0);
		#pragma omp parallel num_threads(2)
		{	
			int id = omp_get_thread_num();
			if(id == 0){
				printf("#threads: %d \n", omp_get_num_threads());
				parallel_quicksort_nested(data, start, mid-1);
			}
			if(id == 1)
				parallel_quicksort_nested(data, mid+1, end);
		}
	}
  	else
    	{	
      		if ( (size == 2) && compare_ge ( (void*)&data[start], (void*)&data[end] ) )
        		SWAP( (void*)&data[start], (void*)&data[end], sizeof(data_t) );
    	}	
}

int verify_partition_sorting(data_t* data,int* starts,int end, int parts)
{
	int v = 0;
	int tmp = 0;
	for(int i = 0; i < parts; i++){
		tmp = starts[i+1]-1;
		if(i == parts-1)
			tmp = end;
		v = verify_sorting(data,starts[i],tmp,0);
		if (v)
			printf("sub-array %d is sorted correctly \n", i);
		else
			printf("sub-array %d is not sorted correctly \n", i);
	}
	
}

void parallel_quicksort( data_t* data, int start, int end)
{
	int nThreads;
	int* starts;

	#pragma omp parallel 
	{
		int myId = omp_get_thread_num();
		#pragma omp master
		{
			nThreads = omp_get_num_threads();
			starts = (int*) malloc(nThreads*sizeof(int));
			printf("spawned %d threads... \n",nThreads);
		}
		#pragma omp barrier	
		int my_length = (end+1) / nThreads + ((myId < ((end+1) % nThreads)) ? 1:0);
		int my_start = my_length*myId + ((myId >= ((end+1) % nThreads)) ? (end+1) % nThreads : 0);
		#pragma omp critical
		{
			printf("thread %d starts working at index %d with a length of %d\n",myId,my_start,my_length);
		}
		
		serial_quicksort(data,my_start, my_start + my_length-1);
		starts[myId] = my_start;
	}

	/*
	 *Now, we have that our array is made up by nThreads sorted sub-arrays, let's merge them
	 * */
	verify_partition_sorting(data,starts,end,nThreads);
	printf("merging arrays...\n");
	merge_all(data,starts,end+1,nThreads);
}





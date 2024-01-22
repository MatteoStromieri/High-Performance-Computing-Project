#include "gendata.h"
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <stdio.h>

data_t* generate_data(int n){
	
	data_t *arr_data = (data_t*)malloc(n*sizeof(data_t));
  	long int seed;
 	#if defined(_OPENMP)
 	#pragma omp parallel
  	{
    		int me             = omp_get_thread_num();
    		short int seed     = time(NULL) % ( (1 << sizeof(short int))-1 );
    		short int seeds[3] = {seed-me, seed+me, seed+me*2};

   		#pragma omp for
    		for ( int i = 0; i < n; i++ )
      			arr_data[i].data[HOT] = erand48( seeds );
  	}
 	#else
  	{
    		seed = time(NULL);
    		srand48(seed);
		
		printf("Writing the array \n");
    		for ( int i = 0; i < n; i++ ){
      			arr_data[i].data[HOT] = drand48();
  			printf("%f ",arr_data[i].data[HOT]);
		}
		printf("Done writing the array \n");
	}
 	#endif

	return arr_data;
}

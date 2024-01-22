#include <stdlib.h>
#include "gendata.h"
#include "quicksort.h"

// Function to merge two sorted arrays
hot_data_t* merge_arrays( hot_data_t* data, int start1, int start2, int end1, int end2, hot_data_t* merged) 
{
    	int size1 = end1 - start1 + 1;
	int size2 = end2 - start2 + 1;
	int k = 0;
	if (merged == NULL)
		merged = (hot_data_t*)malloc((size1 + size2) * sizeof(hot_data_t));

    // Compare elements from both arrays and merge them
	while (start1 <= end1 && start2 <= end2) {
		if ( !compare_ge((void*)&data[start1],(void*)&data[start2])){
            		merged[k] = data[start1];
            		start1++;
        	} else {
            		merged[k] = data[start2];
            		start2++;
        	}
        	k++;
    	}

    	// Copy remaining elements from arr1, if any
    	while (start1 <= end1) {
        	merged[k] = data[start1];
        	start1++;
        	k++;
    	}

    	// Copy remaining elements from arr2, if any
    	while (start2 <= end2) {
        	merged[k] = data[start2];
        	start2++;
        	k++;
    	}

    	return merged;
}

void copy_array(hot_data_t* data, hot_data_t* merged, int start, int size) {
    for (int i = 0; i < size; i++) {
        data[i+start] = merged[i];
    }
}

void merge_all(hot_data_t* data, int* starts, int lenData, int nStarts)
{
	//allocate an array as long as data
	hot_data_t* merged = (hot_data_t*) malloc(lenData*sizeof(hot_data_t));

	int i;
	for(i = 0; i < nStarts - 2; i++){
		merge_arrays( data, 0, starts[i+1],starts[i+1]-1,starts[i+2]-1,merged);
		copy_array(data,merged,0,starts[i+2]);
	}

	merge_arrays( data, 0, starts[i+1],starts[i+1]-1,lenData-1,merged);
	copy_array(data,merged,0,lenData);

	free(merged);

}
// data_t* data, int start1, int start2, int end1, int end2, data_t* merged
void merge_gathered_data(hot_data_t* data,int step,int nsections){
        //allocate an array as long as data
        hot_data_t* merged = (hot_data_t*) malloc(step*nsections*sizeof(hot_data_t));

        int i;
        for(i = 0; i < nsections-1; i++){
                merge_arrays( data, 0, step*(i+1),step*(i+1)-1,step*(i+2)-1,merged);
                copy_array(data,merged,0,step*(i+2));
        }

        free(merged);
}

#include "gendata.h"

#define SWAP(A,B,SIZE) do {int sz = (SIZE); char *a = (A); char *b = (B); do { char _temp = *a;*a++ = *b;*b++ = _temp;} while (--sz);} while (0)

int verify_sorting( hot_data_t*, int, int, int);

int compare_ge( const void *, const void *);

void serial_quicksort( hot_data_t*, int, int);

void parallel_quicksort( hot_data_t*, int, int);

void parallel_quicksort_nested( hot_data_t*, int, int);

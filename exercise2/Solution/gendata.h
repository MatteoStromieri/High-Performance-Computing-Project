#ifndef GENDATA_H
#define GENDATA_H

#if !defined(DATA_SIZE)
#define DATA_SIZE 8
#endif
#define HOT       0

typedef struct {
	double data[DATA_SIZE];
} data_t;

data_t* generate_data(int);

#endif

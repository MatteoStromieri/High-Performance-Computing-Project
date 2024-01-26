#ifndef GENDATA_H
#define GENDATA_H

#if !defined(DATA_SIZE)
#define DATA_SIZE 8
#endif
#define HOT       0

#define POINTER_SIZE sizeof(void*)

typedef struct {
	double data[DATA_SIZE];
} data_t;

typedef struct {
	double hot;
	char data[8]; // data_t* data;
} hot_data_t;

hot_data_t* generate_data(int);

#endif

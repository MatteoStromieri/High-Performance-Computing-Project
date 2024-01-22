#include <stdio.h>

int main(){
	int a = 10;
	double d = 10.0;
	int* pt_a = &a;
	printf("size of a pointer is %d\n",sizeof(pt_a));
	printf("size of a double is %d\n",sizeof(d));
	double double_pt = (double) pt_a;
	printf("casting of a pointer to double: %f\n", double_pt);
	pt_a = (int*) double_pt;
	printf("value pointed by the pointer obtained by the cast of the double: %d\n", *pt_a);
}

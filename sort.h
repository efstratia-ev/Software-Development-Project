#ifndef SORT_H
#define SORT_H


#include "list.h"
#include "radix.h"

list *finalresults(mytuple *array1, mytuple *array2, uint64_t size1, uint64_t size2);

void sort(radix *r);

int countResults(mytuple *array1, mytuple *array2, uint64_t size1, uint64_t size2);


#endif

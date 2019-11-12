#ifndef SORT_H
#define SORT_H

#include "array.h"
#include "list.h"
#include "radix.h"

list *join(array *array1,array *array2);

void sort(radix *r);

int countResults(mytuple *array1, mytuple *array2, uint64_t size1, uint64_t size2);


#endif

#ifndef SORT_H
#define SORT_H

#include "array.h"
#include "list.h"
#include "radix.h"

list *join(array *array1,array *array2,uint64_t *column1,uint64_t *column2);

array *sort(radix *r);



#endif

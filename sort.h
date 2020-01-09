#ifndef SORT_H
#define SORT_H

#include "rows_array.h"
#include "list.h"
#include "radix.h"

list *join(rows_array *rows_array1,rows_array *rows_array2,uint64_t *column1,uint64_t *column2);

list *sortedjoin(rows_array *rows_array1,rows_array *rows_array2,uint64_t *column1,uint64_t *column2);

rows_array *sort(radix *r);



#endif

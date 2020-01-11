#ifndef SORT_H
#define SORT_H

#include "query.h"
#include "rows_array.h"
#include "list.h"
#include "radix.h"
#include "jobscheduler.h"
#define NUMJOBS 2

extern JobScheduler *js;

list *join(rows_array *rows_array1,rows_array *rows_array2,uint64_t *column1,uint64_t *column2);

list *sortedjoin(rows_array *rows_array1,rows_array *rows_array2,uint64_t *column1,uint64_t *column2);

rows_array *sort(sem_t *semaphore, Query *Q, radix *r);



#endif

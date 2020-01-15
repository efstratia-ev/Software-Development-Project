#ifndef SORT_H
#define SORT_H

#include "query.h"
#include "rows_array.h"
#include "list.h"
#include "radix.h"
#include "jobscheduler.h"
#define NUMJOBS 2

extern JobScheduler *js;

void join(int arr1,int arr2,rows_array *rows_array1,rows_array *rows_array2,uint64_t *column1,uint64_t *column2,Query *Q,bool sorted);

rows_array *sort(sem_t *semaphore, Query *Q, radix *r);

void add_join_results(uint64_t offset1,uint64_t size1,uint64_t offset2,uint64_t size2,rows_array *rows_array1,rows_array *rows_array2,list *resultlist);

void add_sortjoin_results(uint64_t offset1, uint64_t size1, uint64_t offset2, uint64_t size2,rows_array *rows_array2, list *resultlist);
#endif

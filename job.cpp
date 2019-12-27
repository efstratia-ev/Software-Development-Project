#include "do_query.h"
#include "job.h"

int QueryJob::Run() {
    *sums = DoQuery(sql,rels);
}
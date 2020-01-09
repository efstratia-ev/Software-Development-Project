#include "do_query.h"
#include "job.h"

int QueryJob::Run() {
    s=query->DoQuery();
}



#include <inttypes.h>
#include <stdint.h>
#include "query.h"
#include "jobscheduler.h"
#include "job.h"
#include "results_list.h"

extern JobScheduler *js;

//this like an interface i.e it's never instanstiated
class QueriesExecutor {
protected:
    results_list* resultsList;
public:
    QueriesExecutor();
    virtual void prepareResults(Query *query);
    virtual void flush();
    virtual void runQuery(Query *query) {}
    ~QueriesExecutor();
};

class ParallelQueriesExecutor : public QueriesExecutor {
public:
    void runQuery(Query *query);
};

class SequentialQueriesExecutor : public QueriesExecutor {
public:
  SequentialQueriesExecutor() : QueriesExecutor() {}
  void runQuery(Query *query);
};
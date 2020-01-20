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
    QueriesExecutor() {
      resultsList = new results_list();
      js = new JobScheduler();
      js->Init(4);
    }
    virtual void prepareResults(Query *query) {
      resultsList->add(query->getSQL()->get_results_counter(),query->getSums());
    }
    virtual void flush() {
        js->Barrier();
        resultsList->print();
        resultsList->clear();
    }
    virtual void runQuery(Query *query) {}
    ~QueriesExecutor() {
      delete resultsList;
      js->Stop();
      delete js;
    }
};

class ParallelQueriesExecutor : public QueriesExecutor {
  public:
    void runQuery(Query *query) {
        QueriesExecutor::prepareResults(query);
        auto job = new QueryJob(query);
        js->Schedule(job);
    }
};

class SequentialQueriesExecutor : public QueriesExecutor {
  public:
  SequentialQueriesExecutor() : QueriesExecutor() {}
  void runQuery(Query *query) {
    QueriesExecutor::prepareResults(query);
    if(query->DoQuery(query->execute_filters())) delete query;
  }
};
#include "queriesExecutor.h"
#include "select_options.h"

QueriesExecutor::QueriesExecutor() {
    resultsList = new results_list();
    js = new JobScheduler();
    if(NUM_THREADS) js->Init(NUM_THREADS);
    else {
        int threads=std::thread::hardware_concurrency();
        if(threads) js->Init(threads);
        else js->Init(4);
    }
}

void QueriesExecutor::prepareResults(Query *query) {
    resultsList->add(query->getSQL()->get_results_counter(),query->getSums());
}

void QueriesExecutor::flush() {
    js->Barrier();
    resultsList->print();
    resultsList->clear();
}

QueriesExecutor::~QueriesExecutor() {
    delete resultsList;
    js->Stop();
    delete js;
}

void ParallelQueriesExecutor::runQuery(Query *query) {
    QueriesExecutor::prepareResults(query);
    auto job = new QueryJob(query);
    js->Schedule(job);
}

void SequentialQueriesExecutor::runQuery(Query *query) {
    QueriesExecutor::prepareResults(query);
    if(query->DoQuery(query->execute_filters())) delete query;
}

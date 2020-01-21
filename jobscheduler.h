#ifndef JOBSCHEDULER
#define JOBSCHEDULER
#include <thread>
#include <mutex>
#include <condition_variable>
#include "job.h"
#include "queue.h"

//using namespace std;

class JobScheduler {
    int numThreads;
    thread *threads;
    mutex activeJobsMu;
    condition_variable activeJobsCv;
    int activeJobs; //number of jobs queued or in running state
    mutex queuMu; //mutex protecting q
    condition_variable cv; //cond var for q
    queue *q; //job queue
    bool stop; //set when we call Stop()
public:
    JobScheduler();
    bool Init(int numThreads);
    bool Destroy();
    void Barrier();
    void Schedule(Job *job);
    void Schedule(Job *job,sem_t *sem,int val);
    void Schedule(queue *Q);
    void Stop();
    void runJobs();
    void addActiveJobs();
    void addActiveJobs(int jobs);
    void removeActiveJobs();
    //getters for debug
    int getActiveJobs();
    int getNumThreads();
    int getQueueSize();
    ~JobScheduler();
};

#endif
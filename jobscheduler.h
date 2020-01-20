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
    void Schedule(Job *job,sem_t *sem,int val) {
        addActiveJobs();
        unique_lock<mutex> lk(queuMu);
        q->push(job,sem,val);
        lk.unlock();
        cv.notify_one();
    }
    void Schedule(queue *Q) {
        addActiveJobs(Q->getSize());
        unique_lock<mutex> lk(queuMu);
        q->push(Q);
        lk.unlock();
        cv.notify_one();
    }
    void Stop();
    void runJobs();
    void addActiveJobs();
    void addActiveJobs(int jobs) {
        unique_lock<mutex> lk(activeJobsMu);
        activeJobs+=jobs;
        lk.unlock();
    }
    void removeActiveJobs();
    //getters for debug
    int getActiveJobs() {
        unique_lock<mutex> lk(activeJobsMu);
        auto tmp = activeJobs;
        lk.unlock();
        return tmp;
    }
    int getNumThreads() { 
        return numThreads;
    }
    int getQueueSize() {
        unique_lock<mutex> lk(queuMu);
        auto tmp = q->getSize();
        lk.unlock();
    }
    ~JobScheduler(){
        delete q;
        delete[] threads;
    }
};

#endif
#include <thread>
#include <chrono>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "jobscheduler.h"

using namespace std;


class DummyJob : public Job {
    public:
    virtual int Run() {}
};

class CPUIntensiveJob : public Job {
    public:
    int numIterations;
    CPUIntensiveJob(int numIterations) {
        this->numIterations = numIterations;
    }
    //Run performs a cpu intensive job. Specifically it finds the sum of [0,numIterations]
    virtual int Run() {
        int sum = 0;
        for (int i = 0; i < numIterations; i++)
            sum+= i;
        return sum;
    }
};

class SleepJob : public Job {
    public:
    int numSeconds;
    SleepJob(int numSeconds) {
        this->numSeconds = numSeconds;
    }
    virtual int Run() {
        this_thread::sleep_for(chrono::seconds(numSeconds));
    }
};

int numIterationsCPUIntensive = 1000000;

//jobType 1 -> DummyJob
//jobType 2 -> CPUIntensiveJob
//jobType 3 -> SleepJob
double BenchmarkSchedulerJobs(JobScheduler *js,int numJobs,int jobType) {
    auto begin = chrono::steady_clock::now();
    for (int i =0; i < numJobs; i++) {
        switch (jobType) {
        case 1: js->Schedule(new DummyJob());break;
        case 2: js->Schedule(new CPUIntensiveJob(numIterationsCPUIntensive));break;
        case 3: js->Schedule(new SleepJob(1));break;
        }
    }
    js->Barrier();
    auto end = std::chrono::steady_clock::now();
    CU_ASSERT(js->getActiveJobs() == 0);
    //cout << "Processed " << numJobs << " Jobs with job type " << jobType << " in " 
    //<< chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "ns" << endl;
    return chrono::duration_cast<std::chrono::milliseconds> (end - begin).count(); 
}

double BenchmarkSingleThreadJobs(int numJobs,int jobType){
    auto begin = chrono::steady_clock::now();
    for (int i =0; i < numJobs; i++) {
        switch (jobType) {
        case 1: (new DummyJob())->Run();break;
        case 2: (new CPUIntensiveJob(numIterationsCPUIntensive))->Run();break;
        case 3: (new SleepJob(1))->Run();break;
        }
    }
    auto end = std::chrono::steady_clock::now();
    return chrono::duration_cast<std::chrono::milliseconds> (end - begin).count();
}

void benchmarkScheduler() {
    auto js = new JobScheduler();
    int numThreads = 4;
    js->Init(numThreads);
    double res = BenchmarkSingleThreadJobs(10000,1) / BenchmarkSchedulerJobs(js,10000,1);
    //propably scheduler takes more time because thread context switching and destruction and 
    //synchronizations is expensive.
    cout << res << endl;
    int numJobs = 1000;
    res = BenchmarkSingleThreadJobs(numJobs,2) / BenchmarkSchedulerJobs(js,numJobs,2);
    cout << res << endl;
    CU_ASSERT(res > 2);
    res = BenchmarkSingleThreadJobs(10,3) / BenchmarkSchedulerJobs(js,10,3);
    cout << res << endl;
    CU_ASSERT(res > 2);
}


int init_suite1(void)
{
   return 0;
}

int clean_suite1(void)
{
   return 0;
}

int main(int argc,char *argv[]) {
   CU_pSuite pSuite = NULL;
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }
   if ((NULL == CU_add_test(pSuite, "benchmark jobs",benchmarkScheduler)))
         {
         CU_cleanup_registry();
      return CU_get_error();
   }
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
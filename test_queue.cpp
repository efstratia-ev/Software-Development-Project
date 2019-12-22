#include "CUnit/Basic.h"
#include "CUnit/CUnit.h"
#include "queue.h"


class DummyJob : public Job {
    virtual int Run() {}
};

void testQueue() {
    auto q = new queue();
    Job *job = new DummyJob();
    q->push(job);
    CU_ASSERT(q->getHead() != nullptr);
    CU_ASSERT(q->getHead() == q->getTail());
    auto head = q->pop();
    CU_ASSERT(head == job);
    CU_ASSERT(q->pop() == nullptr);
    for (int i = 0; i < 100;i++ ) {
        q->push(new DummyJob());
    }
    CU_ASSERT(q->getSize() == 100);
    //q->print();
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
   if ((NULL == CU_add_test(pSuite, "test queue",testQueue)))
         {
         CU_cleanup_registry();
      return CU_get_error();
   }
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
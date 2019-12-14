#define private public
#include<iostream>
#include <string>
#include "CUnit/Basic.h"
#include "CUnit/CUnit.h"
#include "relation.h"
#include "relations.h"

using namespace std;


void test_Relation() {
    char path[] = "../workloads/small/r0";
    auto rel = new Relation(path);
    CU_ASSERT(rel->getRows() == 1561);
    CU_ASSERT(rel->getCols() == 3);
    CU_ASSERT(rel->value(0,0) == 1);
    CU_ASSERT(rel->value(0,1) == 8463);
    CU_ASSERT(rel->value(1,2) == 6962);
    uint64_t sz;
    CU_ASSERT(rel->col(2,sz)[0] == 582);
    CU_ASSERT(sz == rel->getRows());
    delete rel;
}

void test_Relations() {
    char path[] = "../workloads/small/small.init";
    auto rels = new Relations(path);
    CU_ASSERT(rels->getSize() ==  14);
    auto rel = rels->relation(0);
    CU_ASSERT(rel->getRows() == 1561);
    CU_ASSERT(rel->getCols() == 3);
    rel = rels->relation(12);
    CU_ASSERT(rel->value(1,2) == 1509);
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
   if ((NULL == CU_add_test(pSuite, "test relation",test_Relation)) || 
        (NULL == CU_add_test(pSuite, "test relation",test_Relations)))
         {
         CU_cleanup_registry();
      return CU_get_error();
   }
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}

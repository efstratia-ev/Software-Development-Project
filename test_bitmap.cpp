#include <iostream>
#include "CUnit/Basic.h"
#include "CUnit/CUnit.h"
#include "bitmap.h"


void testBitmap() {
    auto min = 10;
    auto bm = new BitMap(81,min);
    auto bytes = bm->getBytes();
    bm->set(10);
    CU_ASSERT(bm->get(10) == true);
    bm->set(11);
    CU_ASSERT(bm->get(11) == true);
    bm->set(91);
    CU_ASSERT(bm->get(91) == true);
    bm->set(82);
    CU_ASSERT(bm->get(82) == true);
    for (int i = min; i < 81+min; i++) {
        if (i == 10 || i == 11 || i == 91 || i == 82) 
            continue;
        CU_ASSERT(bm->get(i) == false);
    }
    CU_ASSERT(bm->countSetBits() == 4);
    bm = new BitMap(MAX_SIZE+10,20);
    bm->set(55);
    bm->set(MAX_SIZE+10);
    CU_ASSERT(bm->countSetBits() == 2);
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
   if ((NULL == CU_add_test(pSuite, "test bitmap",testBitmap)))
         {
         CU_cleanup_registry();
      return CU_get_error();
   }
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
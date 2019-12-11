
#define private public
#include<iostream>
#include <string>
#include "CUnit/Basic.h"
#include "CUnit/CUnit.h"
#include<iostream>
#include<fstream>
#include <cstdlib>
#include <cstring>
#include "radix.h"
#include "stack.h"
#include "sort.h"
#include "array.h"
#include "relations.h"
#include "results_list.h"
#include "JoinArray.h"
#include "join_preds.h"

using namespace std;

uint64_t rel1data[] = {
   2,2,3,
   4,5,6,
   7,8,9,
   10,11,12,
};

uint64_t rel2data[] = {
   1,2,100,
   133,2,99,
   8,11,98,
   8,11,101,
};


Relations *createRels() {
    uint64_t rel1rows = 4;
    uint64_t rel1cols = 3;
    auto rel1 = new Relation(rel1data,rel1rows,rel1cols);
    uint64_t rel2rows = 4;
    uint64_t rel2cols = 3;
    auto rel2 = new Relation(rel2data,rel2rows,rel2cols);
    int sz = 2;
    Relation **_rels = new Relation*[2];
    _rels[0] = rel1;_rels[1] = rel2;
    Relations *rels = new Relations(_rels,sz);
    return rels;
}

void testJoin() {
    char query[] = "0 1|0.0=1.0|0.0";
    auto sql = new SQL(query);
    auto rels = createRels();
    rels->set_query_rels(sql->get_from_arrays());
    auto result = joinFirstPredicate(nullptr,sql,rels,0);
    auto arr = result->getArray();
    cout << "results:" << endl;
    for (int i = 0; i < result->getSize(); i++) {
       for (int j =0; j < result->getNumRels(); j++) {
            cout << arr[i][j] << endl;
       }
    }

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
   if ((NULL == CU_add_test(pSuite, "test relation",testJoin)))
         {
         CU_cleanup_registry();
      return CU_get_error();
   }
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}

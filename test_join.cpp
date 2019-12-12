
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

//the relation is stored column-oriented, so at rel1data the first
//column is 2,2,3 the second 4,5,6 etc.
uint64_t rel1data[] = {
   2,2,1, 
   1,5,6,
   7,8,9,
   10,11,1,
};

uint64_t rel2data[] = {
   1,2,1,
   133,2,99,
   8,1,1,
   8,11,101,
};

uint64_t rel3data[] = {
   1,1,76,
};


Relations *createRels() {
    uint64_t rel1rows = 3;
    uint64_t rel1cols = 4;
    auto rel1 = new Relation(rel1data,rel1rows,rel1cols);
    uint64_t rel2rows = 3;
    uint64_t rel2cols = 4;
    auto rel2 = new Relation(rel2data,rel2rows,rel2cols);
    uint64_t rel3rows = 3;
    uint64_t rel3cols = 1;
    auto rel3 = new Relation(rel3data,rel3rows,rel3cols);
    int sz = 3;
    Relation **_rels = new Relation*[2];
    _rels[0] = rel1;_rels[1] = rel2;_rels[2] = rel3;
    Relations *rels = new Relations(_rels,sz);
    return rels;
}

JoinArray *JoinQuery(char *query,Relations *rels) {
    auto sql = new SQL(query);
    rels->set_query_rels(sql->get_from_arrays());
    auto result = joinPredicates(nullptr,sql,rels,0);
    return result;
}

void testJoin() {
    auto rels = createRels();
    JoinArray *result;
    //------------------------------------
    //first
    char query[] = "0 1 2|0.0=1.0&1.0=2.0|0.0";
    result = JoinQuery(query,rels);
    CU_ASSERT(result->getSize() == 4);
    CU_ASSERT(result->getNumRels() == 3);
    //second
    char query2[] = "1 2|0.2=0.0&0.0=1.0|0.0";
    result = JoinQuery(query2,rels);
    CU_ASSERT(result->getSize() == 4);
    CU_ASSERT(result->getNumRels() == 3);
    //third
    char query3[] = "0 0|0.3=0.0|0.1";
    result = JoinQuery(query3,rels);
    CU_ASSERT(result->getSize() == 1);
    CU_ASSERT(result->getNumRels() == 2);
    //fourth
    char query4[] = "0 1 2|1.0=1.2&1.0=2.0&0.1=2.0|2.0";
    result = JoinQuery(query4,rels);
    CU_ASSERT(result->getNumRels()==4);
    //---------------------------
    //instructor's dataset
    char filename[] = "../workloads/small/small.init";
    rels = new Relations(filename);
    char *line= NULL;
    size_t size=0;
    SQL *sql;
    JoinArray *joinRes;
    while(true) {
       getline(&line, &size, stdin);
       cout << "got one" << endl;
       line=strtok(line,"\n");
       if (!line) continue;
       if (strcmp(line, "Done") == 0)
         break;
      if (strcmp(line,"F") == 0) 
         break; //avoid crush - change afterwards with continue
       else{
           sql=new SQL(line);
           for(int i=0; i < sql->get_filters_num();i++)
            sql->getPredicate();
           rels->set_query_rels(sql->get_from_arrays());
           joinRes = joinPredicates(nullptr,sql,rels,0);
           if (joinRes->getNumRels() != 0) {
               cout << sql->getNumInnerJoins() + 1 << "," << joinRes->getNumRels() << endl;
               CU_ASSERT(sql->getNumInnerJoins() + 1 == joinRes->getNumRels());
           }
           delete sql;delete joinRes;
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

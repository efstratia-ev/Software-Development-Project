#include "CUnit/Basic.h"
#include "CUnit/CUnit.h"
#include<iostream>
#include<fstream>
#include <cstdlib>
#include <string>
#include "radix.h"
#include "stack.h"
#include "inputFunctions.h"

static FILE* temp_file = NULL;
using namespace std;


bool isSorted(mytuple *arr,int len)  {
    for (int i =1; i < len; i++) {
        if (arr[i].value < arr[i-1].value)
            return false;
    }
    return true;
}

void test_Group() {
    uint64_t count1=0,count2=0;
    struct mytuple *array1,*array2,*_array1,*_array2;
    string filename = "../test.txt";
    if(!getArraySize(filename,count1,count2)) {
        cout<<"Could not open file.\n";
        exit(-1);
    }
    array1=new mytuple[count1];
    array2=new mytuple[count2];
    if(!makearrays(filename,array1,array2)) {
        cout<<"Could not open file.\n";
        exit(-1);
    }
    _array1=new mytuple[count1];
    _array2=new mytuple[count2];
    (new radix(0,count1,array1,_array1))->group();
}

void test_FilequickSort(string filename) {
    uint64_t count1=0,count2=0;
    struct mytuple *array1,*array2,*_array1,*_array2;
    if(!getArraySize(filename,count1,count2)) {
        cout<<"Could not open file.\n";
        exit(-1);
    }
    array1=new mytuple[count1];
    array2=new mytuple[count2];
    if(!makearrays(filename,array1,array2)) {
        cout<<"Could not open file.\n";
        exit(-1);
    }
    _array1=new mytuple[count1];
    _array2=new mytuple[count2];
    (new radix(0,count1,array1,_array1))->quicksort(0,count1);
    (new radix(0,count2,array2,_array2))->quicksort(0,count2);
    CU_ASSERT(isSorted(array1,count1) == true);
    CU_ASSERT(isSorted(array2,count2) == true);
}

void test_qSort() {
   test_FilequickSort("../radix_test3.txt");
   test_FilequickSort("../radix_test2.txt");
}

void testFile(string filename) {
    uint64_t count1=0,count2=0;
    struct mytuple *array1,*array2,*_array1,*_array2;
    if(!getArraySize(filename,count1,count2)) {
        cout<<"Could not open file.\n";
        exit(-1);
    }
    array1=new mytuple[count1];
    array2=new mytuple[count2];
    if(!makearrays(filename,array1,array2)) {
        cout<<"Could not open file.\n";
        exit(-1);
    }
    _array1=new mytuple[count1];
    _array2=new mytuple[count2];
    (new radix(0,count1,array1,_array1))->sort();
    (new radix(0,count2,array2,_array2))->sort();
    CU_ASSERT(isSorted(_array1,count1) == true);
    CU_ASSERT(isSorted(_array2,count2) == true);
    CU_ASSERT(isSorted(array1,count1) == true);
    CU_ASSERT(isSorted(array2,count2) == true);
}

void testFiles() {
    testFile("../test.txt");
    testFile("../radix_test3.txt");
    testFile("../radix_test2.txt");
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
   if ((NULL == CU_add_test(pSuite, "test radix group",test_Group)) || 
      (NULL == CU_add_test(pSuite, "test radix sort",testFiles)))
         {
         CU_cleanup_registry();
      return CU_get_error();
   }
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}

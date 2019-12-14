#include "CUnit/Basic.h"
#include "CUnit/CUnit.h"
#include<iostream>
#include<fstream>
#include <cstdlib>
#include <string>
#include "radix.h"
#include "stack.h"
#include "sort.h"

static FILE* temp_file = NULL;
using namespace std;


bool isSorted(array *arr)  {
   //cout << "start" << endl;
    for (int i =1; i < arr->Size; i++) {
      // cout << arr[i].value << endl;
        if (arr->Array[i].key < arr->Array[i-1].key)
            return false;
    }
    return true;
}

void test_Group() {
    uint64_t count1=0,count2=0;
    struct array *array1,*array2,*_array1,*_array2;
    string filename1 = "../Datasets/tiny/relA";
    string filename2 = "../Datasets/tiny/relB";
    if(!getArraysSize(filename1,filename2,count1,count2)) //find array's size
        exit(-1);
    array1=new array(count1);
    array2=new array(count2);
    if(!makeArrays(filename1,array1,filename2,array2)) //get array's data
        exit(-1);
    _array1=new array(count1);
    _array2=new array(count2);
    (new radix(0,count1,array1->Array,_array1->Array))->group();
}

void test_FilequickSort(string filename1,string filename2) {
    uint64_t count1=0,count2=0;
    struct array *array1,*array2,*_array1,*_array2;
    if(!getArraysSize(filename1,filename2,count1,count2)) //find array's size
        exit(-1);
    array1=new array(count1);
    array2=new array(count2);
    if(!makeArrays(filename1,array1,filename2,array2)) //get array's data
        exit(-1);
    _array1=new array(count1);
    _array2=new array(count2);
    (new radix(0,count1,array1->Array,_array1->Array))->quicksort(0,count1);
    (new radix(0,count2,array2->Array,_array2->Array))->quicksort(0,count2);
    CU_ASSERT(isSorted(array1) == true);
    CU_ASSERT(isSorted(array2) == true);
}

void test_qSort() {
    test_FilequickSort("../Datasets/tiny/relA","../Datasets/tiny/relB");
    test_FilequickSort("../Datasets/small/relA","../Datasets/small/relB");
}

void testFile(string filename1,string filename2) {
    uint64_t count1=0,count2=0;
    struct array *array1,*array2,*_array1,*_array2;
    if(!getArraysSize(filename1,filename2,count1,count2)) //find array's size
        exit(-1);
    array1=new array(count1);
    array2=new array(count2);
    if(!makeArrays(filename1,array1,filename2,array2)) //get array's data
        exit(-1);
    _array1=new array(count1);
    _array2=new array(count2);
    sort(new radix(0,count1,array1->Array,_array1->Array));
    sort(new radix(0,count2,array2->Array,_array2->Array));
    CU_ASSERT(isSorted(array1) == true);
    CU_ASSERT(isSorted(array2) == true);
}

void testFiles() {
    testFile("../Datasets/tiny/relA","../Datasets/tiny/relB");
    testFile("../Datasets/small/relA","../Datasets/small/relB");
    //testFile("../mrelA","../mrelB");

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

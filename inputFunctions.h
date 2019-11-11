#ifndef INPUT_FUNCS
#define INPUT_FUNCS
#include "radix.h"
#include <string>
int getArraySize(std::string filename,uint64_t & count);
int getArraysSize(std::string filename1,std::string filename2,uint64_t & count1,uint64_t & count2);
int makeArray(std::string filename,mytuple * array1);
int makeArrays(std::string filename1,mytuple * array1,std::string filename2,mytuple * array2);
#endif

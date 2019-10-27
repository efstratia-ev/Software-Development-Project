#ifndef INPUT_FUNCS
#define INPUT_FUNCS
#include "radix.h"
#include <string>
int getArraySize(std::string filename,uint64_t & count1,uint64_t & count2);
int makearrays(std::string filename,mytuple * array1,mytuple * array2);
#endif

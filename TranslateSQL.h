#define COLUMNSPRE 4
#define COLUMNSRE 2

void getFROM(std::string line,int * &arrays);
int findArrays(int i,std::string line);
void getWHERE(std::string line,int ** &Predicates,int * Arrays);
void findPredicates(std::string line,int ** &Predicates,int pos);
void getSELECT(std::string line,int ** &ArraysResults,int * Arrays);
void findArraysResults(std::string line,int ** &ArraysResults,int pos);
void makePredicates(int arraySize,int ** &Predicates,int * Arrays);
void makeArraysResults(int arraySize,int ** &ArraysResults,int * Arrays);



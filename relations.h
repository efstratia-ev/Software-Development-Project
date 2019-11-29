#ifndef PROJECT_RELATIONS_H
#define PROJECT_RELATIONS_H
#include <fstream>
#include "relation.h"
#include "string.h"

using namespace std;

char* concat(const char *s1, const char *s2)
{
    char *result = (char *)malloc(strlen(s1) + strlen(s2) + 1); 
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


class Relations {
    Relation **rels;
    int sz;
    public:
    Relations(char *filename) {
        ifstream infile(filename); 
        string line;
        int count = 0;
        while (getline(infile,line)) {
            count++;
        }
        sz = count;
        rels = new Relation*[count];
        ifstream _infile(filename); 
        char *suffix = strrchr(filename,'/');
        char *prefix;
        if (suffix != nullptr) {
            prefix = (char *)malloc((suffix-filename+1) * sizeof(char));
            strncpy(prefix,filename,suffix-filename+1);
        }
        const char *fname;
        count = 0;
        while (getline(_infile,line)) {
            if (suffix != nullptr) 
                fname = concat(prefix,line.c_str());
            else 
                fname = line.c_str();
            rels[count] = new Relation(fname);
            count++;
        }
    }

    Relation *relation(int i) { return rels[i]; }
    int getSize() { return sz; }
};


#endif
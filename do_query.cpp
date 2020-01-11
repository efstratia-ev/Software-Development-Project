#include "do_query.h"

//check array if they have been filtered
int isRelationFiltered(JoinArray **filtered, int max, int relation) {
    for (int j = 0; j < max; j++) {
        if (filtered[j]->exists(relation)) {
            return j;
        }
    }
    return -1;
}


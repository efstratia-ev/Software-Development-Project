#ifndef OMG
#define OMG
#include "sort.h"
#include "JoinArray.h"
#include "SQL.h"
#include "relations.h"
#include <cstdint>

uint64_t *DoQuery(SQL *sql, Relations *relations);

#endif
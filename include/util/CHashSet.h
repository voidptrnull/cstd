#ifndef CSTD_CHASHSET_H
#define CSTD_CHASHSET_H

#include "CResult.h"
#include "../ops/Operators.h"

/// \brief Error code indicating that the hash set already has a particular value.
/// \details This code is returned when a particular value already exists in a set, and hence can not be added.
#define CHASHSET_VALUE_PRESENT -3

/// \brief Error code indicating that the hash set pointer is null.
/// \details This code is returned when a function receives a NULL pointer for the hash set parameter.
#define CHASHSET_NULL_HASHSET -2

/// \brief Error code indicating that the index is out of bounds or that value is not present.
/// \details This code is returned when an operation attempts to access or delete an element at an invalid index.
#define CHASHSET_INDEX_OUT_OF_BOUNDS -1

/// \brief Success code for operations.
/// \details This code is returned when an operation completes successfully.
#define CHASHSET_SUCCESS 0

/// \brief Error code indicating a memory allocation failure.
/// \details This code is returned when a memory allocation (malloc or realloc) fails.
#define CHASHSET_ALLOC_FAILURE 1

/// \struct CHashSet
typedef struct CHashSet CHashSet;

CResult* CHashSet_create(size_t capacity, Hash hash);
int CHashSet_init(CHashSet* set, size_t capacity, Hash hash);
int CHashSet_clear(CHashSet* set);
int CHashSet_add(CHashSet* set, void* value);
int CHashSet_remove(CHashSet* set, void* value);
int CHashSet_contains(CHashSet* set, void* value);
CResult* CHashSet_get(CHashSet* set, size_t key);
int CHashSet_free(CHashSet* set);

#endif

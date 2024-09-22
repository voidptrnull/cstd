#ifndef VECTOR_H
#define VECTOR_H

/* Work around for having a undefined type yet using it for pointer
 * dereferencing. */

#include <util/CVector.h>

struct CVector {
    void **data;        ///< Array to store data.
    size_t size;        ///< Number of elements in the vector.
    size_t capacity;    ///< Capacity of the vector.
    Destructor destroy; ///< Function pointer to the destructor for cleaning up
                        ///< individual elements.
};

#endif
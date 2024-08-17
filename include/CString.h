#ifndef C_UTILS_CSTRING_H
#define C_UTILS_CSTRING_H

#include "CVector.h"

/// \brief Default initial capacity for the character vector.
/// \details The default size of the character vector when it is first initialized.
///          The default value is 32.
#define CSTRING_DEFAULT_ALLOC_SIZE 32

#define CSTRING_OP_FAILURE -3

/// \brief Error code indicating that the character vector pointer is null.
/// \details This code is returned when a function receives a NULL pointer for the vector parameter.
#define CSTRING_NULL_STRING -2

/// \brief Error code indicating that the index is out of bounds.
/// \details This code is returned when an operation attempts to access or delete a character at an invalid index.
#define CSTRING_INDEX_OUT_OF_BOUNDS -1

/// \brief Success code for operations.
/// \details This code is returned when an operation completes successfully.
#define CSTRING_SUCCESS 0

/// \brief Error code indicating a memory allocation failure.
/// \details This code is returned when a memory allocation (malloc or realloc) fails.
#define CSTRING_ALLOC_FAILURE 1

/// \brief Structure representing a string.
/// \details The `CString` structure uses a dynamic array to store characters. It supports various
///          operations such as initialization, setting, retrieving, appending, copying, and clearing.
typedef struct CString {
    CVector* characters; ///< Vector storing the characters of the string.
} CString;

/// \brief Create a new CString object.
/// \return Returns a pointer to a newly created `CString` structure, encapsulated within CResult.
CResult* CString_new();

/// \brief Initialize the CString object with a specified initial capacity.
/// \param string Pointer to the `CString` structure to be initialized.
/// \param size Initial capacity of the vector to be used.
/// \return Returns `CSTRING_SUCCESS` on success, or an error code if initialization fails.
int CString_init(CString* string, size_t size);

/// \brief Set the contents of the CString object.
/// \param string Pointer to the `CString` structure.
/// \param str Pointer to the string to set.
/// \return Returns `CSTRING_SUCCESS` on success, or an error code if the operation fails.
int CString_set(CString* string, const char* str);

/// \brief Get a character at a specific index in the CString object.
/// \param string Pointer to the `CString` structure.
/// \param index Index of the character to retrieve.
/// \return Returns the character at the specified index, or an error code if the index is out of bounds.
char CString_at(const CString* string, size_t index);

/// \brief Free the resources used by the CString object.
/// \param string Pointer to the `CString` structure to be freed.
/// \return Returns `CSTRING_SUCCESS` on success, or an error code if the freeing operation fails.
///
/// This function releases the memory allocated for the CString's internal data and the CString itself.
/// The `CString` structure itself is not freed by this function, so it should be done by the caller if needed.
int CString_free(CString* string);

/// \brief Get the length of the CString object.
/// \param string Pointer to the `CString` structure.
/// \return Returns the length of the string, or an error code if the string is null.
size_t CString_length(const CString* string);

/// \brief Append a string to the end of the CString object.
/// \param string Pointer to the `CString` structure.
/// \param str Pointer to the string to be appended.
/// \return Returns `CSTRING_SUCCESS` on success, or an error code if the operation fails.
///
/// This function adds the contents of the given string to the end of the existing CString. It handles dynamic resizing if necessary.
int CString_append_c(CString* string, const char* str);

/// \brief Append a string to the end of the CString object.
/// \param string Pointer to the `CString` structure.
/// \param str_to_add Pointer to the the `CString` structure, which is to be appended.
/// \return Returns `CSTRING_SUCCESS` on success, or an error code if the operation fails.
///
/// This function adds the contents of the given string to the end of the existing CString. It handles dynamic resizing if necessary.
int CString_append(CString* string, CString* str_to_add);

/// \brief Create a copy of the CString object.
/// \param source Pointer to the source `CString` structure.
/// \return Returns a pointer to a new `CResult` structure, (containing `CString`, which in turn contains the copied data), or `CError` if case of failure.
///
/// This function creates a new `CString` instance with the same content as the source `CString`.
CResult* CString_copy(const CString* source);

/// \brief Clear the contents of the CString object.
/// \param string Pointer to the `CString` structure to be cleared.
/// \return Returns `CSTRING_SUCCESS` on success, or an error code if the clearing operation fails.
///
/// This function removes all characters from the `CString` while retaining the allocated memory for reuse.
int CString_clear(CString* string);

/// \brief Convert the contents of the CString object to a C-Style string.
/// \param string Pointer to the `CString` structure to be converted from.
/// \return Returns `CResult` structure, which has the C-String as `char*` or `CError` structure in case of error.
///
/// This function removes all characters from the `CString` while retaining the allocated memory for reuse.
/// Note: The result string is dynamically allocated so it is necessary to free it to avoid memory leaks.
CResult* CString_c_str(CString* string);

/// \brief Check if two `CString` objects are equal.
/// \param str1 Pointer to the first `CString` object.
/// \param str2 Pointer to the second `CString` object.
/// \return Returns 1 if they are equal, 0 otherwise.
int CString_equals(CString* str1, CString* str2); 

#endif // C_UTILS_CSTRING_H

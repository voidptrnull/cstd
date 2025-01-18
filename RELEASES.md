# RELEASES

**VERSION:** `1.0.0 (2024/12)`

## Addition:
- CError - Structure for dealing with data.
- CHashMap - Hashtable implementation.
- CHashSet - Hashset implementation.
- CHRTime - Provide easy access to High Resolution Time.
- CLog - Provide an efficient, easy to use and light weight logging solution.
- CResult - An encapsulation structure which provides a standardized way for accessing library routine results.
- CStack - Stack implementation.
- CString - A standardized, easy to use, string implementation.
- CVector - Dynamic array implementation.

## Modification:
None

## Removal/Deprecation:
None

**VERSION:** `1.0.1 (2024/12)`

## Addition:
- CLinkedList - Provide a implementation for singly/doubly linked lists, where the user can specify which to use.

## Modification:
- Replaced int64_t of co-routines where signed integers are not required. This includes changes in CHashMap, CHashSet, CString, CVector, Operators. This *MAY* **BREAK** ABI.

## Removal/Deprecation:
None

**VERSION:** `1.0.2 (2025/01)`

## Addition:
- CQueue - Provide a implementation for queues, where the user can specify which to use.

## Modification:
- CError - Added a modify function.
- CResult - Added two modify functions for both types of result outcomes.
- CVector - Added a get (fget) method that returns the pointer directly, thus avoiding the issue of multiple allocations.

## Removal/Deprecation:
None

**VERSION:** `1.0.3 (2025/01)`

## Addition:
- CHeap - Provide a implementation for min-max heaps.

## Modification:
- CError - Added C++ namespace comptability.
- CResult - Added C++ namespace comptability. Reverted from uint64_t to size_t for platform support.
- CHashMap - Added C++ namespace comptability. Reverted from uint64_t to size_t for platform support.
- CHashSet - Added C++ namespace comptability. Reverted from uint64_t to size_t for platform support.
- CStack - Added C++ namespace compatibility.
- CQueue - Added C++ namespace comptability.
- CHRTime - Added C++ namespace compatibility.
- CLog - Added C++ namespace comptability. Reverted from uint64_t to size_t for platform support.
- CLinkedList - Added C++ namespace comptability. Reverted from uint64_t to size_t for platform support.
- CString - Added C++ namespace comptability. Reverted from uint64_t to size_t for platform support.
- CVector - Added C++ namespace comptability. Reverted from uint64_t to size_t for platform support.
- Operators - Added C++ namespace comptability. Reverted from uint64_t to size_t for platform support.

## Removal/Deprecation:
None
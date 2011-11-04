/**
 * @brief Memory management
 *
 * File file implements memory management and memory functions.
 *
 * Simple memory functions are memcpy, memset, etc.
 * All functions are modified to be NULL pointer safe.
 *
 * Memory management contains*
 * @li Allocation and freeing memory
 * @li Memory tracking
 * @li Bounds checking using guardian bytes
 *
 * @date $Date: 2010-10-06 10:41:15 +0200 (Wed, 06 Oct 2010) $
 * @author Andreas Loew
 * @file
 *
 * Copyright by SVOX Deutschland GmbH
 *
 * Transmittal, reproduction, dissemination and/or editing of this
 * source code as well as utilization of its contents and communication
 * thereof to others without express authorization are prohibited.
 * Offenders will be held liable for payment of damages. All rights
 * created by patent grant or registration of a utility model or design
 * patent are reserved.
 */


#ifndef __SVOX_MEMORY_H__
#define __SVOX_MEMORY_H__

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------------*/

#include "SVOX_Types.h"

/**
 * Allocated a block of memory.
 *
 * The standard implementation uses
 * malloc.
 *
 * For debugging enhanced memory checking may be enabled.
 * Every block allocated using SVOX_malloc is filled with a
 * special sequence to detect use of uninitialized memory.
 * At the end of the block some additional guardian bytes
 * are added to detect out of bounds writes.
 *
 * @param[in] size of memory to allocate
 *
 * @retval 0 in case of error
 * @retval pointer in case allocation was ok
 */
#ifdef SVOX_PAL_DEBUG_MEMORY_USE_SOURCE_LOCATIONS
void *SVOX_malloc_
(
    SVOX_size_t      size,
    const SVOX_Char *file,
    SVOX_32S         line
);

#define SVOX_malloc(_SIZE) SVOX_malloc_(_SIZE, __FILE__, __LINE__)
#else
void *SVOX_malloc
(
    SVOX_size_t size
);

#endif

/**
 * Frees a block of memory.
 *
 * For debugging enhanced memory checking may be enabled.
 * Every block freed using SVOX_free is filled with a
 * special sequence to detect dangling pointers.
 * The guardian bytes are checked at the end of the block.
 *
 * The standard implementation uses
 * free but is stable against NULL-Pointers.
 *
 * @param[in] memblock Block of memory to free,
 *            if a NULL-pointer is entered
 *            nothing happens.
 *
 */
#ifdef SVOX_PAL_DEBUG_MEMORY_USE_SOURCE_LOCATIONS
void SVOX_free_
(
    void            *memblock,
    const SVOX_Char *filename,
    SVOX_32S         line
);

#define SVOX_free(_PTR) SVOX_free_(_PTR, __FILE__, __LINE__)
#else
void SVOX_free
(
    void *memblock
);

#endif

/**
 * Allocates a block of memory and sets it's contents
 * to 0.
 * The implementation uses SVOX_mallocZero.
 *
 * @param[in] num number of blocks to allocate
 * @param[in] size size of a single block
 *
 * @retval 0 in case of error
 * @retval pointer in case allocation was ok
 */
#ifdef SVOX_PAL_DEBUG_MEMORY_USE_SOURCE_LOCATIONS
void *SVOX_calloc_
(
    SVOX_size_t      num,
    SVOX_size_t      size,
    const SVOX_Char *filename,
    SVOX_32S         line
);

#define SVOX_calloc(_NUM, _SIZE) SVOX_calloc_(_NUM, _SIZE, __FILE__, __LINE__)
#else
void *SVOX_calloc
(
    SVOX_size_t num,
    SVOX_size_t size
);

#endif

/**
 * Resizes a block of memory and copies the data to the
 * new block.
 *
 * @warning The pointer returned may be diffrent
 * from the pointer given to the operating function.
 *
 * @warning If the function returns NULL the original
 * block of memory is still valid and not freed!
 * You are responsible to keep the pointer and check
 * the result of the function for NULL. If the result
 * is diffrent from NULL the original pointer is freed.
 *
 * @param[in] memory Pointer to the block of memory to resize.
 *                   If the pointer is NULL SVOX_realloc behaves
 *                   like SVOX_malloc
 * @param[in] newSize the new size for the block. If the size is 0
 *                 SVOX_realloc behaves like SVOX_free and
 *                 returns a NULL pointer.
 * @param[in] oldSize size of the "old" memory block
 *
 * @retval NULL if the memory block could not be resized.
 *         The original block is still valid and not freed.
 * @retval NOT_NULL if the allocation was ok, the original
 *         memory pointer is no longer valid.
 *
 * @code
 * void *memory = SVOX_malloc(100);
 * void *resized = SVOX_realloc(memory, 200);
 * if(NULL == resized)
 * {
 *     // memory is still valid here !!!
 *     SVOX_Free(memory);
 *     return;
 * }
 * // memory is not valid anymore
 * // resized is the new pointer now
 * @endcode
 *
 * Uses operating system's realloc if possible,
 * otherwise uses SVOX_malloc + SVOX_memcpy
 * to implement the behaviour.
 */
#ifdef SVOX_PAL_DEBUG_MEMORY_USE_SOURCE_LOCATIONS
void *SVOX_realloc_
(
    void            *memory,
    SVOX_size_t      oldSize,
    SVOX_size_t      newSize,
    const SVOX_Char *filename,
    SVOX_32S         line
);

#define SVOX_realloc(_OLD_, _OLDSIZE, _NEWSIZE) SVOX_realloc_(_OLD_, _OLDSIZE, _NEWSIZE, __FILE__, __LINE__)
#else
void *SVOX_realloc
(
    void       *memory,
    SVOX_size_t oldSize,
    SVOX_size_t newSize
);

#endif

/**
 * Returns the currenly used memory and the maximum memory.
 * This function uses the global memory tracker.
 *
 * @param[in] currentUsagePtr Variable in which the current usage is written
 * @param[in] maxUsagePtr Variable in which the maximum usage is written
 *
 * @retval SVOX_RESULT_OK ok
 * @retval SVOX_RESULT_INVALID_ARG_1 currentUsagePtr is NULL
 * @retval SVOX_RESULT_INVALID_ARG_1 maxUsagePtr is NULL
 * @retval SVOX_RESULT_ERROR_NOT_RUNNING Memory tracking is not active
 */
SVOX_Result SVOX_Memory_GetUsage
(
    SVOX_32S *currentUsagePtr,
    SVOX_32S *maxUsagePtr
);


/**
 * Resets the max usage counter to the current usage
 */
void SVOX_Memory_ResetMaxUsage();


/**
 * Prints a list of all memory blocks allocated
 *
 * This only works if memory SVOX_PAL_DEBUG_MEMORY_USE_LIST is set.
 * Otherwise the current usage is printed if
 * SVOX_PAL_DEBUG_MEMORY_USE_COUNTING is set.
 */
void SVOX_Memory_PrintList();


/**
 * Instructs the memory mgmt to reject all further allocation after
 * the next \a count-1 allocations. All allocations are counted:
 * SVOX_malloc, SVOX_mallocZero, SVOX_calloc, SVOX_realloc (in the
 * case of realloc, it does not matter whether the allocation was
 * really done or not). The number of allocations is counted, not the
 * size.
 * 
 * This function does nothing unless
 * SVOX_PAL_DEBUG_MEMORY_USE_FAILURE_COUNTS is set.
 *
 * If you want to implement thread specific counting, you can use the
 * callback function to implement a check against the desired thread.
 *
 * Note: Of course allocations can fail earlier if there is a real
 * allocation problem!
 *
 * @param [in] count: the number of the allocation that should fail,
 * Use 1 for the next allocation to fail. Use zero to rely fully on
 * the result of the callback.
 *
 * @param [in] threadCb: function pointer to a function returning
 * SVOX_Bool. This can be used to implement thread specific counting,
 * or other "special triggers". A result of SVOX_TRUE means the
 * failure should be triggered (or the count of remaining allocations
 * shoudl be reduced). If a NULL pointer is passed, only \a count is
 * considered.
 *
 * Note: To return to normal memory allocation operation during
 * runtime use:
 *
 * @code
 * SVOX_Memory_RequestAllocFail( 0, NULL );
 * @endcode
 */
void SVOX_Memory_RequestAllocFail
(
    SVOX_32S count,
    SVOX_Bool (*threadCb)()
);
    
/**
 * Allocates a block of memory and sets it's contents
 * to 0.
 *
 * The implementation uses SVOX_malloc and
 * SVOX_memset to flush the block.
 *
 * @param[in] size size of a single block
 *
 * @retval NULL in case of error
 * @retval NOT_NULL in case allocation was ok
 */
#ifdef SVOX_PAL_DEBUG_MEMORY_USE_SOURCE_LOCATIONS
void *SVOX_mallocZero_
(
    SVOX_size_t      size,
    const SVOX_Char *filename,
    SVOX_32S         line
);

#define SVOX_mallocZero(_SIZE) SVOX_mallocZero_(_SIZE, __FILE__, __LINE__)
#else
void *SVOX_mallocZero
(
    SVOX_size_t size
);

#endif

/**
 * Copies the values of num bytes from the location pointed by source to the
 * memory block pointed by destination.
 * Copying takes place as if an intermediate buffer was used, allowing the
 * destination and source to overlap.
 *
 * The underlying type of the objects pointed by both the source and
 * destination pointers are irrelevant for this function;
 * The result is a binary copy of the data.
 *
 * The function does not check for any terminating null character in source
 * it always copies exactly num bytes.
 *
 * To avoid overflows, the size of the arrays pointed by both the
 * destination and source parameters, shall be at least num bytes.
 *
 * Implementation checks for NULL pointers in destination and source.
 *
 * @param[in] destination destination to copy the memory to
 * @param[in] source the source for the memory block
 * @param[in] num number of bytes to copy
 *
 * @return pointer to destination
 */
void *SVOX_memmove
(
    void       *destination,
    const void *source,
    SVOX_size_t num
);

/**
 * Copies the values of num bytes from the location pointed by source
 * directly to the memory block pointed by destination.
 * The underlying type of the objects pointed by both the source and
 * destination pointers are irrelevant for this function; The result
 * is a binary copy of the data.
 * The function does not check for any terminating null character in
 * source - it always copies exactly num bytes.
 *
 * @warning: To avoid overflows, the size of the arrays pointed by both the
 * destination and source parameters, shall be at least num bytes, and
 * should not overlap (for overlapping memory blocks,
 * memmove is a safer approach).
 *
 * @see SVOX_memmove
 *
 * @param[in] destination destination to copy the memory to
 * @param[in] source the source for the memory block
 * @param[in] num number of bytes to copy
 *
 * @return pointer to destination
 */
void *SVOX_memcpy
(
    void       *destination,
    const void *source,
    SVOX_size_t num
);

/**
 * This function compares byte array s1 against byte array s2.
 * Both arrays are assumed to be n bytes long.
 *
 * @param[in] s1 reference block of memory to compare
 * @param[in] s2 the source for the memory block to compare against the reference
 * @param[in] num number of bytes to compare
 *
 * @return The function returns zero if the two strings are identical,
 *         1 if s1 > s2,
 *         -1 if s1 < s2
 */
SVOX_32S SVOX_memcmp
(
    const void *s1,
    const void *s2,
    SVOX_size_t num
);


/**
 * Sets the first num bytes of the block of memory pointed
 * by ptr to the specified value (interpreted as an unsigned char).
 *
 * @param[in] ptr Pointer to the block of memory to fill.
 * @param[in] c Character to fill the memory block with
 * @param[in] length Number of bytes to fill
 *
 * @return ptr
 */
void *SVOX_memset
(
    void       *ptr,
    SVOX_8U     c,
    SVOX_size_t length
);

/**
 * Searches within the first num bytes of the block of memory pointed
 * by ptr for the first occurrence of value and returns a pointer to it.
 *
 * @param[in] ptr memory to search
 * @param[in] value to search
 * @param[in] num number of bytes
 *
 * @return pointer to the first occurence of value if found,
 * NULL if not found.
 */
void *SVOX_memchr
(
    void *ptr,
    SVOX_8U     value,
    SVOX_size_t num
);


/* -----------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif


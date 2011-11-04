/**
 * @brief Atomic functions
 *
 * This file contains functions for thread safe access to
 * variables without using mutexes etc.
 *
 * Basic functions allow setting, incrementing and testing
 * integer variables and pointers.
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

#ifndef __SVOX_ATOMIC_H__
#define __SVOX_ATOMIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __SVOX_PLATFORM_H__
#ifdef SVOX_PAL_IMPLEMENTATION_FILE
#include "SVOX_Platform.h"
#define SVOX_PLATFORM_CLEANUP_DEFINES
#else
#define SVOX_PAL_IMPLEMENTATION_FILE
#include "SVOX_Platform.h"
#define SVOX_PLATFORM_CLEANUP_DEFINES
#undef SVOX_PAL_IMPLEMENTATION_FILE
#endif
#endif

#if (SVOX_PLATFORM_THREADING == SVOX_PLATFORM_THREADING_NONE) && !defined(SVOX_PAL_ENABLE_DUMMY_ATOMIC) \
    && !defined(SVOX_PAL_IMPLEMENTATION_FILE)
#error \
    "Trying to use SVOX_Atomic on a platform which does not support threading. Set SVOX_PAL_ENABLE_DUMMY_ATOMIC to enable using dummy mutexes."
#endif

#include "SVOX_Types.h"

/* -----------------------------------------------------------------------*/

/**
 * @brief Atomic test and set
 *
 * Test is content of ptr is expected then set newValue.
 *
 * Memory semantics are unspecified for this function. To protect
 * memory blocks by the atomic value use the SVOX_Atomic_TestAndSet_Aquire_32S
 * and SVOX_Atomic_TestAndSet_Release_32S.
 *
 * @retval SVOX_TRUE on success
 * @retval SVOX_FALSE on failure
 */
SVOX_Bool SVOX_Atomic_TestAndSet_32S
(
    volatile SVOX_32S *ptr,
    SVOX_32S           expected,
    SVOX_32S           newValue
);

/**
 * @brief Atomic test and set for SVOX_32S with memory barrier (acquire)
 *
 * Test is content of ptr is expected then set newValue.
 *
 * Memory operations following the atomic operation may not be
 * reordered before the atomic operation.
 *
 * @remark On some systems this may result in a full
 * memory barrier.
 *
 * @retval SVOX_TRUE on success
 * @retval SVOX_FALSE on failure
 */
SVOX_Bool SVOX_Atomic_TestAndSet_Acquire_32S
(
    volatile SVOX_32S *ptr,
    SVOX_32S           expected,
    SVOX_32S           newValue
);

/**
 * @brief Atomic test and set for SVOX_32S with memory barrier (release)
 *
 * Test is content of ptr is expected then set newValue.
 *
 * Memory operations before the atomic operation may not be
 * reordered after the atomic operation.
 *
 * @remark On some systems this may result in a full
 * memory barrier.
 *
 * @retval SVOX_TRUE on success
 * @retval SVOX_FALSE on failure
 */
SVOX_Bool SVOX_Atomic_TestAndSet_Release_32S
(
    volatile SVOX_32S *ptr,
    SVOX_32S           expected,
    SVOX_32S           newValue
);

/**
 * @brief Atomic test and set for pointers
 *
 * Test is content of ptr is expected then set newValue.
 *
 * Memory semantics are unspecified for this function.
 *
 * @retval SVOX_TRUE on success
 * @retval SVOX_FALSE on failure
 */
SVOX_Bool SVOX_Atomic_TestAndSet_Ptr
(
    void *volatile *ptr,
    void           *expected,
    void           *newValue
);

/**
 * @brief Atomic increment
 *
 * Add 1 to the memory location.
 *
 * @retval SVOX_FALSE if the value of ptr is 0
 * @retval SVOX_TRUE if the value of ptr is not 0
 */
SVOX_Bool SVOX_Atomic_Increment
(
    volatile SVOX_32S *ptr
);

/**
 * @brief Atomic decrement
 *
 * Subtract 1 to the memory location.
 *
 * @retval SVOX_FALSE if the value of ptr is 0
 * @retval SVOX_TRUE if the value of ptr is not 0
 */
SVOX_Bool SVOX_Atomic_Decrement
(
    volatile SVOX_32S *ptr
);

/**
 * @brief Fetch and add SVOX_32S
 *
 * Adds value to the memory location, returns the new value.
 *
 * Memory semantics are unspecified for this function. To protect
 * memory blocks by the atomic value use the SVOX_Atomic_FetchAndAdd_Acquire_32S
 * and SVOX_Atomic_FetchAndAdd_Release_32S.
 *
 * @retval The content of ptr after adding value
 */
SVOX_32S SVOX_Atomic_Add_32S
(
    volatile SVOX_32S *ptr,
    SVOX_32S           value
);

/**
 * @brief Fetch and add SVOX_32S with memory barrier (acquire)
 *
 * Adds value to the memory location, returns the new value.
 *
 * Memory operations following the atomic operation may not be
 * reordered before the atomic operation.
 *
 * @remark On some systems this may result in a full
 * memory barrier.
 *
 * @retval The content of ptr after adding value
 */
SVOX_32S SVOX_Atomic_Add_Acquire_32S
(
    volatile SVOX_32S *ptr,
    SVOX_32S           value
);

/**
 * @brief Fetch and add SVOX_32S with memory barrier (release)
 *
 * Adds value to the memory location, returns the new value.
 *
 * Memory operations before the atomic operation may not be
 * reordered after the atomic operation.
 *
 * @remark On some systems this may result in a full
 * memory barrier.
 *
 * @retval The content of ptr after adding value
 */
SVOX_32S SVOX_Atomic_Add_Release_32S
(
    volatile SVOX_32S *ptr,
    SVOX_32S           value
);

/**
 * @brief Returns the value of a atomic pointer variable
 *
 * Returns the value of the variable
 *
 * @retval the value, NULL if pointer is NULL
 */
void *SVOX_Atomic_Get_Ptr
(
    void *const volatile *ptr
);

/**
 * @brief Returns the value of a atomic integer variable
 *
 * Returns the value of the variable
 *
 * @retval the value, 0 if pointer is NULL
 */
SVOX_32S SVOX_Atomic_Get_32S
(
    volatile const SVOX_32S *ptr
);

/**
 * @brief Sets the value of a pointer
 *
 * @param[in] ptr pointer to set
 * @param[in] data data to set
 *
 */
void SVOX_Atomic_Set_Ptr
(
    void *volatile *ptr,
    void           *data
);

/**
 * @brief Sets an integer pointer
 *
 * @param[in] ptr pointer to set
 * @param[in] data data to set
 *
 */
void SVOX_Atomic_Set_32S
(
    volatile SVOX_32S *ptr,
    SVOX_32S           data
);

#ifdef SVOX_PLATFORM_CLEANUP_DEFINES
#include "SVOX_Platform.h"
#undef SVOX_PLATFORM_CLEANUP_DEFINES
#endif

#ifdef __cplusplus
}
#endif

#endif


/**
 * @brief Mutex
 *
 * Mutex implementation.
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

#ifndef __SVOX_MUTEX_H__
#define __SVOX_MUTEX_H__

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------------*/

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

#if (SVOX_PLATFORM_THREADING == SVOX_PLATFORM_THREADING_NONE) && !defined(SVOX_PAL_ENABLE_DUMMY_MUTEXES) \
    && !defined(SVOX_PAL_IMPLEMENTATION_FILE)
#error \
    "Trying to use SVOX_Mutex on a platform which does not support threading. Set SVOX_PAL_ENABLE_DUMMY_MUTEXES to enable using dummy mutexes."
#endif

#include "SVOX_Types.h"

/**
 * Internal struct to keep mutex data. This is hidden in the
 * interface and must not be used directly.
 * The implementation of the struct varies for specific
 * platforms.
 */
struct SVOX_Mutex_;

/**
 * Handle which keeps the mutex data.
 */
typedef struct SVOX_Mutex_ SVOX_Mutex;

typedef enum
{
    SVOX_MUTEX_TYPE_RECURSIVE,
    SVOX_MUTEX_TYPE_FAST
} SVOX_Mutex_Type;

/**
 * Opens a new mutex. The mutex can either be a fast mutex which
 * creates a deadlock in recursive calls or a
 * recursive mutex which can be locked multiple times from the
 * same thread.
 *
 * If you disable threading on the current platform you can
 * replace the mutex by a dummy implementation. In this case this
 * function sets mutex to a value != NULL and returns SVOX_RESULT_OK.
 *
 * @ingroup threading
 *
 * @param[out] mutex Pointer to a SVOX_Mutex in which the handle for the
 *                   mutex is written.
 *                   If initialisation of the mutex failes this is set to NULL
 * @param[in] mutexType If set to SVOX_MUTEX_TYPE_FAST this is a fast mutex which blocks
 *                 in the case of recursive locking from one thread.
 *                 If set to SVOX_MUTEX_TYPE_RECURSIVE the mutex can be locked
 *                 recursively from one thread without blocking.
 * @retval SVOX_RESULT_OK Mutex opened.
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 The pointer to the SVOX_Mutex was NULL
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 The mutex type is invalid
 * @retval SVOX_RESULT_ERROR_NO_MEMORY Not enougl memory
 * @retval SVOX_RESULT_ERROR_OPEN Error opening the mutex.
 */
SVOX_Result SVOX_Mutex_Create
(
    SVOX_Mutex    **mutex,
    SVOX_Mutex_Type mutexType
);

/**
 * Close the mutex. The mutex must not be used after closing it.
 *
 * If you disable threading on the current platform you can
 * replace the mutex by a dummy implementation. In this case this
 * function is replaced by the return value SVOX_RESULT_OK.
 *
 * @warning: Make sure that no other thread is locked on the mutex
 * while destroying it. Depending on the platform the thread
 * may not wake up.
 *
 * @param[in] mutex Mutex handle.
 * @retval SVOX_RESULT_OK Mutex was closed.
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 mutex was NULL
 * @retval SVOX_RESULT_ERROR_FATAL The mutex could not be closed (perhaps it was already closed?)
 */
SVOX_Result SVOX_Mutex_Destroy
(
    SVOX_Mutex *mutex
);

/**
 * Lock the mutex. If the mutex is currently locked by another thread this function
 * waits for the release of the mutex.
 * Depending on the initialisation of the mutex the behaviour varies:
 * A recursive mutex may be locked several times from the same thread. It
 * must be unlocked in the same amount to release it for other threads.
 * A fast mutex will create a deadlock if locked twice from the
 * same thread.
 *
 * If you disable threading on the current platform you can
 * replace the mutex by a dummy implementation. In this case this
 * function is replaced by the return value SVOX_RESULT_OK.
 *
 * @ingroup threading
 *
 * @param[in] mutex Mutex handle.
 *
 * @retval SVOX_RESULT_OK Mutex was locked.
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 mutex was NULL
 * @retval SVOX_RESULT_ERROR_FATAL The mutex could not be locked.
 */
SVOX_Result SVOX_Mutex_Lock
(
    SVOX_Mutex *mutex
);

/**
 * Lock the mutex. If the mutex is currently locked by another thread this function
 * returns with SVOX_MUTEX_ALREADY_LOCKED.
 *
 * Depending on the initialisation of the mutex the behaviour varies:
 * A recursive mutex may be locked several times from the same thread. It
 * must be unlocked in the same amount to release it for other threads.
 * A fast mutex will create a deadlock if locked twice from the
 * same thread.
 *
 * If you disable threading on the current platform you can
 * replace the mutex by a dummy implementation. In this case this
 * function is replaced by the return value SVOX_RESULT_OK.
 *
 * @ingroup threading
 *
 * @param[in] mutex Mutex handle.
 *
 * @retval SVOX_RESULT_OK Mutex was locked.
 * @retval SVOX_MUTEX_ALREADY_LOCKED The mutex is already locked by another thread
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 mutex was NULL
 * @retval SVOX_RESULT_ERROR_FATAL The mutex could not be locked.
 */
SVOX_Result SVOX_Mutex_TryLock
(
    SVOX_Mutex *mutex
);

/**
 * Unlock the mutex. If the mutex is locked recursively by one thread this
 * decrements the lock count. If the lock count is 0 the mutex is released.
 *
 * If you disable threading on the current platform you can
 * replace the mutex by a dummy implementation. In this case this
 * function is replaced by the return value SVOX_RESULT_OK.
 *
 * @ingroup threading
 *
 * @param[in] mutex Mutex handle.
 *
 * @retval SVOX_RESULT_OK Mutex was locked.
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 mutex was NULL
 * @retval SVOX_RESULT_ERROR_FATAL The mutex could not be locked.
 */
SVOX_Result SVOX_Mutex_Unlock
(
    SVOX_Mutex *mutex
);

/* -----------------------------------------------------------------------*/
#ifdef SVOX_PLATFORM_CLEANUP_DEFINES
#include "SVOX_Platform.h"
#undef SVOX_PLATFORM_CLEANUP_DEFINES
#endif

#ifdef __cplusplus
}
#endif

#endif


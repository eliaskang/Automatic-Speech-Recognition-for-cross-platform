/**
 * @brief Semaphore implementation
 *
 * This file contains the functions for platform independent
 * semaphores.
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

#ifndef __SVOX_SEMAPHORE_H__
#define __SVOX_SEMAPHORE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------------*/

#include "SVOX_Types.h"

/**
 * Internal struct to keep semaphore data. This is hidden in the
 * interface and must not be used directly.
 * The implementation of the struct varies for specific
 * platforms.
 */
struct SVOX_Semaphore_;

/**
 * Handle which keeps the semaphore data.
 */
typedef struct SVOX_Semaphore_ SVOX_Semaphore;

/**
 * Creates a new semaphore.
 * The internal counter of the semaphore is initialised with initCount.
 *
 * @ingroup threading
 *
 * @param[in] initCount Initial counter of the semaphore. The value must
 *                        be greater or equal to zero.
 * @param[in] max Maximum value for the semaphore's counter
 *
 * @param[out] semaPtr Pointer to a SVOX_Semaphore in which the pointer is stored.
 *                 The pointer is set to NULL if opening the semaphore fails.
 *
 * @retval SVOX_RESULT_OK Semaphore initialised without error
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 initCount < 0
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 max < 0
 * @retval SVOX_RESULT_ERROR_OVERFLOW initCount is greater than max
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_3 semaPtr is NULL
 * @retval SVOX_RESULT_ERROR_NO_MEMORY No memory to allocate the semaphore
 * @retval SVOX_RESULT_ERROR_OPEN Semaphore could not be opened
 * @retval SVOX_RESULT_ERROR Initialisation failed, semaphore could not be
 *                          deinitialised either. In this case a
 *                           memory leak occurs!
 */
SVOX_Result SVOX_Semaphore_Open
(
    SVOX_32S         initCount,
    SVOX_32S         max,
    SVOX_Semaphore **semaPtr
);

/**
 * Close the semaphore. After closing the handle is in valid
 * and must not be used anymore.
 *
 * @ingroup threading
 *
 * @param[in] semaphore SVOX_Semaphore-Handle
 *
 * @retval SVOX_RESULT_OK Semaphore initialised without error
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 semaphore is invalid
 * @retval SVOX_RESULT_ERROR semaphore failed to close (perhaps already closed?).
 *							 In this case the memory for the semaphore is not
 *                           released!
 */
SVOX_Result SVOX_Semaphore_Close
(
    SVOX_Semaphore *semaphore
);

/**
 * Signal the semaphore. This increments the internal count. A waiting
 * thread wakes up on this.
 *
 * @ingroup threading
 *
 * @param[in] semaphore SVOX_Semaphore-Handle
 * @retval SVOX_RESULT_OK Semaphore initialised without error
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 semaphore is invalid
 * @retval SVOX_RESULT_ERROR_OVERFLOW The internal count of the semaphore reached the
 *                                 maximum value.
 * @retval SVOX_RESULT_ERROR_FATAL A fatal error occured while signaling
 *                                 the semaphore. System may be unstable now!
 */
SVOX_Result SVOX_Semaphore_Signal
(
    SVOX_Semaphore *semaphore
);

/**
 * Wait for the semaphore. This waits until the counter of the semaphore is
 * at least 1. If the counter is geater or equal 1 the counter is
 * decremented and SVOX_Semaphore_Wait returns.
 *
 * @ingroup threading
 *
 * @param[in] semaphore SVOX_Semaphore-Handle
 * @retval SVOX_RESULT_OK Semaphore initialised without error
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 semaphore is invalid
 * @retval SVOX_RESULT_ERROR_FATAL A fatal error occured while signaling
 *                                 the semaphore. System may be unstable now!
 */
SVOX_Result SVOX_Semaphore_Wait
(
    SVOX_Semaphore *semaphore
);

/**
 * Wait for the semaphore with timeout. This waits until the counter of the semaphore is
 * at least 1. If the counter is greater or equal 1 the counter is
 * decremented and SVOX_Semaphore_Wait returns.
 * If the given time is over without signaling the semaphore
 * the function returns with SVOX_RESULT_TIMEOUT.
 *
 * @ingroup threading
 *
 * @param[in] semaphore SVOX_Semaphore-Handle
 * @param[in] milliseconds requested timeout
 *
 * @retval SVOX_RESULT_OK Semaphore initialized without error
 * @retval SVOX_RESULT_TIMEOUT timeout occurred
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 semaphore is invalid
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 time value is negative
 * @retval SVOX_RESULT_ERROR_FATAL A fatal error occurred while signaling
 *                                 the semaphore. System may be unstable now!
 */
SVOX_Result SVOX_Semaphore_TryWait
(
    SVOX_Semaphore *semaphore,
    SVOX_32S        milliseconds
);

/* -----------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif



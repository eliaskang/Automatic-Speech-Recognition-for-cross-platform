/**
 * @brief Exit functions
 *
 * Everything related to exit functions
 *
 *
 * @date $Date: 2010-06-10 13:29:52 +0200 (Do, 10 Jun 2010) $
 * @author Stefan Sablatnoeg
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

#ifndef __SVOX_EXIT_H__
#define __SVOX_EXIT_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief error code for succesfule program termination
 */
#define SVOX_EXIT_SUCCESS 1

/**
 * @brief error code for program termination due to failures
 */
#define SVOX_EXIT_FAILURE 0



/* -----------------------------------------------------------------------*/

#include "SVOX_Types.h"
/**
 * @brief exit the actual process.
 *
 * Exit the current process with a graceful shutdown of the c-library,
 * including calls to all functions registered with atexit. Of course
 * this function will never return :-) so it has no return value.
 *
 * @param exitcode return value to return from the process
 */
void SVOX_exit(SVOX_32S exitcode);
    
/**
 * @brief register handler function to be called at exit
 *
 * This is usually part of ISO C since 1989, up to 32 handlers are possible.  The
 * functions are called in reverse order of their registration. If
 * functions are registered multiple times, they are called as many
 * times as they are registered.
 *
 * @param func function pointer to handler function with prototype
 * @code{void func(void);}
 * @retval 0 on success
 * @retval nonzero on failure
 */
SVOX_32S SVOX_atexit(void (*func)(void));
    
/* -----------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __SVOX_EXIT_H__ */


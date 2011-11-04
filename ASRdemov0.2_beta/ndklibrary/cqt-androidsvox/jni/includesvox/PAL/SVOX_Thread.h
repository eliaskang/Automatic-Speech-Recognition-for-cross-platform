/**
 * @brief Threads
 *
 * This file contains functions for creating and using
 * threads.
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

#ifndef __SVOX_THREAD_H__
#define __SVOX_THREAD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------------*/

#include "SVOX_Types.h"

/**
 * Internal struct to keep thread data. This is hidden in the
 * interface and must not be used directly.
 * The implementation of the struct varies for specific
 * platforms.
 */
struct SVOX_Thread_;

/**
 * Handle which keeps the thread data.
 */
typedef struct SVOX_Thread_ SVOX_Thread;

/**
 * Thread function pointer
 */
typedef void * (*SVOX_Thread_Function)(void *);


typedef enum
{
    /**
     * Thread is inisialised and ready to run
     */
    SVOX_THREAD_STATE_INITIALISED,

    /**
     * Thread is currently running
     */
    SVOX_THREAD_STATE_RUNNING,

    /**
     * The thread left its function but SVOX_Thread_join
     * was not yet called.
     */
    SVOX_THREAD_STATE_TERMINATING,

    /**
     * Thread is terminated (SVOX_Thread_join was called)
     */
    SVOX_THREAD_STATE_TERMINATED,

    /**
     * An error occured - during starting the thread.
     *
     */
    SVOX_THREAD_STATE_START_ERROR
} SVOX_Thread_State;


typedef enum
{
    /**
     *  Identifies Hook called just before the new thread enters its
     *  run work routine. The context is already the thread context of
     *  the new thread.
     */
    SVOX_THREAD_PRERUN,
    /**
     *  Identifies Hook called just after the new thread left its
     *  run work routine. The context is still the thread context of
     *  the working thread.
     */
    SVOX_THREAD_POSTRUN,
    __SVOX_THREAD_HOOK_SIZE__
} SVOX_Thread_HookType;

/**
 * function pointer type for thread hook
 */
typedef void (*SVOX_Thread_HookFunction)(SVOX_Thread *, void *, SVOX_Thread_HookType);
    

/**
 * Create a thread.
 * The thread is initialised with default values.
 *
 * @param [in] threadName Pointer to the name of the thread. The name is
 *             copied into the thread structure.
 * @param [in] stackSize Stacksize for the thread. May be incremented by the
 *             implementation to the target's memory page size.
 *             Use 0 to use system's default stack size.
 * @param [in] function Pointer to the function to run
 * @param [in] data Pointer to a data structure which is sent to the thread
 *             function. May be NULL.
 *
 * @param [out] threadPtr Pointer to a thread handle in which the thread is returned.
 *
 * @retval SVOX_RESULT_OK Ok.
 * @retval SVOX_RESULT_ERROR_NO_MEMORY Out of memory.
 * @retval SVOX_RESULT_ERROR_THREAD_NO_MORE_AVAILABLE No more thread handles available
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_0 Pointer to the threadName is NULL.
 * @retval SVOX_RESULT_ERROR_INVALID_THREAD_PRIORITY Priority is invalid.
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 Stacksize is invalid.
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_3 Pointer to the function is NULL.
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_4 threadPtr is NULL.
 * @retval SVOX_RESULT_ERROR_FATAL Fatal error. System may be unstable now.
 */
SVOX_Result SVOX_Thread_Create
(
    const SVOX_Char     *threadName,
    SVOX_32U             stackSize,
    SVOX_Thread_Function function,
    void                *data,
    SVOX_Thread        **threadPtr
);

/**
 * Destroy the thread.
 * Waits for the thread to finish before it is destroyed.
 *
 * @param [in] thread Handle to the thread
 *
 * @retval SVOX_RESULT_OK Ok
 * @retval SVOX_RESULT_ERROR_NO_MEMORY Out of memory.
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_0 Handle to the thread is invalid.
 * @retval SVOX_RESULT_ERROR_FATAL Fatal error. System may be unstable now.
 */
SVOX_Result SVOX_Thread_Destroy
(
    SVOX_Thread *thread
);

/**
 * Starts the thread.
 *
 * @param [in] thread Handle to the thread
 *
 * @retval SVOX_RESULT_OK Ok
 * @retval SVOX_RESULT_ERROR_THREAD_INVALID_STATE The thread is not in state
 *         SVOX_THREAD_STATE_INITIALISED
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_0 Handle to the thread is invalid.
 * @retval SVOX_RESULT_ERROR_FATAL Fatal error. System may be unstable now.
 */
SVOX_Result SVOX_Thread_Start
(
    SVOX_Thread *thread
);

/**
 * Waits for the thread to finish.
 *
 * @param [in] thread Handle to the thread
 *
 * @retval SVOX_RESULT_OK Ok
 * @retval SVOX_RESULT_ERROR_THREAD_INVALID_STATE The thread is not in state
 *         SVOX_THREAD_STATE_RUNNING, SVOX_THREAD_STATE_TERMINATING
 *         or SVOX_THREAD_STATE_TERMINATED
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_0 Handle to the thread is invalid.
 * @retval SVOX_RESULT_ERROR_THREAD_CANT_JOIN Thread can not be joined
 * @retval SVOX_RESULT_ERROR_FATAL Fatal error. System may be unstable now.
 */
SVOX_Result SVOX_Thread_Join
(
    SVOX_Thread *thread
);

#if NOT_YET_IMPLEMENTED

/**
 * Set the thread's priority. The values are represented by SVOX_Thread_Prio
 * which is mapped to the target specific values.
 * In future this may be overridden by a configuration file.
 * Priority may be changed at runtime
 *
 * @param [in] thread Handle to the thread
 * @param [in] threadPrio Priority to set for this thread.
 *
 * @retval SVOX_RESULT_OK Ok
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_0 Handle to the thread is invalid.
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 threadPrio is invalid.
 * @retval SVOX_RESULT_ERROR_FATAL Fatal error. System may be unstable now.
 *
 */
SVOX_Result SVOX_Thread_SetPriority
(
    SVOX_Thread         *thread,
    SVOX_Thread_Priority threadPrio
);

#endif


/**
 * Returns the current state of the thread.
 *
 * @see SVOX_Thread_State
 *
 * @param [in] thread Handle to the thread
 * @param [in] isRunningPtr Pointer to a variable in which the state is stored
 *
 * @retval SVOX_RESULT_OK Ok
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 Handle to the thread is invalid.
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 isRunningPtr is NULL.
 *
 */
SVOX_Result SVOX_Thread_GetState
(
    SVOX_Thread       *thread,
    SVOX_Thread_State *isRunningPtr
);

/**
 * Returns the result value the thread function returned.
 * Returns an error if the thread is still running.
 *
 * @param [in] thread Handle to the thread
 * @param [in] resultPtr Pointer to a SVOX_Int32 in which the result is stored.
 *
 * @retval SVOX_RESULT_OK Ok
 * @retval SVOX_RESULT_ERROR_THREAD_INVALID_STATE The thread is still running
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_0 Handle to the thread is invalid.
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 resultPtr is NULL.
 * @retval SVOX_RESULT_ERROR_FATAL Fatal error. System may be unstable now.
 *
 */
SVOX_Result SVOX_Thread_GetThreadResult
(
    const SVOX_Thread *thread,
    void             **resultPtr
);

/**
 * Returns the handle to a thread by name.
 *
 * @param [in] threadName Name of the thread to return the handle to
 * @param [out] threadPtr Handle to the current thread
 *
 * @retval SVOX_RESULT_OK Ok
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_0 threadName is NULL.
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 threadPtr is NULL.
 * @retval SVOX_RESULT_ERROR_OPEN The thread with the given name does not exist.
 * @retval SVOX_RESULT_ERROR_FATAL Fatal error. System may be unstable now.
 */
SVOX_Result SVOX_Thread_GetThreadByName
(
    const SVOX_Char *threadName,
    SVOX_Thread    **threadPtr
);

/**
 * @brief Give control to other thread
 *
 * The SVOX_Thread_Yield() function yields the processor from the currently
 * executing thread to another ready-to-run, active thread of
 * equal or higher priority.
 *
 * If no other thread is ready to run the current thread continues
 * execution.
 *
 */
void SVOX_Thread_Yield
    ();


/**
 * @brief Register Hook for thread
 *
 * The SVOX_Thread_RegisterHook() function registers the given handler
 * function for the desired hook. 
 * To deregister handler @see SVOX_Thread_DeRegisterHook().
 * Signature for hooks is void hookfunction(SVOX_Thread *,void *userdata,SVOX_Thread_HookType).
 *
 * @param hookType enum value to define which type of hook should be set.
 * @param function function pointer to call (@see SVOX_Thread_HookFunction)
 * @param userData opaque user data pointer that will be passed to the hook.
 */
SVOX_Result SVOX_Thread_RegisterHook(SVOX_Thread_HookType hookType, SVOX_Thread_HookFunction function, void *userData);

/**
 * @brief DeRegister Hook for thread
 *
 * The SVOX_Thread_DeRegisterHook() function unregisters the given
 * handler function for the desired hook, that was registered before
 * using SVOX_ThreadRegisterHook().
 *
 * @param hookType enum value to define which type of hook should be deregistered.
 * @param function function pointer used for registration (@see SVOX_Thread_HookFunction)
 */
SVOX_Result SVOX_Thread_DeRegisterHook(SVOX_Thread_HookType hookType, SVOX_Thread_HookFunction function);
    

/* -----------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif


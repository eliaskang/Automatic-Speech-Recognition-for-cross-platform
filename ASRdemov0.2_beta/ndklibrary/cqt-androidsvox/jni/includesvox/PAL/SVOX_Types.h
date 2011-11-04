/**
 * @brief Datypes
 *
 * This file contains datatype definitions for all platforms.
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

#ifndef __SVOX_TYPES_H__
#define __SVOX_TYPES_H__

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

/* -----------------------------------------------------------------------*/

/* values according to limits.h */
#define SVOX_32S_MIN (-2147483647 - 1) /**< minimum signed 32 bit value */
#define SVOX_32S_MAX 2147483647        /**< maximum signed 32 bit value */

#define SVOX_32U_MAX 0xffffffff       /**< maximum unsigned 32 bit value */

#define SVOX_16S_MIN (-32767 - 1)      /**< minimum signed 16 bit value */
#define SVOX_16S_MAX 32767             /**< maximum signed 16 bit value */

#define SVOX_16U_MAX 0xffff           /**< maximum unsigned 16 bit value */

#define SVOX_8S_MIN (-127 - 1)          /**< minimum signed 8 bit value */
#define SVOX_8S_MAX 127                 /**< maximum signed 8 bit value */

#define SVOX_8U_MAX 0xff               /**< maximum unsigned 8 bit value */


/**
 * signed 32 bit integer datatype
 */
typedef signed int SVOX_32S;

/**
 * unsigned 32 bit integer datatype
 */
typedef unsigned int SVOX_32U;

/**
 * signed 16 bit integer datatype
 */
typedef short SVOX_16S;

/**
 * unsigned 16 bit integer datatype
 */
typedef unsigned short SVOX_16U;

/**
 * unsigned 16 bit integer datatype as wide character
 */
typedef unsigned short SVOX_WChar;

/**
 * unsigned 32 bit integer datatype as ucs-4 character
 */
typedef unsigned int SVOX_Ucs4Char;

/**
 * size_t for sizes
 */
typedef SVOX_32U SVOX_size_t;

/**
 * character for strings
 * Macro used in debug because some debuggers
 * don't show strings if they are not char*
 */
#ifndef NDEBUG
#define SVOX_Char char
#else
typedef char SVOX_Char;
#endif

/**
 * unsigned 8 bit integer
 * @warning do not use this data type for
 * strings.
 * @see SVOX_Char
 */
typedef unsigned char SVOX_8U;

/**
 * signed 8 bit integer
 * @warning do not use this data type for
 * strings.
 * @see SVOX_Char
 */
typedef signed char SVOX_8S;


/**
 * double precision floating point
 */
typedef double SVOX_Double;

/**
 * floating point
 */
typedef float SVOX_Float;

/**
 * Varargs
 */
#ifdef __GNUC__

/* taken from GCC's stdarg.h */
#define SVOX_va_start(v, l) __builtin_va_start(v, l)
#define SVOX_va_end(v) __builtin_va_end(v)
#define SVOX_va_arg(v, l) __builtin_va_arg(v, l)
#define SVOX_va_copy(d, s) __builtin_va_copy(d, s)
typedef __builtin_va_list SVOX_va_list;

#elif (SVOX_PLATFORM == SVOX_PLATFORM_WinCE)

#include <stdarg.h>
#include <stdio.h>

#define SVOX_va_start va_start
#define SVOX_va_arg va_arg
#define SVOX_va_end va_end
#define SVOX_va_copy(d, s) ((d) = (s))
typedef va_list SVOX_va_list;

#elif (SVOX_PLATFORM == SVOX_PLATFORM_Windows)

#include <vadefs.h>

#define SVOX_va_start _crt_va_start
#define SVOX_va_arg _crt_va_arg
#define SVOX_va_end _crt_va_end
#define SVOX_va_copy(d, s) ((d) = (s))
typedef va_list SVOX_va_list;

#elif (SVOX_PLATFORM == SVOX_PLATFORM_ADS)

#define SVOX_va_start(v, l) ((void)(*(v) = __va_start(l)))
#define SVOX_va_end(v) ((void)(*(v) = 0))
#define SVOX_va_arg(v, l) __va_arg(*(v), l)
#define SVOX_va_copy(d, s) ((void)(*(d) = *(s)))
typedef int *SVOX_va_list[1];

#else
#error "Types for SVOX_va_* not yet implemented for this compiler. Please check SVOX_Types.h"
#endif

/**
 * Macros for deprecated functions (only windows)
 */

#if (SVOX_PLATFORM == SVOX_PLATFORM_Windows)

#if (!defined(SVOX_DONT_WARN_ON_DEPRECATED_FUNCTIONS))

#if (_MSC_FULL_VER >= 140050320)
#define SVOX_DEPRECATE_TEXT(_Text) __declspec(deprecated(_Text))
#else
#define SVOX_DEPRECATE_TEXT(_Text) __declspec(deprecated)
#endif
#define SVOX_DEPRECATED_FUNCTION(_Replacement) \
    SVOX_DEPRECATE_TEXT(                       \
    "This function or variable may be unsafe. Consider using " #_Replacement " instead.")
#else
/* we have seen the driective to ignore the deprecated */
#define SVOX_DEPRECATED_FUNCTION(_Replacement)
#endif
#else
/* all other platforms */
#define SVOX_DEPRECATED_FUNCTION(_Replacement)
#endif

/**
 * Result values for all SVOX-Pal functions
 */
typedef enum
{
    SVOX_RESULT_OK = 0,                                 /*!< Ok */
    SVOX_RESULT_ERROR,                                  /*!< Generic error */
    SVOX_RESULT_TIMEOUT,                                /*!< Timeout, not as error. E.g. waiting for a mutex or semaphore */
    SVOX_RESULT_AT_END,                                 /*!< End of something was reached, e.g. Iterator at end of list, .... */
    SVOX_RESULT_MUTEX_ALREADY_LOCKED,                   /*!< Trylock on mutex failed */
    SVOX_RESULT_ERROR_ALREADY_IN_USE,                   /*!< Resource is already in use */
    SVOX_RESULT_ERROR_NOT_FOUND,                        /*!< Something was not found */
    SVOX_RESULT_ERROR_INVALID,                          /*!< Something is invalid */

    SVOX_RESULT_ERROR_NO_MEMORY,                        /*!< The system could not allocate memory for a specific function */
    SVOX_RESULT_ERROR_OPEN,                             /*!< The system failed to open a file, semaphore,... */
    SVOX_RESULT_ERROR_OVERFLOW,                         /*!< There was an overflow in a value */
    SVOX_RESULT_ERROR_INVALID_HANDLE,                   /*!< The check of the magic id inside the handle failed. */

    SVOX_RESULT_ERROR_INVALID_ARG_1,                    /*!< invalid argument 1 (e.g. NULL pointer) */
    SVOX_RESULT_ERROR_INVALID_ARG_2,                    /*!< invalid argument 2 (e.g. NULL pointer) */
    SVOX_RESULT_ERROR_INVALID_ARG_3,                    /*!< invalid argument 3 (e.g. NULL pointer) */
    SVOX_RESULT_ERROR_INVALID_ARG_4,                    /*!< invalid argument 4 (e.g. NULL pointer) */
    SVOX_RESULT_ERROR_INVALID_ARG_5,                    /*!< invalid argument 5 (e.g. NULL pointer) */
    SVOX_RESULT_ERROR_INVALID_ARG_6,                    /*!< invalid argument 6 (e.g. NULL pointer) */

    SVOX_RESULT_ERROR_NOT_RUNNING,                      /*!< the system needed by this function is not running */
    SVOX_RESULT_ERROR_PERMISSION,                       /*!< there are not permissions to execute the function */
    SVOX_RESULT_ERROR_INTERNAL_HANDLE_INVALID,          /*!< the pal internal handle is invalid */
    SVOX_RESULT_ERROR_UNKNOWN,                          /*!< an unknown error occurd - e.g. a result value which is not expected */

    SVOX_RESULT_ERROR_NO_ACCESS,                        /*!< no access to perform operation */

    SVOX_RESULT_ERROR_THREAD_PRIORITY_NOT_SUPPORTED,    /*!< changing the thread priority is not supported */
    SVOX_RESULT_ERROR_THREAD_INVALID_PRIORITY,          /*!< the thread priority is invalid */
    SVOX_RESULT_ERROR_THREAD_CAN_NOT_JOIN,              /*!< the thread can't be joined */
    SVOX_RESULT_ERROR_THREAD_DOES_NOT_EXIST,            /*!< the thread does not exist */
    SVOX_RESULT_ERROR_THREAD_INVALID_NAME,              /*!< the thread name is invalid */
    SVOX_RESULT_ERROR_THREAD_NAME_ALREADY_EXISTS,       /*!< a thread with this name already exists */
    SVOX_RESULT_ERROR_THREAD_DEADLOCK,                  /*!< a deadlock was detected */
    SVOX_RESULT_ERROR_THREAD_CREATE,                    /*!< creating a thread was not possible */
    SVOX_RESULT_ERROR_THREAD_INVALID_STACK_SIZE,        /*!< the specified stack size is invalid */
    SVOX_RESULT_ERROR_THREAD_NO_MORE_AVAILABLE,         /*!< the maximum number of threads on the system is reached */
    SVOX_RESULT_ERROR_THREAD_INVALID_ATTRIBUTES,        /*!< changing thread's attributes failed */
    SVOX_RESULT_ERROR_THREAD_INVALID_STATE,             /*!< state of the thread is not valid for this action */

    SVOX_RESULT_ERROR_FATAL,                            /*!< fatal error. this error is set if the system is in an unstable state. e.g. mutex can't be unlocked,... */

    SVOX_RESULT_ERROR_SYNTAX,                           /*!< syntax error */

    SVOX_RESULT_ERROR_LENGTH,                           /*!< a field does not have enough room to hold the result */
    
    SVOX_RESULT_ERROR_FILE_NOT_FOUND = SVOX_RESULT_ERROR_NOT_FOUND, /*!< file not found, deprecated use SVOX_RESULT_ERROR_NOT_FOUND instead @deprecated  */
    /**
     * Do not use this value - it is just for counting
     * the number of result values.
     */
    SVOX_RESULT___LAST___
} SVOX_Result;

/**
 * Use this to get rid of warnings "unreferenced formal parameter"
 */
#define SVOX_UNUSED_ARGUMENT(x) (void)x

#ifndef NULL
#define NULL (0)
#endif

/**
 * Boolean data type
 */
typedef enum
{
    SVOX_FALSE = (0),
    SVOX_TRUE  = (!0)
} SVOX_Bool;


/* for armcc, tcc  */
#if (defined(__arm) && (SVOX_PLATFORM != SVOX_PLATFORM_iPhone) && (SVOX_PLATFORM != SVOX_PLATFORM_Symbian))
#define SVOX_ALIGNMENT_PREFIX(X) __align(X)
#else
#define SVOX_ALIGNMENT_PREFIX(X) /* empty, is compiler/linker-specific */
#endif


/* well, do something for gcc, g++ too. Of course these require
 *  * a postfix instead of a prefix....so...
 *   */
#ifdef __GNUC__       /* GNUC */
#define SVOX_ALIGNMEMT_POSTFIX(X) __attribute__ ((aligned(X)))
#else
#ifdef __GNUP__      /* GNU C++ */
#define SVOX_ALIGNMENT_POSTFIX(X) __attribute__ ((aligned(X)))
#else
#define SVOX_ALIGNMENT_POSTFIX(X) /* empty, is compiler/linker-specific */
#endif
#endif


/**
 * Returns a pointer to a string containing the value
 * for an error code.
 *
 * @param[in] res result value of a SVOX PAL function
 *
 * @retval pointer to a string containing the name for the
 *         result value
 */
const SVOX_Char *SVOX_GetErrorString
(
    SVOX_Result res
);

/**
 * Check if the types are ok.
 * Checks width of datatypes, strings etc.
 *
 * @retval SVOX_RESULT_OK Everything is ok
 * @retval SVOX_RESULT_ERROR_FATAL Datatypes are not ok
 *
 */
SVOX_Result SVOX_CheckTypeIntegrity
    ();

/**
 * @brief Check Endianness.
 *
 * This function can be used to distinguish between little and big endian platforms. 
 *
 * @retval SVOX_TRUE on little endian platforms
 * @retval SVOX_FALSE on all others platforms (i.e. big endian platforms)
 *
 */
SVOX_Bool SVOX_IsLittleEndian
    ();



/* -----------------------------------------------------------------------*/

#ifdef SVOX_PLATFORM_CLEANUP_DEFINES
#include "SVOX_Platform.h"
#undef SVOX_PLATFORM_CLEANUP_DEFINES
#endif

#ifdef __cplusplus
}
#endif

#endif


/**
 * @brief Assertions and low level debugging functions
 *
 * This file contains assertions and low level debugging functions
 * for checking data at runtime and invoking the debugger.
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

#ifndef __SVOX_ASSERT_H__
#define __SVOX_ASSERT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------------*/
/* Default setting is to have assertions only for debug builds */
/* -----------------------------------------------------------------------*/
#if !defined(NDEBUG)
#define SVOX_PAL_ENABLE_ASSERT
#endif

/* Override default settings */
#if (defined(SVOX_PAL_ENABLE_RELEASE_ASSERTIONS) && defined(NDEBUG)) 
#define SVOX_PAL_ENABLE_ASSERT
#endif

/* Override default settings */
#if (defined(SVOX_PAL_DISABLE_DEBUG_ASSERTIONS) && !defined(NDEBUG)) 
#undef SVOX_PAL_ENABLE_ASSERT
#endif

#ifdef SVOX_PAL_ENABLE_ASSERT

#include "SVOX_File.h"
#include "SVOX_Types.h"

/**
 * This function invokes the debugger. It is only
 * available in debug builds and will compile to
 * (void)0 in release versions.
 */
void SVOX_DbgBreak
    ();

/**
 * This function is a printf which is only
 * available in debug builds. It compiles to
 * (void)0 in release versions
 *
 * Because of the use of varargs for the
 * SVOX_printf you have to use double
 * braces around the arguments for the function
 *
 * @code
 * SVOX_DbgPrintf(("Hallo %s}\n","Welt"));
 * @endcode
 *
 */
#define SVOX_DbgPrintf(x) SVOX_printf x

/**
 * This function is a printf which is only
 * available in debug builds. It compiles to
 * (void)0 in release versions.
 * Invokes the debugger after print.
 *
 * Because of the use of varargs for the
 * SVOX_printf you have to use double
 * braces around the arguments for the function
 *
 * @code
 * SVOX_DbgPrintfBreak(("Hallo %s}\n","Welt"));
 * @endcode
 *
 */
#define SVOX_DbgPrintfBreak(x) (void)((SVOX_printf x, 0) || (SVOX_DbgBreak(), 0))

/**
 * Assertion function called by SVOX_Assert* macros.
 * This function tries to invoke the debugger.
 *
 * @param[in] expression string containing the expression
 * @param[in] message extended message
 * @param[in] filename name of the file in which the assertion occured
 * @param[in] lineNumber line number in the file in which the assertion occured
 *
 */
void SVOX_Assert_Internal
(
    const SVOX_Char *expression,
    const SVOX_Char *message,
    const SVOX_Char *filename,
    SVOX_32S         lineNumber
);

/**
 * Simple Assertion.
 * Checks _EXPR, if _EXPR is not valid the message is printed
 * and the debugger is invoked.
 *
 * @warning The complete assert replaced by (void)0 in
 * release mode. Do not implement vital functions
 * inside the assertion!
 *
 * @param[in] _EXPR Expression to check
 */
#define SVOX_Assert(_EXPR) (void)((!!(_EXPR)) || (SVOX_Assert_Internal(#_EXPR, "", __FILE__, __LINE__), 0))

/**
 * Enhanced Assertion.
 * Checks _EXPR, if _EXPR is not valid the message is printed
 * and the debugger is invoked.
 *
 * @warning The complete assert replaced by (void)0 in
 * release mode. Do not implement vital functions
 * inside the assertion!
 *
 * @param[in] _EXPR Expression to check
 * @param[in] _MSG Message
 */
#define SVOX_AssertEx(_EXPR, _MSG) (void)((!!(_EXPR)) || (SVOX_Assert_Internal(#_EXPR, _MSG, __FILE__, __LINE__), 0))


#ifdef SVOX_DEBUG_ENABLE_STRICT_PARAMETER_CHECKS

/**
 * Enhanced Assertion.
 *
 * Checks _EXPR, if _EXPR is not valid the message is printed
 * and the debugger is invoked.
 *
 * @warning The complete assert replaced by (void)0 in
 * release mode. Do not implement vital functions
 * inside the assertion!
 *
 * Use this assertion only for extended API checks.
 * You can enable it by setting SVOX_ASSERT_ENABLE_STRICT_PARAMETER_CHECKS
 *
 * @param[in] _EXPR Expression to check
 * @param[in] _MSG Message
 */
#define SVOX_AssertEx_Strict(_EXPR, _MSG) \
    (void)((!!(_EXPR))                    \
           || (SVOX_Assert_Internal(#_EXPR, _MSG, __FILE__, __LINE__), 0))
#define SVOX_DbgPrintfBreak_Strict(x) (void)((SVOX_printf x, 0) || (SVOX_DbgBreak(), 0))
#else
#define SVOX_AssertEx_Strict(_EXPR, _MSG) (void)0
#define SVOX_DbgPrintfBreak_Strict(x) (void)(0)
#endif

#else
#define SVOX_Assert(_EXPR) (void)0
#define SVOX_AssertEx(_EXPR, _MSG) (void)0
#define SVOX_AssertNotNULL(_EXPR, _MSG) (void)0
#define SVOX_AssertEx_Strict(_EXPR, _MSG) (void)0
#define SVOX_DbgBreak() (void)0
#define SVOX_DbgPrintf(x) (void)0
#define SVOX_DbgPrintfBreak(x) (void)0
#define SVOX_DbgPrintfBreak_Strict(x) (void)0
#endif


/* -----------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif


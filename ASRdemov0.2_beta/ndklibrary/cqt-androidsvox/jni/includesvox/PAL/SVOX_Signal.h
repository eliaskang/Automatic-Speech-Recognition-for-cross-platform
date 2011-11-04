/**
 * @brief Signals
 *
 * This file implements the functions for working with signals.
 * It is strongly adived not to use these functions because
 * there is no consistent behaviour across all systems!
 * There is no support for signals for Windows CE and Mobile.
 *
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

#ifndef __SVOX_SIGNAL_H__
#define __SVOX_SIGNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------------*/

#include "SVOX_Types.h"

typedef enum
{
    SVOX_SIGNAL_ABRT, /**< abort signal */
    /* SVOX_SIGNAL_FPE, disabled because windows handles it*/
    SVOX_SIGNAL_ILL,  /**< illegal instruction, will not be thrown by windows nt */
    SVOX_SIGNAL_INT,  /**< interrupt */
    SVOX_SIGNAL_SEGV, /**< segment violation */
    SVOX_SIGNAL_TERM  /**< software termination, will not be thrown by windows nt */
} SVOX_Signal;


/**
 * Datatype for signal handler pointers
 * The paramter to the function must not be used.
 */
typedef void (*SVOX_sig_t)(SVOX_32S);

/**
 * Special value setting the default signal handler
 */
static const SVOX_sig_t SVOX_SIG_DFL = (SVOX_sig_t)0;

/**
 * Special value setting the ignore signal handler
 */
static const SVOX_sig_t SVOX_SIG_IGN = (SVOX_sig_t)1;

/**
 * Error result value for SVOX_signal
 */
static const SVOX_sig_t SVOX_SIG_ERR = (SVOX_sig_t)(-1);

/**
 * @brief Install signal handler
 *
 * Sets or removes a signal handler.
 *
 * If you want a signal handler to be called again after a signal
 * is called you should set the handler again in the first
 * line of the handler.
 *
 * To remove a handler you should set the handler to SVOX_SIG_IGN
 * to ignore it or SVOX_SIG_DFL to set the default handler.
 *
 * @remark The set of signals available is reduced to the set
 * available under windows. Windows seems to have the minimum set
 * of handlers.
 *
 * @remark On some platforms (Windows CE) there are no signal handlers.
 * On these the signal handler set SVOX_SIG_ERR.
 *
 * @warning You sould not use this function because it is not possible
 * to implement the same behaviour across all platforms.
 *
 * @warning Some platforms remove the signal handler if a signal
 * occurs and replace it with SVOX_SIG_DFL before calling
 * the signal handler function.
 *
 * This may result into a problem even if settings the signal
 * handler again inside the handler because if the signal
 * occurs twice the default handler may be called.
 *
 * Some platforms leave the signal handler untouched before
 * calling the function.
 *
 * The signal function get a parameter with the os signal value
 * which is not mapped back to the SVOX types.
 * So the value must not be used!
 *
 * @warning On windows pressing CTRL-C creates a new thread
 * even if working in a single threaded application. This
 * causes the application to be multithreaded!
 *
 * @warning Within the signal handler following things must not be
 * done:
 * @li No IO functions must be called: SVOX_printf, SVOX_fread,...
 * @li No heap functions like SVOX_malloc, SVOX_free, SVOX_strdub,...
 * @li No function that generates a system call like SVOX_time,...
 * @li No SVOX_longjmp
 *
 * @remark SVOX_SIGNAL_FPE is disabled because handling it
 *         needs restarting the floating point unit under
 *         several oparating systems. And the signature of the
 *
 *
 * @remark Under Windows NT, SVOX_SIGNAL_ILL and SVOX_SIGNAL_TERM
 *         are not generated but could be triggered by
 *         SVOX_raise.
 *
 * @remark on Windows CE and Mobile there is nofunctionality and the
 * return value is always SVOX_RESULT_ERROR
 *
 * @param[in] sig The signal to set the handler for
 *
 * @param[in] func Pointer to the function to call or SVOX_SIG_DFL for
 *             the default handler, SVOX_SIG_IGN to ignore the signal
 *
 * @result Pointer to the old signal handler (which also
 *         includes SVOX_SIG_DFL and SVOX_SIG_IGN.
 * @retval SVOX_SIG_ERR in case of an error
 */
SVOX_sig_t SVOX_signal
(
    SVOX_Signal sig,
    SVOX_sig_t  func
);

/**
 * @brief Invokes a signal handler
 *
 * This function calls a signal handler.
 * If raise is not implemented or there was an other error.
 *
 * @remark on Windows CE and Mobile there is nofunctionality and the
 * return value is always NULL
 *
 * @param[in] sig signal to raise
 * @retval SVOX_RESULT_OK if ok
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 if mapping the signal failed
 * @retval SVOX_RESULT_ERROR if an error occured
 *
 */
SVOX_Result SVOX_raise
(
    SVOX_Signal sig
);

#ifdef __cplusplus
}
#endif

#endif


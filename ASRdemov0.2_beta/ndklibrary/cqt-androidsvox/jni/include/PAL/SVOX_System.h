/**
 * @brief Functions for extended system interaction
 *
 * This file contains more advanced functions for
 * interacting with the system enviroment.
 *
 * @date $Date: 2010-05-03 09:51:27 +0200 (Mo, 03 Mai 2010) $
 * @author Stefan Sablatnoeg
 * @file SVOX_System.h
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


#ifndef __SVOX_System_H__
#define __SVOX_System_H__

#include "SVOX_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Execute an external command
 *
 * Executes a command. cmd must be a valid command for the shell that executed it.
 * PAL cannot make these portable. Keep in mind, that you might need escaping.
 * SVOX_System(NULL) will return 0 on systems where no comand
 * processor is available, non zero otherwise.
 *
 * @param[in] command with path name and arguments to pass to the executing shell
 *
 * @retval return value of the command or if \a cmd was NULL see above
 */
SVOX_32S SVOX_system
(
    const SVOX_Char *cmd
);

/**
 * Retrieve environment variable.
 *
 * It is required to provide a pointer to a storage area, otherwise
 * this would not be thread safe if the environment changes.  The
 * string will always be appended a trailing zero, so strings copied
 * to the user provided buffer will be guaranteed to be zero
 * terminated. \a size is used to provide the size of the string \a
 * value can take. If \a value is NULL, only the size of the
 * environment string is calculated.
 *
 * @note This is not yet fully thread safe on all platforms, iff (!)
 * another thread changes the environment (which is not supported at
 * the moment through PAL)
 *
 * @param[in] name of the environment variable to retrieve
 * @param[out] pointer to character array to hold the result or NULL
 * if you are only interested in the length required
 * @param[in/out] length of the field passed in value, length required
 * for the value
 * @retval SVOX_RESULT_OK    if the request could be filled in
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 \a key is a NULL pointer
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_3 \a size is a NULL pointer
 * @retval SVOX_RESULT_ERROR_LENGTH The given size is insufficient to
 * complete the request, the content of \a value is undefined.
 * @retval SVOX_RESULT_ERROR_NOT_FOUND key is not present in
 * environment. \a *size is set to 0 in this case
 */
SVOX_Result SVOX_getenv
(
    const SVOX_Char *key,
    SVOX_Char *value,
    SVOX_size_t *size
);

#ifdef __cplusplus
}
#endif

#endif


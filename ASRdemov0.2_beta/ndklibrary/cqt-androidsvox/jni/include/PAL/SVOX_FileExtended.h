/**
 * @brief Functions for extended file management
 *
 * This file contains more advanced functions for
 * handling files and directories, that are not common on all platforms,
 * For the more basic function @see SVOX:File.h in PALBASIC 
 *
 * All functions work in an operating system
 * independent way so it does not matter if you
 * use "/" vs "\" as path separator.
 *
 * @date $Date: 2010-05-03 09:51:27 +0200 (Mo, 03 Mai 2010) $
 * @author Stefan Sablatnoeg
 * @file SVOX_FileExtended.h
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


#ifndef __SVOX_FileExtended_H__
#define __SVOX_FileExtended_H__

#include "SVOX_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create the directory
 *
 * @param[in] path name of the new path
 *
 * @retval SVOX_RESULT_OK if ok
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 if path is NULL
 * @retval SVOX_RESULT_ERROR if the creation failed
 */
SVOX_Result SVOX_File_CreateDirectory
(
    const SVOX_Char *path
);

/**
 * Remove the directory
 *
 * @param[in] path name of the path to delete
 *
 * @retval SVOX_RESULT_OK if ok
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 if path is NULL
 * @retval SVOX_RESULT_ERROR if the removal failed
 */
SVOX_Result SVOX_File_RemoveDirectory
(
    const SVOX_Char *path
);
    
#ifdef __cplusplus
}
#endif

#endif


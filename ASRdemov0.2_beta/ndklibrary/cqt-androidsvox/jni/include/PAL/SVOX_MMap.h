/**
 * @brief Memory mapping
 *
 * This file contains functions used for memory mapping.
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

#ifndef __SVOX_MMAP_H__
#define __SVOX_MMAP_H__

#ifdef __cplusplus
extern "C" {
#endif

/* -----------------------------------------------------------------------*/

#include "SVOX_Types.h"

/**
 */
struct SVOX_MMap_;

/**
 * Handle which keeps the thread data.
 */
typedef struct SVOX_MMap_ SVOX_MMap;


typedef enum
{
    SVOX_MMAP_READ,
    SVOX_MMAP_CREATE,
    SVOX_MMAP_READWRITE
} SVOX_MMap_OpenMode;

/**
 * @brief Open/Create a memory mapped file
 *
 * Maps a file into memory.
 *
 * If SVOX_MMAP_READ is specified, we will get a pointer which we ae only allowed to read,
 * writing may result in a SIGSEGV. The content of the file is never changed.
 * If SVOX_MMAP_READWRITE is specified, a pointer to read and writeable memory area will be returned.
 * The file is required to exist. It will be enlarged to the given size if necesary. If the size is smaller,
 * the file will _not_ be truncated.
 * If SVOX_MMAP_CREATE is specified, the behaviour is the same as for SVOX_MMAP_READWRITE, but the file will be created
 * if necessary. It will be create with permission 0777, this might be a security issue.
 *
 * @param[out] mmapPtr A pointer to store the SVOX_MMap handle
 * @param[in] fileName name of the file to open
 * @param[in] fileSize size of the file to map, may be -1 if opened for reading,
 *            in this case the routine determines the size of the file
 * @param[in] openMode mode to open the file
 *
 * @retval SVOX_RESULT_OK ok
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 mmap is NULL
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 fileName is NULL
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_3 fileSize is < 0 and openMode is SVOX_MMAP_WRITE
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_4 invalid openMode
 * @retval SVOX_RESULT_ERROR_OPEN the file could not be opened
 * @retval SVOX_RESULT_ERROR_NO_MEMORY no memory
 *
 */
SVOX_Result SVOX_MMap_Open
(
    SVOX_MMap        **mmapPtr,
    const SVOX_Char   *fileName,
    SVOX_32S           fileSize,
    SVOX_MMap_OpenMode openMode
);

/**
 * @brief Closes a file mapping
 *
 * Closes the file mapping
 *
 * @param[in] mmapHandle Filemapping to close. The handle must not be used
 *                anymore after closing it
 *
 * @retval SVOX_RESULT_OK ok
 * @retval SVOX_RESULT_ERROR_INVALID_HANDLE mmapHandle is not a handle
 *          optained by SVOX_MMap_Open or was already freed
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 mmapHandle ist NULL
 */
SVOX_Result SVOX_MMap_Close
(
    SVOX_MMap *mmapHandle
);

/**
 * @brief Get a pointer to a given offset in the file
 *
 * Sets the pointer to the memory using the file mapping
 *
 * @retval SVOX_RESULT_OK ok
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 address is NULL
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 mmap is NULL
 * @retval SVOX_RESULT_ERROR_INVALID_HANDLE mmapHandle is not a handle
 *          optained by SVOX_MMap_Open or was already freed
 * @retval SVOX_RESULT_ERROR_OVERFLOW offset is outside the file size
 */
SVOX_Result SVOX_MMap_GetPointer
(
    void      **addressPtr,
    SVOX_MMap  *mapping,
    SVOX_size_t offset
);


#ifdef __cplusplus
}
#endif

#endif


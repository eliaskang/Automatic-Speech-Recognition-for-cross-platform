/**
 * @brief Unicode functions
 *
 * All kinds of unicode functions
 *
 * The functions are modified to be robust agains NULL-ptrs.
 *
 * @date $Date: 2010-10-06 10:41:15 +0200 (Wed, 06 Oct 2010) $
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

#ifndef __SVOX_UNICODE_H__
#define __SVOX_UNICODE_H__

#ifdef __cplusplus
extern "C" {
#endif


/* -----------------------------------------------------------------------*/

#include "SVOX_Types.h"

/** Performs a lowercase conversion of the wide character given.  The
 * wide character is interpreted as a unicode character in UCS-2
 * coding.
 *
 * @remark This is not completely equivalent to UTF-16 !!
 *
 * @remark The lowercase conversion follows the method of casefolding
 * as described in the unicode book chapter 5.18. this implies that it
 * is independent of the actual locale. The method implemented is the
 * simple one that does a one character to one character mapping. for
 * Details see also the CaseFolding.txt file version 5.2.0 at www.unicode.org
 *
 * @todo { The mapping might need to change given the locale settings of the system. }
 * @todo { The mapping is not correct for characters that map onto two charcters through "lower casing". }
 *
 */
SVOX_WChar SVOX_towlower
(
    SVOX_WChar in
);


/** Performs a lowercase comparison of strings of wide characters.
 * wide characters are interpreted as unicode characters.
 * Returns:
 *          < 0 when string1 less than string2,
 *          0   when string1 identical to string2 or
 *          > 0  when string1 greater than string2
 *          0 when it is not implemented for the current platform, this can be checked
 *          with the define
 *
 * @see SVOX_towlower
 *
 * As this function uses the SVOX_towlower function, this function
 * also follows the method described for Caseless Matching in the
 * unicode book chapter 5.18.  It does not implement locale specific
 * differences.  The case folding is based on the CaseFolding.txt
 * 5.2.0, it implements only the simple form (i.e. all mappings with
 * status C+S).
 *
 */
SVOX_32S SVOX_wcsicmp
(
    const SVOX_WChar *string1,
    const SVOX_WChar *string2
);


/**
 * Counts number of UTF-8 Characters.
 *
 * Does count the UTF-8 characters, excluding the trailing Zero.  Does
 * not fully parse the string for validity, only checks the usual high
 * bit escapes
 *
 * @param src  [in] pointer to the UTF-8 string
 * @param srcn [in] number of @a characters to analyze at maximum, use 0 to count the
 * string up to the terminating zero.
 *
 * @return number of characters before the first zero was found, -1 in
 * case of a passed NULL pointer or invalid UTF-8 detected
 */
SVOX_32S SVOX_Utf8CharacterCount
(
    const SVOX_Char  *src,
    SVOX_size_t srcn
);
    
/**
 * Counts number of UCS2 Characters.
 *
 * Does count the UCS-2 characters, excluding the trailing Zero.
 *
 * @param src [in] pointer to the UCS-2 string
 *
 * @return number of characters before the first zero was found, 0 in
 * case of a passed NULL pointer
 */
SVOX_size_t SVOX_Ucs2CharacterCount
(
    const SVOX_WChar  *src
);
    
/**
 * Counts number of UCS4 Characters.
 *
 * Does count the UCS-4 characters, excluding the trailing Zero.
 *
 * @param src [in] pointer to the UCS-4 string
 *
 * @return number of characters before the first zero was found, 0 in
 * case of a passed NULL pointer
 */
SVOX_size_t SVOX_Ucs4CharacterCount
(
    const SVOX_Ucs4Char  *src
);
    
/**
 * Counts number of UCS2 Characters.
 *
 * Does count the UCS-2 characters, and calculate the number of bytes the string would need once recoded as UTF8.
 * The result will never include space required for the trailing zero.
 *
 * @param src [in] pointer to the UCS-2 string
 * @param srcn [in] number of characters to use, use 0 to use the whole string in \a src
 *
 * @return number of bytes a UTF-8 convesion of the UCS-2 string would require
 */
SVOX_size_t SVOX_Ucs2ByteCountAsUtf8
(
    const SVOX_WChar  *src, SVOX_size_t srcn
    );
    
/**
 * Counts number of UCS2 Characters.
 *
 * Does count the UCS-4 characters, and calculate the number of bytes the string would need once recoded as UTF8.
 * The result will never include space required for the trailing zero.
 *
 * @param src [in] pointer to the UCS-2 string
 * @param srcn [in] number of characters to use, use 0 to use the whole string in \a src
 *
 * @return number of bytes a UTF-8 convesion of the UCS-4 string would require
 */
SVOX_size_t SVOX_Ucs4ByteCountAsUtf8
(
    const SVOX_Ucs4Char  *src, SVOX_size_t srcn
    );
    
/**
 * Converts UCS-2 to UTF-8.
 *
 * Does the UCS-2 to UTF-8 conversion. The destination \a dest must be allocated by the caller.
 * The result will always have a termination zero. 
 *
 * @param src [in] pointer to the UCS-2 string
 * @param srcn [in] number of characters to use, use 0 to use the whole string in \a src
 * @param dest [in] pointer to the destination for the UTF-8 string
 * @param destn [in] number of bytes that can be used in \a dest.
 *
 * @retval SVOX_RESULT_OK if the conversion succeeded
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 if a Null pointer was passed for \a src
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_3 if a Null pointer was passed for \a dest
 * @retval SVOX_RESULT_ERROR_LENGTH if \a destn is not large enough to hold the result (nothing is done in this case)
 */
SVOX_Result SVOX_Ucs2ToUtf8
( 
    const SVOX_WChar  *src, 
    SVOX_size_t  srcn, 
    SVOX_Char   *dest, 
    SVOX_size_t  destn
    );
    
/**
 * Converts UTF-8 to UCS-2.
 *
 * Does the UTF-8 to UCS-2 conversion. The destination \a dest must be allocated by the caller.
 * The result will always have a termination zero. 
 *
 * @param src [in] pointer to the UTF-8 string
 * @param srcn [in] number of bytes to use, use 0 to use the whole string in \a src
 * @param dest [in] pointer to the destination for the UCS-2 string
 * @param destn [in] number of characters (of type SVOX_WChar) that can be used in \a dest.
 *
 * @retval SVOX_RESULT_OK if the conversion succeeded
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 if a Null pointer was passed for \a src
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 if \a srcn does not point to a full character (nothing is done in this case). Or if the requested size (\a srcn) ends within a character.
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_3 if a Null pointer was passed for \a dest
 * @retval SVOX_RESULT_ERROR_LENGTH if \a destn is not large enough to hold the result (nothing is done in this case).
 */
SVOX_Result SVOX_Utf8ToUcs2
( 
    const SVOX_Char   *src, 
    SVOX_size_t  srcn, 
    SVOX_WChar  *dest, 
    SVOX_size_t  destn
    );

/**
 * Converts UCS-4 to UTF-8.
 *
 * Does the UCS-4 to UTF-8 conversion. The destination \a dest must be allocated by the caller.
 * The result will always have a termination zero. 
 *
 * If the conversion fails, the contents of *dest is undefined.
 *
 * @param src [in] pointer to the UCS-4 string
 * @param srcn [in] number of characters to use, use 0 to use the whole string in \a src
 * @param dest [in] pointer to the destination for the UTF-8 string
 * @param destn [in] number of bytes that can be used in \a dest.
 *
 * @retval SVOX_RESULT_OK if the conversion succeeded
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 if a Null pointer was passed for \a src
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_3 if a Null pointer was passed for \a dest
 * @retval SVOX_RESULT_ERROR_LENGTH if \a destn is not large enough to hold the result (nothing is done in this case)
 */
SVOX_Result SVOX_Ucs4ToUtf8
( 
    const SVOX_Ucs4Char  *src, 
    SVOX_size_t  srcn, 
    SVOX_Char   *dest, 
    SVOX_size_t  destn
    );
    
/**
 * Converts UTF-8 to UCS-4.
 *
 * Does the UTF-8 to UCS-4 conversion. The destination \a dest must be allocated by the caller.
 * The result will always have a termination zero. 
 *
 * If the conversion fails, the contents of *dest is undefined.
 *
 * @param src [in] pointer to the UTF-8 string
 * @param srcn [in] number of bytes to use, use 0 to use the whole string in \a src
 * @param dest [in] pointer to the destination for the UCS-4 string
 * @param destn [in] number of characters (of type SVOX_WChar) that can be used in \a dest.
 *
 * @retval SVOX_RESULT_OK if the conversion succeeded
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_1 if a Null pointer was passed for \a src
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_2 if \a srcn does not point to a full character (nothing is done in this case). Or if the requested size (\a srcn) ends within a character.
 * @retval SVOX_RESULT_ERROR_INVALID_ARG_3 if a Null pointer was passed for \a dest
 * @retval SVOX_RESULT_ERROR_LENGTH if \a destn is not large enough to hold the result (nothing is done in this case).
 */
SVOX_Result SVOX_Utf8ToUcs4
( 
    const SVOX_Char   *src, 
    SVOX_size_t  srcn, 
    SVOX_Ucs4Char  *dest, 
    SVOX_size_t  destn
    );
    
/* -----------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __SVOX_UNICODE_H__ */

